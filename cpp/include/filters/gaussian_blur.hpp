// filters/gaussian_blur.hpp
#pragma once
#include <opencv2/opencv.hpp>

// 声明高斯模糊滤镜函数
cv::Mat apply_gaussian_blur(const cv::Mat& input, int kernel_size = 5);