// main.cpp
#include <opencv2/opencv.hpp>
#include "filters/edge_detection.hpp"
#include "filters/gaussian_blur.hpp"
#include "filters/hdr_tone_mapping.hpp"
#include "filters/vintage.hpp"
#include "filters/cartoon.hpp"
#include "utils/on_screen_display.hpp"
#include <chrono>  // 必须包含的头文件


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


// 全局变量
// 当前选中的滤镜
FilterType current_filter = EDGE_DETECTION;
// 计算帧率用到的变量
auto last_time = std::chrono::steady_clock::now();  // 上一次记录时间
int frame_count = 0;                                // 累计帧数
double current_fps = 0.0;                           // 当前计算的帧率

int main() {
    // 打开默认摄像头（索引0）
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "错误：无法打开摄像头！" << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (true) {
        cap >> frame; // 捕获一帧
        if (frame.empty()) break;

        // 按键检测
        char key = cv::waitKey(1);
        if (key == '1') current_filter = EDGE_DETECTION;
        else if (key == '2') current_filter = GAUSSIAN_BLUR;
        else if (key == '3') current_filter = HDR_TONE_MAPPING;
        else if (key == '4') current_filter = VINTAGE;
        else if (key == '5') current_filter = CARTOON;

        // 滤镜处理耗时统计
        auto start = std::chrono::high_resolution_clock::now();
        // 应用对应滤镜
        cv::Mat filtered;
        switch (current_filter) {
            case EDGE_DETECTION:
                filtered = apply_edge_detection(frame);
                break;
            case GAUSSIAN_BLUR:
                filtered = apply_gaussian_blur(frame, 7); // 使用7x7高斯核
                break;
            case VINTAGE:
                filtered = apply_vintage_effect(frame);
                break;
            case CARTOON:
                filtered = apply_cartoon_effect(frame);
                break;
            case HDR_TONE_MAPPING:
                // 注意：HDR处理前需归一化输入
                cv::Mat frame_float;
                frame.convertTo(frame_float, CV_32FC3, 1.0/255);
                filtered = apply_hdr_tone_mapping(frame_float, 0.8);
                break;
        }

        // 滤镜处理耗时计算
        auto end = std::chrono::high_resolution_clock::now();
        double process_time = std::chrono::duration<double, std::milli>(end - start).count();

        // --- 帧率计算部分 ---
        auto current_time = std::chrono::steady_clock::now();
        double elapsed_sec = std::chrono::duration<double>(current_time - last_time).count();
        frame_count++;

        // 每1秒更新一次帧率（避免频繁刷新）
        if (elapsed_sec >= 1.0) {
            current_fps = frame_count / elapsed_sec;
            frame_count = 0;
            last_time = current_time;
        }

        // 获取当前滤镜名称
        std::string filter_name = FILTER_NAMES.at(current_filter);
        // 左上角矩形显示滤镜名称
        OSDUtils::draw_filter_status(filtered, filter_name, process_time);
        // 左上角显示帧率
        OSDUtils::draw_fps(filtered, current_fps);
        // 显示滤镜结果
        cv::imshow(filter_name, filtered);

        // 按ESC退出
        if (cv::waitKey(1) == 27) break;
    }

    return 0;
}