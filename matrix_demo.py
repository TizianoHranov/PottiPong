import time
import argparse
import os
import struct

from luma.led_matrix.device import max7219
from luma.core.interface.serial import spi, noop
from luma.core.render import canvas
from luma.core.legacy import text, show_message
from luma.core import legacy

def main():
    # Create matrix device
    serial = spi(port=0, device=0, gpio=noop())
    device = max7219(serial, cascaded=4, block_orientation=90, blocks_arranged_in_reverse_order=True)

    fifo = 'my_fifo'

    # Create the named pipe (FIFO) if it doesn't exist
    if not os.path.exists(fifo):
        os.mkfifo(fifo)

    while True:
        # Open FIFO for reading
        try:
            with open(fifo, 'rb') as f:
                # Read data from FIFO
                data = f.read(256)  # Read 256 bytes (32x8 matrix)
                # Unpack 256 unsigned integers from the data buffer
                points = struct.unpack('256B', data)
                # Convert flat list to 2D array
                matrix = [[points[i + j*32] for i in range(32)] for j in range(8)]
                print(f"Received data: {matrix}")
        except FileNotFoundError:
            print(f"Error: FIFO {fifo} not found. Please ensure it is created.")
            break

        with canvas(device) as draw:
            # Draw the points on the LED matrix
            for y, row in enumerate(matrix):
                for x, value in enumerate(row):
                    if value != 0:
                        draw.point((x, y), fill="white")

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        pass
