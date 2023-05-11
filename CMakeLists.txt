cmake_minimum_required(VERSION 3.5)

project(ExtremeQualityImagingDemo VERSION 0.1 LANGUAGES CXX)
message(STATUS "Project Name: " ${PROJECT_NAME})
message(STATUS "current platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "current architecture: ${CMAKE_HOST_SYSTEM_PROCESSOR}")

#配置库文件路径
if(WIN32)
    set(THIRD_PARTY_LIB_PATH "E:\\runtime")
else()
    set(THIRD_PARTY_LIB_PATH "/home/mc/Projects/runtime")
endif()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(OpenCV_DIR "${THIRD_PARTY_LIB_PATH}/opencv")
    #ncnn
    set(ncnn_DIR "${THIRD_PARTY_LIB_PATH}/ncnn/x64/lib/cmake/ncnn")
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    #配置OpenCV路径
    set(OpenCV_DIR "${THIRD_PARTY_LIB_PATH}/opencv/lib/cmake/opencv4")
    set(ncnn_DIR "${THIRD_PARTY_LIB_PATH}/ncnn/lib/cmake/ncnn")
    #onnx
    set(OnnxRuntime_DIR "${THIRD_PARTY_LIB_PATH}/onnxruntime-linux-x64-gpu")
endif()

message(${OnnxRuntime_DIR})

#设置ToupCam_DIR和ToupCamConfig
set(ToupCam_DIR "${THIRD_PARTY_LIB_PATH}/toupcam")

#导入第三方库
find_package(OpenCV REQUIRED)
find_package(ToupCam REQUIRED)
find_package(ncnn REQUIRED)
find_package(OnnxRuntime REQUIRED)

include_directories("${ncnn_DIR}/include")


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Multimedia)


set(TS_FILES ${PROJECT_NAME}_zh_CN.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        utils.h utils.cpp
        cusbcamera.h cusbcamera.cpp
        ctoupcamera.h ctoupcamera.cpp
        appinit.cpp appinit.h
        appevent.cpp appevent.h
        connx.h connx.cpp
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${PROJECT_NAME}
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

#链接第三方库
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${ToupCam_LIBS})
target_link_libraries(${PROJECT_NAME} PRIVATE ncnn)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Multimedia)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OnnxRuntime_LIBS})


set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
