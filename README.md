# RichTextEditorWidget 演示项目

这是一个演示 `RichTextEditorWidget` 控件使用的Qt6应用程序。

## 功能特性

- **RichTextEditorWidget**: 自定义的富文本编辑器控件
- **人员标签功能**: 输入文本后添加分号(;)自动转换为人员标签
- **标签悬停效果**: 鼠标悬停时标签变色，鼠标指针变为手形
- **多实例支持**: 支持在同一界面创建多个独立的编辑器实例
- **自动高度调整**: 编辑器高度根据内容自动调整
- **现代化UI**: 美观的界面设计和渐变色标签

## 项目结构

```
src/
├── main.cpp                    # 程序入口
├── mainwindow.h               # 主窗口头文件
├── mainwindow.cpp             # 主窗口实现，演示两个RichTextEditorWidget实例
├── richtexteditorwidget.h     # 富文本编辑器控件头文件
├── richtexteditorwidget.cpp   # 富文本编辑器控件实现
├── tagtextobject.h           # 人员标签对象头文件
└── tagtextobject.cpp         # 人员标签对象实现
```

## 使用方法

1. 在文本编辑器中输入文字
2. 在需要转换为人员标签的文字后面输入分号(;)
3. 文字会自动转换为带有渐变背景的人员标签
4. 将鼠标悬停在标签上可以看到颜色变化和手形指针

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
