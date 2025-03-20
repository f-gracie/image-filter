// filters/hdr_tone_mapping.cpp

#include "filters/hdr_tone_mapping.hpp"
cv::Mat apply_hdr_tone_mapping(const cv::Mat& input, float gamma) {
    cv::Mat output;
    cv::Ptr<cv::TonemapDrago> tonemap = cv::createTonemapDrago();
    tonemap->setGamma(gamma);
    tonemap->process(input, output);
    output.convertTo(output, CV_8UC3, 255); // 转换回8位格式
    return output;
}