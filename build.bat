@echo off
echo 正在构建Qt CMake项目...

if not exist build (
    mkdir build
)

cd build

echo 配置CMake项目...
:: 使用Qt 5.15路径，请根据您的实际安装路径修改
cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/5.15.0/msvc2019_64"

if %ERRORLEVEL% neq 0 (
    echo CMake配置失败！请检查Qt5路径设置。
    echo 请确保已安装Qt 5.15并修改上面的路径。
    pause
    exit /b 1
)

echo 开始构建...
cmake --build . --config Release

if %ERRORLEVEL% neq 0 (
    echo 构建失败！
    pause
    exit /b 1
)

echo 构建完成！
echo 可执行文件位置: .\Release\QtApp.exe
pause
