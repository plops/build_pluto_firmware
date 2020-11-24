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
_code_git_version="8ba119a129405ce06818d35cab375850274bace6"
_code_repository="https://github.com/plops/build_pluto_firmware/python/tree/master/python/source/run_00_start.py"
_code_generation_time="19:31:25 of Tuesday, 2020-11-24 (GMT+1)"
a=np.fromfile("/home/martin/stage/build_pluto_firmware/capture_simulator/source/o_4channels", np.int16)
ss=12
b=(((((1.0    ))*(a[((32)+(0)):((ss)*(2)*(23330)):2])))+(((1j)*((((1.0    ))*(a[((32)+(1)):((ss)*(2)*(23330)):2]))))))
n=len(b)
q=(0.2446630    )
osc=np.exp(((-2j)*(np.pi)*(q)*(np.arange(n))))
bs=((b)*(osc))
# use gaussian
sigma=(3.20    )
fbs=((scipy.ndimage.gaussian_filter(np.real(bs), sigma))+(((1j)*(scipy.ndimage.gaussian_filter(np.imag(bs), sigma)))))
dbs=np.gradient(fbs)
dphi=np.imag(((dbs)/(fbs)))
# select template
t0=dphi[14000:22000]
cc=scipy.signal.correlate(t0, dphi)
plt.plot(cc)
plt.grid()