// utils/on_screen_display.hpp
#pragma once
#include <opencv2/opencv.hpp>
#include <string>

namespace OSDUtils {
    /**
     * @brief 在图像上叠加滤镜状态信息
     * @param image 输入/输出图像（直接修改原始图像）
     * @param filter_name 要显示的滤镜名称
     * @param position 文字起始位置（默认左上角：cv::Point(20, 50)）
     * @param text_color 文字颜色（默认绿色：cv::Scalar(0, 255, 0)）
     * @param bg_color 背景框颜色（默认黑色：cv::Scalar(0, 0, 0)）
     */
    void draw_filter_status(
        cv::Mat& image,
        const std::string& filter_name,
        cv::Point position = cv::Point(20, 20),
        cv::Scalar text_color = cv::Scalar(0, 255, 0),
        cv::Scalar bg_color = cv::Scalar(0, 0, 0)
    );
}