// filters/vintage.cpp
#include "filters/vintage.hpp"

cv::Mat apply_vintage_effect(const cv::Mat& input) {
    cv::Mat output = input.clone();
    for (int y = 0; y < input.rows; y++) {
        for (int x = 0; x < input.cols; x++) {
            cv::Vec3b pixel = input.at<cv::Vec3b>(y, x);
            int r = pixel[2], g = pixel[1], b = pixel[0];
            int new_r = std::min(0.393*r + 0.769*g + 0.189*b, 255.0);
            int new_g = std::min(0.349*r + 0.686*g + 0.168*b, 255.0);
            int new_b = std::min(0.272*r + 0.534*g + 0.131*b, 255.0);
            output.at<cv::Vec3b>(y, x) = cv::Vec3b(new_b, new_g, new_r);
        }
    }
    return output;
}