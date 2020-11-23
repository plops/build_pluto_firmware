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
_code_git_version="091bf8c2f72043ae379fea20fd69bfb22a3dbc56"
_code_repository="https://github.com/plops/build_pluto_firmware/python/tree/master/python/source/run_00_start.py"
_code_generation_time="22:25:34 of Monday, 2020-11-23 (GMT+1)"
a=np.fromfile("/home/martin/stage/build_pluto_firmware/capture_simulator/source/o_4channels", np.int16)