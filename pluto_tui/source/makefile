# source /opt/Xilinx/Vivado/2018.3/settings64.sh
# export PATH=$PATH:/opt/Xilinx/SDK/2018.3/gnu/aarch32/lin/gcc-arm-linux-gnueabi/bin/
CROSS=arm-linux-gnueabihf-
CROSS_ROOT=/opt/Xilinx/SDK/2018.3/gnu/aarch32/lin/gcc-arm-none-eabi/arm-none-eabi/libc/
SDK=/home/martin/src/plutosdr-fw/buildroot/output/host/arm-buildroot-linux-gnueabihf/sysroot
CXX=${CROSS}g++
LD=${CROSS}ld

#-Og -g3 -ggdb

# model name      : ARMv7 Processor rev 0 (v7l)
# Features        : half thumb fastmult vfp edsp neon vfpv3 tls vfpd32 
# Hardware        : Xilinx Zynq Platform
GUI_INCLUDES=-I/home/martin/src/imtui/examples/ncurses0/.. -I/home/martin/src/imtui/src/../include -I/home/martin/src/imtui/third-party/imgui 

CXXFLAGS=-fomit-frame-pointer -fno-common -fno-builtin -mfloat-abi=hard\
-mcpu=cortex-a9 -march=armv7-a -mtune=cortex-a9 -mthumb-interwork -mtune=cortex-a9 \
-O3 -ffast-math -funsafe-math-optimizations -mfpu=neon-vfpv3 \
-std=gnu++17 \
-Wall -Wextra -Werror -Wfatal-errors\
-ffunction-sections -fdata-sections \
-Wno-error=unused-variable \
-Wno-error=unused-parameter \
-Wno-error=double-promotion \
-Wno-error=unused-parameter \
-Wno-error=shift-negative-value \
-Wno-error=attributes \
-Wno-error=sign-compare \
-pipe \
--sysroot=${SDK} \
${GUI_INCLUDES} \
-I. 
# -I${SDK}/usr/include 
# -I${CROSS_ROOT}/usr/include

LDFLAGS= \
-Wl,--gc-sections \
-Wl,--print-gc-sections \
--sysroot=${SDK} \
-L${SDK}/usr/lib \
-liio -lfftw3f -lm

#-L${SDK}/lib \
#-L${SDK}/usr/lib \
#-L${CROSS_ROOT}/lib \
#-L${CROSS_ROOT}/usr/lib 





all: vis

#LIBMEMS_FILES=libmems/iio_channel_impl.cc  libmems/iio_context_impl.cc  libmems/iio_device.cc  libmems/iio_device_impl.cc   libmems/iio_device_trigger_impl.cc  

#$(shell ls libmems/*.cc | grep .cc$)
#LIBMEMS_OBJ:=$(LIBMEMS_FILES:%.cc=%.o2)

CFILES=$(shell ls *.cpp| grep .cpp$)
OBJ:=$(CFILES:%.cpp=%.o)

GUI_FILES=third-party/imgui/imgui/imgui.cpp \
third-party/imgui/imgui/imgui_draw.cpp \
third-party/imgui/imgui/imgui_demo.cpp \
third-party/imgui/imgui/imgui_widgets.cpp \
src/imtui-impl-text.cpp \
src/imtui-impl-ncurses.cpp \
examples/imtui-demo.cpp 


%.o: %.cpp globals.h
	$(CXX) -c -o $@ $< $(CXXFLAGS)

%.o2: %.cc globals.h
	$(CXX) -c -o $@ $< $(CXXFLAGS)



vis: $(OBJ) #$(LIBMEMS_OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)



clean:
	rm vis *.o libmems/*.o2


copy:
	scp vis root@pluto:/dev/shm
