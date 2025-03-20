// filters/cartoon.cpp
#include "filters/cartoon.hpp"

cv::Mat apply_cartoon_effect(const cv::Mat& input) {
    // Step 1: 双边滤波平滑颜色
    cv::Mat smoothed;
    cv::bilateralFilter(input, smoothed, 9, 75, 75);

    // Step 2: 边缘检测
    cv::Mat gray, edges;
    cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    cv::medianBlur(gray, gray, 7); // 中值滤波降噪
    cv::adaptiveThreshold(gray, edges, 255, 
        cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 9, 2);

    // Step 3: 合并平滑区域和边缘
    cv::Mat edges_bgr;
    cv::cvtColor(edges, edges_bgr, cv::COLOR_GRAY2BGR);
    cv::bitwise_and(smoothed, edges_bgr, edges_bgr);
    return edges_bgr;
}