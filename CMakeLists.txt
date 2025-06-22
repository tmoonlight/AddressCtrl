cmake_minimum_required(VERSION 3.16)

# Qt5/Qt6兼容项目配置
# 使用方法：
# 1. 对于Qt6: cmake -S . -B build
# 2. 对于Qt5: 如果系统中Qt6和Qt5共存，可以指定Qt5路径
#             cmake -S . -B build -DCMAKE_PREFIX_PATH="path/to/qt5"
# 3. 构建: cmake --build build --config Release

project(QtApp
    VERSION 1.0.0
    DESCRIPTION "Qt Application with CMake - Compatible with Qt5 and Qt6"
    LANGUAGES CXX
)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 允许用户指定Qt版本
option(FORCE_QT5 "Force use Qt5 even if Qt6 is available" OFF)

# 启用Qt的MOC、UIC和RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 优先查找Qt6，如果找不到则查找Qt5
if(NOT FORCE_QT5)
    find_package(Qt6 QUIET COMPONENTS Core Widgets Gui)
endif()

if(Qt6_FOUND AND NOT FORCE_QT5)
    message(STATUS "Found Qt6: ${Qt6_VERSION}")
    set(QT_VERSION_MAJOR 6)
    set(QT_LIBS Qt6::Core Qt6::Widgets Qt6::Gui)
    set(QT_QMAKE_TARGET Qt6::qmake)
else()
    find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)
    message(STATUS "Found Qt5: ${Qt5_VERSION}")
    set(QT_VERSION_MAJOR 5)
    set(QT_LIBS Qt5::Core Qt5::Widgets Qt5::Gui)
    set(QT_QMAKE_TARGET Qt5::qmake)
endif()

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

# 链接Qt库（兼容Qt5和Qt6）
target_link_libraries(${PROJECT_NAME} ${QT_LIBS})

# 输出使用的Qt版本信息
message(STATUS "Using Qt${QT_VERSION_MAJOR}")
message(STATUS "Qt libraries: ${QT_LIBS}")

# Qt6特定的配置
if(QT_VERSION_MAJOR EQUAL 6)
    # Qt6可能需要额外的编译器特性
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
endif()

# 设置目标属性
set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# 为Windows设置Qt的DLL搜索路径
if(WIN32)
    # 查找Qt的windeployqt工具（兼容Qt5和Qt6）
    get_target_property(QT_QMAKE_EXECUTABLE ${QT_QMAKE_TARGET} IMPORTED_LOCATION)
    get_filename_component(QT_WINDEPLOYQT_EXECUTABLE ${QT_QMAKE_EXECUTABLE} PATH)
    set(QT_WINDEPLOYQT_EXECUTABLE "${QT_WINDEPLOYQT_EXECUTABLE}/windeployqt.exe")
    
    if(EXISTS ${QT_WINDEPLOYQT_EXECUTABLE})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${QT_WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${PROJECT_NAME}>
            COMMENT "Deploying Qt${QT_VERSION_MAJOR} libraries"
        )
    else()
        message(WARNING "windeployqt not found. You may need to manually deploy Qt libraries.")
    endif()
endif()
