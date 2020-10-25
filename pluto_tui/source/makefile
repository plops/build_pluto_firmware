# export PATH=$PATH:/opt/Xilinx/SDK/2018.3/gnu/aarch32/lin/gcc-arm-linux-gnueabi/bin/
CROSS=arm-linux-gnueabihf-
CXX=${CROSS}g++
LD=${CROSS}ld

CXXFLAGS=-Og -g3 -ggdb \
-std=gnu++17 \
-Wall -Wextra -Werror -Wfatal-errors\
-ffunction-sections -fdata-sections \
-Wno-error=unused-variable \
-Wno-error=unused-parameter \
-Wno-error=double-promotion \
-pipe

LDFLAGS= \
-L/usr/local/lib \
-Wl,--gc-sections \
-Wl,--print-gc-sections \
-lpthread





all: vis

CFILES=$(shell find simple*.| grep .cpp$)
OBJ:=$(CFILES:%.cpp=%.o)

%.o: %.cpp globals.h
	$(CXX) -c -o $@ $< $(CXXFLAGS)



vis: $(OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)



clean:
	rm vis *.o
