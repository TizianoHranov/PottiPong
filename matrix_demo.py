#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Copyright (c) 2017-18 Richard Hull and contributors
# See LICENSE.rst for details.

import re
import time
import argparse

from luma.led_matrix.device import max7219
from luma.core.interface.serial import spi, noop
from luma.core.render import canvas
from luma.core.virtual import viewport
from luma.core.legacy import text, show_message
from luma.core import legacy
from luma.core.legacy.font import proportional, CP437_FONT, TINY_FONT, SINCLAIR_FONT, LCD_FONT
import os
import struct


def main():
    # create matrix device
    serial = spi(port=0, device=0, gpio=noop())

    device = max7219(serial, cascaded=4, block_orientation=90, blocks_arranged_in_reverse_order=True)

    fifo = 'my_fifo'

    # Create the named pipe (FIFO)
    if not os.path.exists(fifo):
        os.mkfifo(fifo)

    while True:

        # Open FIFO for reading
        try:
            with open(fifo, 'rb') as f:
                data = f.read(32 * 4)  # Read 32 integers (each 4 bytes)
                flat_integers = struct.unpack('32i', data)
                # Convert flat list to 2D array
                integers = [flat_integers[i:i+32] for i in range(0, len(flat_integers), 32)]
                # Process the received data
                print(f"Received data: {integers}")
        except FileNotFoundError:
            print(f"Error: FIFO {fifo} not found. Please ensure it is created.")
            break

        MY_CUSTOM_BITMAP_FONT = integers

        with canvas(device) as draw:
            # Note that "\0" is the zero-th character in the font (i.e the only one)
            legacy.text(draw, (0, 0), "\0", fill="white", font=MY_CUSTOM_BITMAP_FONT)

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        pass