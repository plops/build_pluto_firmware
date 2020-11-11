# source /opt/Xilinx/Vivado/2019.1/settings64.sh
# export PATH=$PATH:/opt/Xilinx/SDK/2019.1/gnu/aarch32/lin/gcc-arm-linux-gnueabi/bin/
CROSS=arm-linux-gnueabihf-
CROSS_ROOT=/opt/Xilinx/SDK/2019.1/gnu/aarch32/lin/gcc-arm-none-eabi/arm-none-eabi/libc/
SDK=/home/martin/src/plutosdr-fw/buildroot/output/host/arm-buildroot-linux-gnueabihf/sysroot
CXX=${CROSS}g++
LD=${CROSS}ld

#-Og -g3 -ggdb

# model name      : ARMv7 Processor rev 0 (v7l)
# Features        : half thumb fastmult vfp edsp neon vfpv3 tls vfpd32 
# Hardware        : Xilinx Zynq Platform


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
-I. \
-D_GNU_SOURCE -D_DEFAULT_SOURCE
# ${GUI_INCLUDES} 
# -I${SDK}/usr/include 
# -I${CROSS_ROOT}/usr/include

LDFLAGS= \
-Wl,--gc-sections \
-Wl,--print-gc-sections \
--sysroot=${SDK} \
-L${SDK}/usr/lib \
-liio

#-lfftw3f -lm

#-lncursesw

#-L${SDK}/lib \
#-L${SDK}/usr/lib \
#-L${CROSS_ROOT}/lib \
#-L${CROSS_ROOT}/usr/lib 


all: vis

CFILES=$(shell ls *.cpp|grep ^vis|grep .cpp$)
OBJ:=$(CFILES:%.cpp=%.o)

%.o: %.cpp globals.h
	$(CXX) -c -o $@ $< $(CXXFLAGS)


vis: $(OBJ) 
	$(CXX) $^ -o $@ $(LDFLAGS)

clean:
	rm vis *.o


copy:
	scp vis root@pluto:/dev/shm
