cmake_minimum_required( VERSION 3.4 )
project( vis LANGUAGES CXX )
set( CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++ )
set( CMAKE_VERBOSE_MAKEFILE ON )
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-common -fno-builtin -mfloat-abi=hard -mcpu=cortex-a9 -march=armv7-a -mtune=cortex-a9 -mthumb-interwork -mtune=cortex-a9 -O0 -ffast-math -funsafe-math-optimizations -mfpu=neon-vfpv3  -Wall -Wextra -Werror -Wfatal-errors -ffunction-sections -fdata-sections -Wno-error=unused-variable -Wno-error=unused-parameter -Wno-error=double-promotion -Wno-error=unused-parameter -Wno-error=shift-negative-value -Wno-error=attributes -Wno-error=sign-compare --sysroot=/home/martin/pluto-0.35.sysroot/ -I. -D_GNU_SOURCE -D_DEFAULT_SOURCE ")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections --sysroot=/home/martin/pluto-0.35.sysroot/ -L/home/martin/pluto-0.35.sysroot//usr/lib -liio -lpthread ")
set( CMAKE_CXX_STANDARD 17 )
set( SRCS /home/martin/stage/build_pluto_firmware/capture2/source/vis_00_base.cpp
/home/martin/stage/build_pluto_firmware/capture2/source/vis_01_server.cpp
/home/martin/stage/build_pluto_firmware/capture2/source/vis_02_tsqueue.cpp )
add_executable( vis ${SRCS} )
target_compile_features( vis PUBLIC cxx_std_17 )
