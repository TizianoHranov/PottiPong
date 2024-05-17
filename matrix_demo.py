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
                # Read data from FIFO
                data = f.read(256)  # Read 256 bytes (8x32 matrix)
                # Unpack 256 unsigned bytes from the data buffer
                integers = struct.unpack('256B', data)
                # Convert flat list to 2D array (8x32)
                matrix = [integers[i:i+32] for i in range(0, len(integers), 32)]
                print(f"Received data: {matrix}")
        except FileNotFoundError:
            print(f"Error: FIFO {fifo} not found. Please ensure it is created.")
            break

        MY_CUSTOM_BITMAP_FONT = integers

        with canvas(device) as draw:
            #for y, row in enumerate(matrix):
                #for x, value in enumerate(row):
                    #if value:
                        #draw.point((x, y), fill="white")
            draw.point((16, 0), fill="white")
            draw.point((31, 1), fill="white")
            draw.point((0, 7), fill="white")
            draw.point((0, 5), fill="white")

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        pass