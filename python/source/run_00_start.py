# %% imports
import matplotlib
import matplotlib.pyplot as plt
import matplotlib.animation as animation
plt.ion()
import sys
import time
import pathlib
import numpy as np
import pandas as pd
import xarray as xr
import xarray.plot as xrp
import scipy.ndimage
import scipy.optimize
import numpy.fft
_code_git_version="bd07a58841d97b68a728abb64929ffaacf517ea4"
_code_repository="https://github.com/plops/build_pluto_firmware/python/tree/master/python/source/run_00_start.py"
_code_generation_time="22:42:03 of Monday, 2020-11-23 (GMT+1)"
a=np.fromfile("/home/martin/stage/build_pluto_firmware/capture_simulator/source/o_4channels", np.int16)
b=(((((1.0    ))*(a[((32)+(0)):((2)*(23330)):2])))+(((1j)*((((1.0    ))*(a[((32)+(1)):((2)*(23330)):2]))))))
n=len(b)
q=(0.2446630    )
osc=np.exp(((-2j)*(np.pi)*(q)*(np.arange(n))))
bs=((b)*(osc))
plt.semilogy(np.fft.fftfreq(n), np.abs(np.fft.fft(bs)))
plt.grid()