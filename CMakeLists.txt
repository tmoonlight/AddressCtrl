cmake_minimum_required(VERSION 3.16)

project(QtApp
    VERSION 1.0.0
    DESCRIPTION "Qt Application with CMake"
    LANGUAGES CXX
)

# 设置C++标准（Qt 5.15支持C++14和C++17）
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置Qt5安装路径（请根据你的实际Qt5安装路径修改）
# 注释掉硬编码路径，让CMake自动查找Qt5
# set(CMAKE_PREFIX_PATH "D:/Qt/5.15.0/msvc2022_64" ${CMAKE_PREFIX_PATH})

# 查找Qt5
find_package(Qt5 REQUIRED COMPONENTS
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
    src/tagtextobject.cpp
    src/tagtextobject.h
    src/richtexteditorwidget.cpp
    src/richtexteditorwidget.h
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 链接Qt5库
target_link_libraries(${PROJECT_NAME} 
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
)

# 设置目标属性
set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# 为Windows设置Qt的DLL搜索路径
if(WIN32)
    # 查找Qt5的windeployqt工具
    get_target_property(QT5_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(QT5_WINDEPLOYQT_EXECUTABLE ${QT5_QMAKE_EXECUTABLE} PATH)
    set(QT5_WINDEPLOYQT_EXECUTABLE "${QT5_WINDEPLOYQT_EXECUTABLE}/windeployqt.exe")
    
    if(EXISTS ${QT5_WINDEPLOYQT_EXECUTABLE})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${QT5_WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${PROJECT_NAME}>
            COMMENT "Deploying Qt5 libraries"
        )
    endif()
endif()
