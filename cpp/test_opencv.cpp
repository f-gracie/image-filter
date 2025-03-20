// test_opencv.cpp
#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // 打印OpenCV版本
    std::cout << "OpenCV C++ 版本: " << CV_VERSION << std::endl;

    // 读取图片（使用绝对路径避免相对路径问题）
    std::string image_path = "/Users/gracef/Projects/image-filter/assets/input/test.jpeg";
    cv::Mat img = cv::imread(image_path);

    if (img.empty()) {
        std::cerr << "错误：无法读取图片！请检查路径或文件格式。" << std::endl;
        return -1;
    }

    // 显示图片
    cv::imshow("OpenCV C++ 测试", img);
    cv::waitKey(0); // 按任意键关闭窗口

    return 0;
}