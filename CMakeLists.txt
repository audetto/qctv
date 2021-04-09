cmake_minimum_required(VERSION 3.5)

project(qctv LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDK "$ENV{HOME}/projects/cctv/sdk")
set(PLAYER "$ENV{HOME}/projects/cctv/player")

set(SDK_INC "${SDK}/incEn")
set(SDK_LIB "${SDK}/lib")
set(SDK_COM "${SDK_LIB}/HCNetSDKCom")
set(PLAYER_LIB "${PLAYER}")

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
)

target_include_directories(qctv PRIVATE ${SDK}/incEn)
find_library(HCNETSDK NAMES hcnetsdk PATHS ${SDK_LIB})
target_link_libraries(qctv PRIVATE
  ${HCNETSDK}
  )

target_link_libraries(qctv PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

add_custom_command(TARGET qctv POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDK_COM} "${CMAKE_BINARY_DIR}/HCNetSDKCom")

add_custom_command(TARGET qctv POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${PLAYER_LIB}/*.so "${CMAKE_BINARY_DIR}")
