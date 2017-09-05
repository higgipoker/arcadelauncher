#cmake version
cmake_minimum_required (VERSION 2.6)

#project name
project (arcadelauncher)

#set environment variables
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_C_FLAGS_DEBUG "-DMY_DEBUG")

# --------------------------------------------------
# compiler flags
# --------------------------------------------------
add_definitions(-std=c++11)

# set include dirs
include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("/usr/local/include/SDL2")

#add files
add_executable(arcadelauncher
src/gui/selectiontheme.cpp
src/gui/coverflow.cpp
src/gui/staticgrid.cpp
src/gui/main_menu.cpp
src/video/videostreamer2.cpp
src/config/config.cpp
src/resources/resources.cpp
src/utils/timer.cpp
src/utils/gridtools.cpp
src/utils/point.cpp
src/utils/color.cpp
src/utils/ipaddress.cpp
src/files/configfile.cpp
src/files/file.cpp
src/main.cpp
src/sdl/window.cpp
src/sdl/text.cpp
src/sdl/sdl.cpp
src/sdl/renderable.cpp
src/sdl/image.cpp
src/widgets/widget.cpp
src/widgets/animations/expand.cpp
src/widgets/animations/slide.cpp
src/widgets/animations/pulse.cpp
src/widgets/animations/blink.cpp
src/widgets/animations/scale.cpp
src/widgets/animations/animation.cpp
src/widgets/genericicon.cpp
src/widgets/background.cpp
src/widgets/video.cpp
src/widgets/icon.cpp
src/widgets/screeneffect.cpp
src/widgets/textbar.cpp
src/input/input.cpp
src/input/joystick.cpp
src/input/keyboard.cpp
src/input/emulator.cpp

)

# link libs
target_link_libraries (arcadelauncher "SDL2main")
target_link_libraries (arcadelauncher "SDL2")
target_link_libraries (arcadelauncher "SDL2_image")
target_link_libraries (arcadelauncher "SDL2_ttf")
target_link_libraries (arcadelauncher "SDL2_mixer")
#target_link_libraries (arcadelauncher "sdl2_gfx")
target_link_libraries (arcadelauncher "avformat")
target_link_libraries (arcadelauncher "avcodec")
target_link_libraries (arcadelauncher "avutil")
target_link_libraries (arcadelauncher "swresample")
target_link_libraries (arcadelauncher "swscale")
target_link_libraries (arcadelauncher "pthread")
target_link_libraries (arcadelauncher "dl")
target_link_libraries (arcadelauncher "z")
#target_link_libraries (arcadelauncher "gamelib")
