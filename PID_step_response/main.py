import random
import time

import control
import numpy as np
import serial
from matplotlib import pyplot as plt
import keyboard
import math

ser = serial.Serial(port='COM3', baudrate=500000)

nb_pixel_h = 64
nb_pixel_w = 128

global pixel_array
pixel_array = np.full((nb_pixel_h, nb_pixel_w), False, dtype=bool)


def setPixel(i, j):
    if nb_pixel_h > i >= -1 and nb_pixel_w > j >= 0:
        pixel_array[i][j] = True


def clearPixel(i, j):
    if nb_pixel_h > i >= -1 and nb_pixel_w > j >= 0:
        pixel_array[i][j] = False


def toHex(i, j):
    sum = 0
    add = 128
    for k in range(0, 8):
        if pixel_array[i][j + k]:
            sum += add
        add /= 2
    return sum


def send_pixel_array():
    six9 = bytearray()
    six9.append(int(69))
    ser.write(six9)
    six99 = bytearray()
    six99.append(int(69))
    ser.write(six99)

    for h in range(0, nb_pixel_h):
        for w in range(0, int(nb_pixel_w / 8)):
            packet = bytearray()
            packet.append(int(toHex(h, 8 * w)))
            ser.write(packet)

Ka_n = 2;
Rm_n = 2.6;
Km_n = 0.00767;
Kg_n = 70;
Jmot_n = 3.87e-7;
Ks_n = 1.2;
b_n = 0.004;
Jmod_n = 3.944e-4;
Jbr_n = 0.0037;

Jm_n = Jmod_n + Kg_n * Kg_n * Jmot_n;
s = control.TransferFunction.s
G = (Ks_n * Kg_n * Km_n * Ka_n) / ((Jbr_n * s * s + Ks_n) * (
        Rm_n * Jm_n * s * s + Rm_n * b_n * s + Km_n * Km_n * Kg_n * Kg_n * s) + Rm_n * Ks_n * Jbr_n * s * s)

plt.figure()
control.bode_plot(G)
plt.show()

Kp = 1.7433
Ti = 0.1755
Td = 0.0439
Kp_delta = 0
Ti_delta = 0
Td_delta = 0


while True:
    Dc = (Kp + 0.5 * int(Kp_delta)) + (Ti + 0.5 * Ti_delta) / s + (Td + 0.1 * Td_delta) * s

    t, y = control.step_response(G / (1 + G * Dc), 2)
    maximum = max(y) + 1
    for i in range(0, len(t)):
        setPixel(nb_pixel_h - 1 - math.floor(y[i] * nb_pixel_h / maximum * 2), math.floor(i / len(t) * nb_pixel_w))

    send_pixel_array()
    time.sleep(0.5)
    while ser.inWaiting() <= 0:
        pass
    Kp_delta = ord(ser.read(1))
    if Kp_delta > 127:
        Kp_delta -= 255
    Ti_delta = ord(ser.read(1))
    if Ti_delta > 127:
        Ti_delta -= 255
    Td_delta = ord(ser.read(1))
    if Td_delta > 127:
        Td_delta -= 255

    print("Kp: ", Kp + 0.1 * Kp_delta)
    print("Ti: ", Ti + 0.1 * Ti_delta)
    print("Td: ", Td + 0.01 * Td_delta)
    print()

    pixel_array = np.full((nb_pixel_h, nb_pixel_w), False, dtype=bool)
