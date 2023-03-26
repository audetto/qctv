cmake_minimum_required(VERSION 3.5)

project(qctv LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDK $ENV{HOME}/projects/cctv/sdk)
set(PLAYER $ENV{HOME}/projects/cctv/player)

set(SDK_INC ${SDK}/incEn)
set(SDK_LIB ${SDK}/lib)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(qctv
    sdk/hk_sdk.cpp
    sdk/hk_sdk.h
    sdk/hk_dvr.cpp
    sdk/hk_dvr.h
    sdk/hk_liveplayer.cpp
    sdk/hk_liveplayer.h
    sdk/hk_error.cpp
    sdk/hk_error.h
    sdk/hk_utils.h
    sdk/hk_playback.cpp
    sdk/hk_playback.h
    sdk/hk_callback.cpp
    sdk/hk_callback.h

    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    liveframe.cpp
    liveframe.h
    liveframe.ui
    utils.cpp
    utils.h
    receiver.cpp
    receiver.h

    playbacktime.cpp
    playbacktime.h

    playbackname.cpp
    playbackname.h

    playbackframe.cpp
    playbackframe.h
    playbackframe.ui

    downloadframe.cpp
    downloadframe.h
    downloadframe.ui

    abilityviewer.cpp
    abilityviewer.h
    abilityviewer.ui

    filebrowser.cpp
    filebrowser.h
    filebrowser.ui

    qctv.qrc
)

target_include_directories(qctv PRIVATE ${SDK_INC} ${PLAYER})

find_library(HCNETSDK NAMES hcnetsdk PATHS ${SDK_LIB})
find_library(PLAYCTRL NAMES PlayCtrl PATHS ${SDK_LIB})
find_library(AUDIORENDER NAMES AudioRender PATHS ${SDK_LIB})
find_library(SUPERRENDER NAMES SuperRender PATHS ${SDK_LIB})

target_link_libraries(qctv PRIVATE
  ${HCNETSDK}
  ${PLAYCTRL}
  )

target_link_libraries(qctv PRIVATE Qt::Widgets)

add_custom_command(TARGET qctv POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDK_LIB}/HCNetSDKCom ${CMAKE_BINARY_DIR}/HCNetSDKCom)

add_custom_command(TARGET qctv POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${AUDIORENDER} ${PLAYCTRL} ${SUPERRENDER} ${CMAKE_BINARY_DIR})
