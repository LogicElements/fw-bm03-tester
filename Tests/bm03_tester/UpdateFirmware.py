import glob
from time import sleep

from Bm03Tester import *

from Bm03TesterRegs import Bm03TesterRegs as Reg

dev = Bm03Tester()

ports = dev.search_com()
dev.open(comport=ports[0])

path = ["../../Firmware/bm03_tester_app/Debug/"]

firmware_bin_name = 'bm03_tester_app-*.bin'


# Check all paths and try to find given file
for p in path:
    files = glob.glob(p + firmware_bin_name)
    if files:
        files.sort()
        if len(files) >= 1:
            dev.upgrade_firmware(files[-1])



