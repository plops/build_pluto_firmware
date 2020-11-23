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
_code_git_version="59e1717c105f66c5131577cb8103d945cd1ea0c0"
_code_repository="https://github.com/plops/build_pluto_firmware/python/tree/master/python/source/run_00_start.py"
_code_generation_time="22:28:11 of Monday, 2020-11-23 (GMT+1)"
a=np.fromfile("/home/martin/stage/build_pluto_firmware/capture_simulator/source/o_4channels", np.int16)
b=(((((1.0    ))*(a[0:40000:2])))+(((1j)*((((1.0    ))*(a[1:40000:2]))))))
plt.plot(np.abs(b))