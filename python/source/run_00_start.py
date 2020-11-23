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
import scipy.signal
import numpy.fft
_code_git_version="4c6f52c49828e76b2664486e0bbd47700db93998"
_code_repository="https://github.com/plops/build_pluto_firmware/python/tree/master/python/source/run_00_start.py"
_code_generation_time="22:52:59 of Monday, 2020-11-23 (GMT+1)"
a=np.fromfile("/home/martin/stage/build_pluto_firmware/capture_simulator/source/o_4channels", np.int16)
b=(((((1.0    ))*(a[((32)+(0)):((2)*(23330)):2])))+(((1j)*((((1.0    ))*(a[((32)+(1)):((2)*(23330)):2]))))))
n=len(b)
q=(0.2446630    )
osc=np.exp(((-2j)*(np.pi)*(q)*(np.arange(n))))
bs=((b)*(osc))
sav_win=57
fbs=((scipy.signal.savgol_filter(np.real(bs), sav_win, 3, deriv=0))+(((1j)*(scipy.signal.savgol_filter(np.imag(bs), sav_win, 3, deriv=0)))))
dbs=((scipy.signal.savgol_filter(np.real(bs), sav_win, 3, deriv=1))+(((1j)*(scipy.signal.savgol_filter(np.imag(bs), sav_win, 3, deriv=1)))))
dphi=np.imag(((dbs)/(fbs)))
plt.plot(dphi)
plt.ylim(((-5.00e-2),(5.00e-2),))
plt.grid()