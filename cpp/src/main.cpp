// main.cpp
#include <opencv2/opencv.hpp>
#include "filters/edge_detection.hpp"
#include "filters/gaussian_blur.hpp"
#include "filters/hdr_tone_mapping.hpp"
#include "utils/on_screen_display.hpp"

// 定义滤镜类型枚举
enum FilterType {
    EDGE_DETECTION,
    GAUSSIAN_BLUR,
    HDR_TONE_MAPPING
};

const std::map<FilterType, std::string> FILTER_NAMES = {
    {EDGE_DETECTION, "Edge Detection"},
    {GAUSSIAN_BLUR, "Gaussian Blur"},
    {HDR_TONE_MAPPING, "HDR Tone Mapping"}
};


// 全局变量：当前选中的滤镜
FilterType current_filter = EDGE_DETECTION;

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

        std::cout << "当前滤镜: " << current_filter << std::endl;

        // 应用对应滤镜
        cv::Mat filtered;
        switch (current_filter) {
            case EDGE_DETECTION:
                filtered = apply_edge_detection(frame);
                break;
            case GAUSSIAN_BLUR:
                filtered = apply_gaussian_blur(frame, 7); // 使用7x7高斯核
                break;
            case HDR_TONE_MAPPING:
                // 注意：HDR处理前需归一化输入
                cv::Mat frame_float;
                frame.convertTo(frame_float, CV_32FC3, 1.0/255);
                filtered = apply_hdr_tone_mapping(frame_float, 0.8);
                break;
        }

        // 获取当前滤镜名称
        std::string filter_name = FILTER_NAMES.at(current_filter);
        // 左上角矩形显示滤镜名称
        // display_filter_name(filter_name, filtered);
        OSDUtils::draw_filter_status(filtered, filter_name);
        // 显示滤镜结果
        cv::imshow(filter_name, filtered);

        // 按ESC退出
        if (cv::waitKey(1) == 27) break;
    }

    return 0;
}