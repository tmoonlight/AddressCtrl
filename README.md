# Qt CMake 项目

这是一个使用CMake构建的Qt6应用程序示例。

## 功能特性

- 现代化的Qt6 GUI界面
- CMake构建系统
- 跨平台支持（Windows、Linux、macOS）
- 响应式UI设计
- 文本输入和显示功能

## 系统要求

- Qt6 (Core, Widgets, Gui模块)
- CMake 3.16或更高版本
- C++17支持的编译器

## 构建步骤

### Windows (使用Visual Studio)

1. 确保已安装Qt6和CMake
2. 打开命令提示符或PowerShell
3. 进入项目目录：
   ```cmd
   cd c:\Users\shao\testqt2
   ```

4. 创建构建目录：
   ```cmd
   mkdir build
   cd build
   ```

5. 配置项目：
   ```cmd
   cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/msvc2019_64"
   ```
   （请根据你的Qt安装路径调整CMAKE_PREFIX_PATH）

6. 构建项目：
   ```cmd
   cmake --build . --config Release
   ```

7. 运行程序：
   ```cmd
   .\Release\QtApp.exe
   ```

### Linux/macOS

```bash
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH="/path/to/qt6"
make -j4
./QtApp
```

## 项目结构

```
testqt2/
├── CMakeLists.txt          # CMake配置文件
├── README.md              # 项目说明
└── src/                   # 源代码目录
    ├── main.cpp           # 程序入口点
    ├── mainwindow.h       # 主窗口头文件
    ├── mainwindow.cpp     # 主窗口实现
    └── mainwindow.ui      # UI文件（可选）
```

## 应用程序功能

- **文本输入**: 在输入框中输入文本
- **添加文本**: 点击"添加文本"按钮或按回车键将文本添加到历史记录
- **清空**: 清除所有历史记录
- **退出**: 关闭应用程序

## 注意事项

- 确保Qt6已正确安装并配置环境变量
- 如果遇到构建问题，请检查CMAKE_PREFIX_PATH是否指向正确的Qt安装目录
- 在Windows上，程序会自动部署必要的Qt DLL文件
