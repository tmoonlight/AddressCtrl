#!/bin/bash

echo "正在构建Qt5 CMake项目..."

# 创建构建目录
if [ ! -d "build" ]; then
    mkdir build
fi

cd build

echo "配置CMake项目..."

# 使用pkg-config查找Qt5或指定Qt5路径
# 如果系统已安装Qt5，CMake会自动查找
# 如果需要指定路径，请取消注释下面的行并修改路径
# CMAKE_PREFIX_PATH="/usr/lib/x86_64-linux-gnu/qt5" 

cmake .. 

if [ $? -ne 0 ]; then
    echo "CMake配置失败！"
    echo "请确保已安装Qt5开发包："
    echo "  Ubuntu/Debian: sudo apt install qtbase5-dev qttools5-dev-tools"
    echo "  CentOS/RHEL:   sudo yum install qt5-qtbase-devel qt5-qttools-devel"
    echo "  Fedora:        sudo dnf install qt5-qtbase-devel qt5-qttools-devel"
    echo "  Arch:          sudo pacman -S qt5-base qt5-tools"
    exit 1
fi

echo "开始构建..."
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo "构建失败！"
    exit 1
fi

echo "构建完成！"
echo "可执行文件位置: ./QtApp"
