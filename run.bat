@echo off
echo 正在运行Qt应用程序...

if exist "build\Release\QtApp.exe" (
    cd build\Release
    QtApp.exe
) else if exist "build\Debug\QtApp.exe" (
    cd build\Debug
    QtApp.exe
) else (
    echo 找不到可执行文件！请先运行build.bat构建项目。
    pause
)
