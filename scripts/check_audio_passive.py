#!/usr/bin/env python3
"""Rev.B受動音声経路の小信号計算。実測・SPICE・ERCの代替ではない。"""
from math import pi, log10

def gain(hz):
    w = 2j * pi * hz
    # VMIDを交流的に理想GNDと近似。SLICの公称入出力抵抗を使用。
    z_input = 60000 + 1 / (w * 10e-9)
    z_to = 1 / (1 / 100000 + 1 / z_input)
    z_bias = 1 / (1 / 47000 + 1 / (70 + z_to))
    z_source = 10 + 22000 + 1 / (w * 100e-9)
    return z_bias / (z_source + z_bias) * z_to / (70 + z_to) * 60000 / z_input

if __name__ == '__main__':
    print('周波数,利得dB（公称値・VMID理想近似）')
    for hz in (300, 400, 1000, 3400):
        print(f'{hz},{20 * log10(abs(gain(hz))):.2f}')
