# source /opt/Xilinx/Vivado/2018.3/settings64.sh
# export PATH=$PATH:/opt/Xilinx/SDK/2018.3/gnu/aarch32/lin/gcc-arm-linux-gnueabi/bin/
CROSS=arm-linux-gnueabihf-
CROSS_ROOT=/opt/Xilinx/SDK/2018.3/gnu/aarch32/lin/gcc-arm-none-eabi/arm-none-eabi/libc/
SDK=/home/martin/src/plutosdr-fw/buildroot/output/host/arm-buildroot-linux-gnueabihf/sysroot
CXX=${CROSS}g++
LD=${CROSS}ld

#-Og -g3 -ggdb 
CXXFLAGS= -O2 \
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
-I/mnt/chromium-master/
# -I${SDK}/usr/include 
# -I${CROSS_ROOT}/usr/include

LDFLAGS= \
-Wl,--gc-sections \
-Wl,--print-gc-sections \
--sysroot=${SDK} \
-L${SDK}/usr/lib \
-liio


#-L${SDK}/lib \
#-L${SDK}/usr/lib \
#-L${CROSS_ROOT}/lib \
#-L${CROSS_ROOT}/usr/lib 





all: vis

LIBMEMS_FILES=libmems/iio_channel_impl.cc  libmems/iio_context_impl.cc  libmems/iio_device.cc  libmems/iio_device_impl.cc   libmems/iio_device_trigger_impl.cc  

#$(shell ls libmems/*.cc | grep .cc$)
LIBMEMS_OBJ:=$(LIBMEMS_FILES:%.cc=%.o2)

CFILES=$(shell ls *.cpp| grep .cpp$)
OBJ:=$(CFILES:%.cpp=%.o)


%.o: %.cpp globals.h
	$(CXX) -c -o $@ $< $(CXXFLAGS)

%.o2: %.cc globals.h
	$(CXX) -c -o $@ $< $(CXXFLAGS)



vis: $(OBJ) $(LIBMEMS_OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)



clean:
	rm vis *.o libmems/*.o2


copy:
	scp vis root@pluto:/dev/shm
