// utils/on_screen_display.cpp
#include "utils/on_screen_display.hpp"

void OSDUtils::draw_filter_status(
    cv::Mat& image,
    const std::string& filter_name,
    double process_time,
    cv::Point position,
    cv::Scalar text_color,
    cv::Scalar bg_color
) {
    // 字体配置
    const int font_face = cv::FONT_HERSHEY_SIMPLEX;
    const double font_scale = 0.8;
    const int thickness = 2;
    int baseline = 0;

    // 计算文字尺寸
    cv::Size text_size = cv::getTextSize(
        filter_name, 
        font_face, 
        font_scale, 
        thickness, 
        &baseline
    );

    // 绘制背景框
    cv::rectangle(
        image,
        cv::Point(position.x - 10, position.y - 10), // 左上角
        cv::Point(position.x + text_size.width, position.y + text_size.height + 10),  // 右下角
        bg_color,
        cv::FILLED
    );

    // 绘制文字
    cv::putText(
        image,
        filter_name,
        cv::Point(position.x, position.y + text_size.height),
        font_face,
        font_scale,
        text_color,
        thickness,
        cv::LINE_AA
    );

    // 在滤镜名称下方添加耗时
    std::string time_text = "Time: " + std::to_string(process_time).substr(0,4) + " ms";
    cv::putText(
        image,
        time_text,
        cv::Point(position.x, position.y + 50),  // Y坐标下移
        font_face,
        font_scale * 0.8,  // 字体稍小
        text_color,
        thickness,
        cv::LINE_AA
    );
}

void OSDUtils::draw_fps(cv::Mat& image, double fps, cv::Point position) {
    std::string fps_text = "FPS: " + std::to_string((int)fps);
    cv::putText(
        image,
        fps_text,
        position,
        cv::FONT_HERSHEY_SIMPLEX,
        0.6,                  // 字体大小
        cv::Scalar(0, 255, 0),// 颜色（绿色）
        2,                    // 线宽
        cv::LINE_AA
    );
}