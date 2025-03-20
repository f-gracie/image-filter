// filters_hdr_tone_mapping.hpp
#pragma once
#include <opencv2/opencv.hpp>

// 声明hdr色调映射滤镜函数
cv::Mat apply_hdr_tone_mapping(const cv::Mat& input, float gamma = 1.2);