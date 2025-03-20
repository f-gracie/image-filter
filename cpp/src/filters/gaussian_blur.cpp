// filters/gaussian_blur.cpp

#include "filters/gaussian_blur.hpp"
cv::Mat apply_gaussian_blur(const cv::Mat& input, int kernel_size) {
    cv::Mat output;
    cv::GaussianBlur(input, output, cv::Size(kernel_size, kernel_size), 0);
    return output;
}