unset LD_LIBRARY_PATH
export ADI_USE_OOC_SYNTHESIS=y
export CROSS_COMPILE=arm-linux-gnueabihf-
export PATH=$PATH:/opt/Xilinx/SDK/2019.1/gnu/aarch32/lin/gcc-arm-linux-gnueabi/bin/:/opt/Xilinx/Vivado/2019.1/bin/
export VIVADO_SETTINGS=/opt/Xilinx/Vivado/2019.1/settings64.sh
 
export VIVADO_VERSION=v2019.1

#git clone https://github.com/analogdevicesinc/hdl/
#git checkout    remotes/origin/hdl_2019_r1

# git clone https://github.com/analogdevicesinc/plutosdr-fw/
# git submodule update --init --recursive
