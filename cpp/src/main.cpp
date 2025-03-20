// main.cpp
#include <opencv2/opencv.hpp>
#include <atomic>
#include <mutex>
#include <thread>
#include <chrono>

#include "filters/edge_detection.hpp"
#include "filters/gaussian_blur.hpp"
#include "filters/hdr_tone_mapping.hpp"
#include "filters/vintage.hpp"
#include "filters/cartoon.hpp"
#include "utils/on_screen_display.hpp"

// 定义滤镜类型枚举
enum FilterType {
    EDGE_DETECTION,
    GAUSSIAN_BLUR,
    HDR_TONE_MAPPING,
    VINTAGE,
    CARTOON
};

const std::map<FilterType, std::string> FILTER_NAMES = {
    {EDGE_DETECTION, "Edge Detection"},
    {GAUSSIAN_BLUR, "Gaussian Blur"},
    {HDR_TONE_MAPPING, "HDR Tone Mapping"},
    {VINTAGE, "Vintage Effect"},
    {CARTOON, "Cartoon Effect"}
};

// 叠加信息用到的参数 filtered_frame, filter_name, process_time, current_fps

struct FrameInfo{
    cv::Mat filtered_frame;
    std::string filter_name;
    double process_time;
    double current_fps;
};

//--------------------- 全局变量 ---------------------
std::mutex frame_mutex;                     // 保护图像数据的互斥锁
cv::Mat latest_camera_frame;                // 最新捕获的摄像头帧
std::atomic<bool> is_running(true);         // 线程控制标志
// std::mutex info_mutex;                      // 互斥锁
// std::atomic<bool> info_ready{false};        // 信息就绪标志
FrameInfo filtered_frame_info;              // 滤镜处理后的帧信息

std::atomic<FilterType> current_filter{EDGE_DETECTION};  // 原子操作保证线程安全

//--------------------- 线程函数 ---------------------

// 摄像头捕获线程
void captureThread() {
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "摄像头打开失败！" << std::endl;
        is_running = false;
        return;
    }

    cv::Mat tmp_frame;
    while (is_running) {
        cap >> tmp_frame;
        if (tmp_frame.empty()) continue;

        // 加锁更新共享帧
        {
            std::lock_guard<std::mutex> lock(frame_mutex);
            tmp_frame.copyTo(latest_camera_frame);
        }
        
        // 适当休眠避免CPU空转
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

// 处理显示线程
void processThread() {
    cv::Mat processing_frame;
    // 计算帧率用到的变量
    double current_fps = 0.0;
    int frame_counter = 0;
    auto last_time = std::chrono::steady_clock::now();

    while (is_running) {
        auto loop_start = std::chrono::steady_clock::now();
        
        // 1. 获取待处理帧
        {
            std::lock_guard<std::mutex> lock(frame_mutex);
            if (latest_camera_frame.empty()) continue;
            processing_frame = latest_camera_frame.clone(); // 深拷贝减少锁占用时间
        }

        // 2. 应用滤镜
        cv::Mat filtered_frame;
        auto filter_start = std::chrono::high_resolution_clock::now();
        // 使用原子变量读取当前滤镜类型
        FilterType selected_filter = current_filter.load();
        switch (selected_filter) {
            case EDGE_DETECTION:
                filtered_frame = apply_edge_detection(processing_frame);
                break;
            case GAUSSIAN_BLUR:
                filtered_frame = apply_gaussian_blur(processing_frame, 7); // 使用7x7高斯核
                break;
            case VINTAGE:
                filtered_frame = apply_vintage_effect(processing_frame);
                break;
            case CARTOON:
                filtered_frame = apply_cartoon_effect(processing_frame);
                break;
            case HDR_TONE_MAPPING:
                // 注意：HDR处理前需归一化输入
                cv::Mat frame_float;
                processing_frame.convertTo(frame_float, CV_32FC3, 1.0/255);
                filtered_frame = apply_hdr_tone_mapping(frame_float, 0.8);
                break;
        }

        // 滤镜处理耗时计算
        auto filter_end = std::chrono::high_resolution_clock::now();
        double process_time = std::chrono::duration<double, std::milli>(filter_end - filter_start).count();

        // --- 帧率计算部分 ---
        auto current_time = std::chrono::steady_clock::now();
        double elapsed_sec = std::chrono::duration<double>(current_time - last_time).count();
        frame_counter++;

        // 每1秒更新一次帧率（避免频繁刷新）
        if (elapsed_sec >= 1.0) {
            current_fps = frame_counter / elapsed_sec;
            frame_counter = 0;
            last_time = current_time;
        }

        std::string filter_name = FILTER_NAMES.at(selected_filter);

        // 加锁更新共享帧信息
        {
            std::lock_guard<std::mutex> lock(frame_mutex);
            filtered_frame.copyTo(filtered_frame_info.filtered_frame);
            filtered_frame_info.filter_name = filter_name;
            filtered_frame_info.process_time = process_time;
            filtered_frame_info.current_fps = current_fps;
        }

        // 动态调整处理节奏
        auto loop_time = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - loop_start);
        if (loop_time < std::chrono::milliseconds(10)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10 - loop_time.count()));
        }

    }
    
}

int main() {
    // 启动线程
    std::thread cam_thread(captureThread);
    std::thread proc_thread(processThread);

    // 在主线程中处理GUI
    cv::Mat display_frame;
    std::string filter_name;
    double process_time = 0.0;
    double current_fps = 0.0;
    cv::namedWindow("Real-time Camera Filter");
    while (is_running) {
        {
            std::lock_guard<std::mutex> lock(frame_mutex);
            if (!filtered_frame_info.filtered_frame.empty()) {
                filtered_frame_info.filtered_frame.copyTo(display_frame);
                filter_name = filtered_frame_info.filter_name;
                process_time = filtered_frame_info.process_time;
                current_fps = filtered_frame_info.current_fps;
            }
        }


        if (!display_frame.empty()) {

            OSDUtils::draw_filter_status(display_frame, filter_name, process_time);
            OSDUtils::draw_fps(display_frame, current_fps);
            cv::imshow("Real-time Camera Filter", display_frame);
        }

        int key = cv::waitKey(1);
        if (key == 27) {  // ESC退出
            is_running = false;
        } else if (key >= '1' && key <= '5') {  // 数字键切换滤镜
            current_filter = static_cast<FilterType>(key - '1');
        }
    }

    // 清理
    is_running = false;
    cam_thread.join();
    proc_thread.join();
    cv::destroyAllWindows();
    return 0;
}