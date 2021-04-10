cmake_minimum_required(VERSION 3.5)

project(qctv LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDK $ENV{HOME}/projects/cctv/sdk)

set(SDK_INC ${SDK}/incEn)
set(SDK_LIB ${SDK}/lib)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

add_executable(qctv
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    hk_sdk.cpp
    hk_sdk.h
    hk_dvr.cpp
    hk_dvr.h
    hk_play.cpp
    hk_play.h
    hk_error.cpp
    hk_error.h
    playframe.cpp
    playframe.h
    playframe.ui
    utils.cpp
    utils.h
)

target_include_directories(qctv PRIVATE ${SDK_INC})

find_library(HCNETSDK NAMES hcnetsdk PATHS ${SDK_LIB})
find_library(PLAYCTRL NAMES PlayCtrl PATHS ${SDK_LIB})
find_library(AUDIORENDER NAMES AudioRender PATHS ${SDK_LIB})
find_library(SUPERRENDER NAMES SuperRender PATHS ${SDK_LIB})

target_link_libraries(qctv PRIVATE
  ${HCNETSDK}
  )

target_link_libraries(qctv PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

add_custom_command(TARGET qctv POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDK_LIB}/HCNetSDKCom ${CMAKE_BINARY_DIR}/HCNetSDKCom)

add_custom_command(TARGET qctv POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${AUDIORENDER} ${PLAYCTRL} ${SUPERRENDER} ${CMAKE_BINARY_DIR})
