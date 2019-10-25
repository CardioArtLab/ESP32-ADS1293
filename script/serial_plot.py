'''
This python script listens on UDP port 3333
for messages from the ESP32 board and prints them
'''
import serial
from serial.tools.list_ports import comports
import serial.threaded

import threading
import queue

from statistics import median
from math import sqrt
from scipy.io import savemat

import matplotlib.pyplot as plt
import matplotlib.style as mplstyle
from scipy.signal import savgol_filter, butter, sosfilt,cheby2

# fast and light-weight plotting style
# https://matplotlib.org/tutorials/introductory/usage.html#sphx-glr-tutorials-introductory-usage-py
mplstyle.use(['dark_background', 'fast'])

shutdownEvent = threading.Event()
ch1_data = []
#ch2_data = []

MAX_COUNT = 2000
MAX_TIMEOUT = 60 

def convert(input_value, num_bits):
    '''Calculates a two's complement integer from the given input value's bits'''
    # mask = 2**(num_bits - 1)
    # input_value = -(input_value & mask) + (input_value & ~mask)
    return (input_value/0xc35000 - 0.5)*48000/35

class Reader(serial.threaded.LineReader):
    TERMINATOR = b'\n'
    count = 0
    def connection_made(self, transport):
        super(Reader, self).connection_made(transport)
        print('Port Open')
    def handle_packet(self, packet):
        global ch1_data #, ch2_data
        if len(packet) == 3:
            # ch1 = twos_complement((packet[0] << 24) | (packet[1] << 16) | (packet[2] << 8) | packet[3], 32)
            # ch2 = twos_complement((packet[4] << 24) | (packet[5] << 16) | (packet[6] << 8) | packet[7], 32)
            ch1 = convert((packet[0] << 16) | (packet[1] << 8) | packet[2], 24)
            # ch2 = convert((packet[3] << 16) | (packet[4] << 8) | packet[5], 24)
            ch1_data.append(ch1)
            # ch2_data.append(ch2)
        # Increase counter variables
        self.count += 1
        if (self.count % 1000) == 999:
            print(self.count+1)
        if (self.count >= MAX_COUNT):
            shutdownEvent.set()
    def connection_lost(self, exc):
        if exc:
            print(exc)
        shutdownEvent.set()

if __name__ == "__main__":
    device = None
    for c in comports(True):
        device = c.device
        print('Found {} ({})'.format(c.device, c.manufacturer))
        break
    with serial.Serial(device, 256000, timeout=1) as ser:
        with serial.threaded.ReaderThread(ser, Reader) as protocol:
            try:
                shutdownEvent.wait(MAX_TIMEOUT)
            except KeyboardInterrupt:
                pass

    #savemat('dataset.mat', {'ch1': ch1_data})
    sos = cheby2(20, 20, 45, 'lp', fs=1280, output='sos')
    # ch1 = savgol_filter(ch1_data, 7, 1)
    ch1 = sosfilt(sos, ch1_data)
    plt.plot(ch1_data, '.r', label='lead I (Raw)', markersize=2.3, linewidth=0.5)
    plt.plot(ch1, 'g', label='lead I (Filtered)', markersize=0.5, linewidth=1.5)
    plt.legend()
    plt.xlabel('sample')
    plt.ylabel('mV')
    plt.show()