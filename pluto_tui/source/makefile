# export PATH=$PATH:/opt/Xilinx/SDK/2018.3/gnu/aarch32/lin/gcc-arm-linux-gnueabi/bin/
CROSS=arm-linux-gnueabihf-
CROSS_ROOT=/opt/Xilinx/SDK/2018.3/gnu/aarch32/lin/gcc-arm-none-eabi/arm-none-eabi/libc/
SDK=/home/martin/src/plutosdr-fw/buildroot/output/host/arm-buildroot-linux-gnueabihf/sysroot
CXX=${CROSS}g++
LD=${CROSS}ld

CXXFLAGS=-Og -g3 -ggdb \
-std=gnu++17 \
-Wall -Wextra -Werror -Wfatal-errors\
-ffunction-sections -fdata-sections \
-Wno-error=unused-variable \
-Wno-error=unused-parameter \
-Wno-error=double-promotion \
-pipe \
-I${SDK}/usr/include \
-I${CROSS_ROOT}/usr/include

LDFLAGS= \
-L${SDK}/lib \
-L${SDK}/usr/lib \
-L${CROSS_ROOT}/lib \
-L${CROSS_ROOT}/usr/lib \
-Wl,--gc-sections \
-Wl,--print-gc-sections 





all: vis

CFILES=$(shell find *.cpp| grep .cpp$)
OBJ:=$(CFILES:%.cpp=%.o)

%.o: %.cpp globals.h
	$(CXX) -c -o $@ $< $(CXXFLAGS)



vis: $(OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)



clean:
	rm vis *.o
