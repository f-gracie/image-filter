# 实时摄像头滤镜处理程序

## 项目功能
- 多线程实时视频处理架构（摄像头捕获线程 + 处理显示线程）
- 支持5种实时滤镜效果：
  - 1. 边缘检测（Edge Detection）
  - 2. 高斯模糊（Gaussian Blur）
  - 3. HDR色调映射（HDR Tone Mapping）
  - 4. 复古效果（Vintage）
  - 5. 卡通效果（Cartoon）
- 实时性能监控（显示处理时间、帧率）
- 线程安全的帧数据同步（通过互斥锁保护共享资源）

## 技术特性
- **开发语言**: C++14
- **核心依赖库**: 
  - OpenCV 4@（视频捕获与图像处理）
  - C++标准线程库（多线程管理）


## 使用方法
### 编译要求
- CMake 3.10+
- OpenCV 4@（需配置环境变量）

### 编译指令
```bash
mkdir build && cd build
cmake ..
make
```

### 运行程序
```bash
./RealTimeVideoFilter
```

### 操作指南
- 数字键 `1-5` 切换滤镜效果（对应功能列表顺序）
- 按 `Q` 或 `ESC` 退出程序
- 控制台实时显示：
  - 当前使用滤镜名称
  - 单帧处理时间（毫秒）
  - 实际运行帧率（FPS）

## 代码结构
```
.
├── CMakeLists.txt
├── include                         # 头文件
│   ├── filters
│   │   ├── cartoon.hpp
│   │   ├── edge_detection.hpp
│   │   ├── gaussian_blur.hpp
│   │   ├── hdr_tone_mapping.hpp
│   │   └── vintage.hpp
│   └── utils
│       └── on_screen_display.hpp
└── src
    ├── filters                     # 滤镜算法实现
    │   ├── cartoon.cpp
    │   ├── edge_detection.cpp
    │   ├── gaussian_blur.cpp
    │   ├── hdr_tone_mapping.cpp
    │   └── vintage.cpp
    ├── main.cpp                    # 程序入口与主控制逻辑
    └── utils                       # OSD叠加模块
        └── on_screen_display.cpp
```

## 注意事项
1. 需要连接有效的摄像头设备
2. 首次运行可能提示选择视频输入源
3. HDR处理为模拟浮点数运算，并非真正的HDR视频源
4. 高斯模糊使用固定7x7核大小
