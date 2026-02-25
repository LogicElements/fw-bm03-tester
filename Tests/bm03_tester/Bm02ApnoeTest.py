import logging
import csv
from datetime import *
from lecore.LeBin.RegisterMap import *
from lecore.LeBin.SerialCom import *
from time import sleep

from Bm03TesterRegs import Bm03TesterRegs as BM

# Chnage logger level just for errors
logger = logging.getLogger()
logger.setLevel(logging.ERROR)

# Find and open com port
com = SerialCom()
port = com.get_list_ports(pid=22336, sn="BM03TESTER")
com.open_com(port[0], 1)

# Create register map object
regs = RegisterMap(com)
regs.load('Bm03Tester_registers.json')


def iterate(breath=10.0, timeout=240):
    period = 60000.0 / breath

    # print(f"Start with {b} breaths per minute (breath every {round(period/1000, 2)} s.)")
    regs.write_by_name(BM.BM_SIGNAL_PERIOD, round(period))
    regs.write_by_name(BM.BM_ALARM_0, 0)
    regs.write_by_name(BM.BM_ALARM_1, 0)
    regs.write_by_name(BM.BM_POWER_0, 1)
    regs.write_by_name(BM.BM_POWER_1, 1)
    t_start = datetime.now()
    sleep(3)
    regs.write_by_name(BM.BM_SIGNAL_AMPLITUDE, 1000)

    results = [0.0] * 2
    run = True
    while run:
        if regs.read_by_name(BM.BM_ALARM_0) > 1:
            results[0] = (datetime.now() - t_start).total_seconds()
            regs.write_by_name(BM.BM_POWER_0, 0)
            regs.write_by_name(BM.BM_ALARM_0, 0)
        if regs.read_by_name(BM.BM_ALARM_1) > 1:
            results[1] = (datetime.now() - t_start).total_seconds()
            regs.write_by_name(BM.BM_POWER_1, 0)
            regs.write_by_name(BM.BM_ALARM_1, 0)

        if (datetime.now() - t_start).seconds > timeout:
            run = False

        if results[0] != 0.0 and results[1] != 0.0:
            run = False

        sleep(0.1)

    regs.write_by_name(BM.BM_SIGNAL_AMPLITUDE, 0)
    regs.write_by_name(BM.BM_POWER_0, 0)
    regs.write_by_name(BM.BM_POWER_1, 0)
    sleep(3)
    per = round(period / 1000, 2)
    print(f"Run with {b} breaths per minute (breath every {per} s.) with result time {results}")

    res = [b, per]
    res.extend(results)
    return res


# Run application
if __name__ == '__main__':

    # Setup
    regs.write_by_name(BM.BM_SIGNAL_BASE, 1250)
    regs.write_by_name(BM.BM_SIGNAL_AMPLITUDE, 0)
    regs.write_by_name(BM.BM_POWER_0, 0)
    regs.write_by_name(BM.BM_POWER_1, 0)
    sleep(3)

    breaths = [x * 0.1 for x in range(29, 101)]

    # breaths = [2.9, 3]

    csv_res = [['breath', 'period', 'time 1', 'time 2']]

    for b in breaths:
        csv_res.append(iterate(b))

    writer = csv.writer(open("output.csv", 'w', newline=''))

    writer.writerows(csv_res)

