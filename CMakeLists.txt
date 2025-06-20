cmake_minimum_required(VERSION 3.16...3.22)

project(QtApp
    VERSION 1.0.0
    DESCRIPTION "Qt Application with CMake"
    LANGUAGES CXX
)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置Qt6安装路径（请根据你的实际Qt6安装路径修改）
set(CMAKE_PREFIX_PATH "D:/Qt/6.9.0/msvc2022_64" ${CMAKE_PREFIX_PATH})

# 查找Qt6
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
    Gui
)

# 启用Qt的MOC、UIC和RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 源文件
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    src/addresseditwidget.cpp
    src/tagtextobject.cpp
    src/tagtextobject.h
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 链接Qt库
target_link_libraries(${PROJECT_NAME} 
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

# 设置目标属性
set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# 为Windows设置Qt的DLL搜索路径
if(WIN32)
    find_program(QT_WINDEPLOYQT_EXECUTABLE windeployqt HINTS ${Qt6_DIR}/../../../bin)
    
    if(QT_WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${QT_WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${PROJECT_NAME}>
            COMMENT "Deploying Qt libraries"
        )
    endif()
endif()
