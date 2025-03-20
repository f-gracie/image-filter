//filters/edge_detection.hpp
#pragma once
#include <opencv2/opencv.hpp>

// 声明边缘检测函数
cv::Mat apply_edge_detection(const cv::Mat& input);