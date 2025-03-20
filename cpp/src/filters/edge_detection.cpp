// filters/edge_detection.cpp
#include "filters/edge_detection.hpp"

// 返回值 cv::Mat
// OpenCV 中的矩阵类，用于存储图像数据
// 参数   const cv::Mat& input
// 输入图像的常量引用（避免拷贝原图像且不可修改原数据）。

cv::Mat apply_edge_detection(const cv::Mat& input) {
    cv::Mat gray, grad_x, grad_y, abs_grad, output;
    
    // 转换为灰度图
    // 将输入的 BGR 彩色图像转为灰度图（边缘检测通常在灰度图上进行，简化计算）
    cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    
    // Sobel 算子：一种边缘检测算子，通过计算图像水平和垂直方向的梯度强度来识别边缘
    // CV_16S：输出图像数据类型为 16 位有符号整数（允许负梯度值）
    // 计算X/Y方向梯度
    cv::Sobel(gray, grad_x, CV_16S, 1, 0, 3);  // X方向 1, 0：计算 X 方向梯度（水平边缘）
    cv::Sobel(gray, grad_y, CV_16S, 0, 1, 3);  // Y方向 0, 1：计算 Y 方向梯度（垂直边缘）
    
    // 转换为8位无符号整型
    // 将含有负值的梯度结果转为 0-255 范围的 8 位无符号整型图像，便于后续显示和处理。
    cv::convertScaleAbs(grad_x, grad_x);
    cv::convertScaleAbs(grad_y, grad_y);
    
    // 合并梯度
    // 加权合并：将 X/Y 方向的梯度图按 0.5 的权重相加，综合两个方向的边缘信息。
    // 公式：output = 0.5*grad_x + 0.5*grad_y + 0
    // 效果：得到完整的边缘图（各向同性叠加）。
    cv::addWeighted(grad_x, 0.5, grad_y, 0.5, 0, output);

    // 新增：将单通道灰度图转换为三通道BGR
    cv::cvtColor(output, output, cv::COLOR_GRAY2BGR);
    
    return output;
}