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

    # Open read end of the pipe
    read_fd = os.open("/tmp/my_pipe", os.O_RDONLY)

    # Read data from pipe
    size = 8 * 32 * 4  # Size of 8x32 integer array in bytes (assuming int is 4 bytes)
    data_bytes = os.read(read_fd, size)

    # Close read end of pipe
    os.close(read_fd)

    # Unpack binary data into a 2D list
    data = struct.unpack('256i', data_bytes)  # Assuming int is 4 bytes

    # Reshape data into a 2D list
    data = [data[i:i+32] for i in range(0, len(data), 32)]

    print("Received data:")
    for row in data:
        print(row)

    # Send acknowledgment message back to sender
    ack_message = b"Acknowledgment: Data received successfully"
    ack_pipe = os.open("/tmp/ack_pipe", os.O_WRONLY | os.O_NONBLOCK)
    os.write(ack_pipe, ack_message)
    os.close(ack_pipe)

    print("Acknowledgment sent.")

    while True:

        MY_CUSTOM_BITMAP_FONT = [
            [
                0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 
                0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00
            ]
        ]

        with canvas(device) as draw:
            # Note that "\0" is the zero-th character in the font (i.e the only one)
            legacy.text(draw, (0, 0), "\0", fill="white", font=MY_CUSTOM_BITMAP_FONT)

        time.sleep(5)

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        pass