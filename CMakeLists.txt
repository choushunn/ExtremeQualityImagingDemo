cmake_minimum_required(VERSION 3.5)

project(ExtremeQualityImagingDemo VERSION 0.1 LANGUAGES CXX)
message(STATUS "Project Name: " ${PROJECT_NAME})
message(STATUS "current platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "current architecture: ${CMAKE_HOST_SYSTEM_PROCESSOR}")

set(CMAKE_PREFIX_PATH "D:\\Programs\\Qt\\6.2.4\\msvc2019_64")


#配置库文件路径
if (WIN32)
    set(THIRD_PARTY_LIB_PATH "F:\\share\\runtime")
else ()
    set(THIRD_PARTY_LIB_PATH "/root/runtime")
endif ()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(OpenCV_DIR "${THIRD_PARTY_LIB_PATH}/opencv")
    #ncnn
    set(ncnn_DIR "${THIRD_PARTY_LIB_PATH}/ncnn/x64/lib/cmake/ncnn")
    #onnx
    set(OnnxRuntime_DIR "${THIRD_PARTY_LIB_PATH}/onnxruntime")
    #设置ToupCam_DIR和ToupCamConfig
    set(ToupCam_DIR "${THIRD_PARTY_LIB_PATH}/toupcam/win")
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
    #配置OpenCV路径
    set(OpenCV_DIR "${THIRD_PARTY_LIB_PATH}/opencv/lib/cmake/opencv4")
    set(ncnn_DIR "${THIRD_PARTY_LIB_PATH}/ncnn/lib/cmake/ncnn")
    #onnx
    set(OnnxRuntime_DIR "${THIRD_PARTY_LIB_PATH}/onnxruntime-linux-x64-gpu")
endif ()

message(${OnnxRuntime_DIR})


#导入第三方库
find_package(OpenCV REQUIRED)
find_package(ToupCam REQUIRED)
find_package(ncnn REQUIRED)
find_package(OnnxRuntime REQUIRED)

include_directories("${ncnn_DIR}/include")


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Multimedia)


# 设置头文件目录为include文件夹
include_directories(include)

# 设置自动生成的ui头文件的输出路径为include目录
set(CMAKE_AUTOUIC_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include)
# 设置ui文件搜索路径
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

#匹配寻找头文件
FILE(GLOB_RECURSE HEADER_FILES "${PROJECT_SOURCE_DIR}/include/*.h")
#匹配寻找源文件
FILE(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
#匹配寻找UI文件
FILE(GLOB_RECURSE UI_FILES "${PROJECT_SOURCE_DIR}/ui/*.ui")
set(TS_FILES "translations/${PROJECT_NAME}_zh_CN.ts")

set(PROJECT_SOURCES
        ${HEADER_FILES}
        ${SRC_FILES}
        ${TS_FILES}
        ${UI_FILES}
        )

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            )
    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else ()
    if (ANDROID)
        add_library(${PROJECT_NAME} SHARED
                ${PROJECT_SOURCES}
                )
        # Define properties for Android with Qt 5 after find_package() calls as:
        #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else ()
        add_executable(${PROJECT_NAME}
                ${PROJECT_SOURCES}
                )
    endif ()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif ()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Multimedia)

#链接第三方库
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${TOUPCAM_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE ncnn)
target_link_libraries(${PROJECT_NAME} PRIVATE ${ONNXRUNTIME_LIBS})


set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )


# message("${CMAKE_INSTALL_LIBDIR}")
# install target and libraries
install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include
        )

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif ()
