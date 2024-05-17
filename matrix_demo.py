import os
import struct
import time
from luma.led_matrix.device import max7219
from luma.core.interface.serial import spi, noop
from luma.core.render import canvas
from luma.core import legacy

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
                data = f.read(32)  # Read 32 bytes (1x32 matrix)
                # Unpack 32 unsigned integers from the data buffer
                integers = struct.unpack('32B', data)
                # Convert flat list to 2D array
                print(f"Received data: {integers}")

                # Draw received data on LED matrix
                with canvas(device) as draw:
                    for x, value in enumerate(integers):
                        draw.point((x, value), fill="white")

                draw.point((0, 0xff), fill="white")
                draw.point((1, 0x01), fill="white")
                draw.point((2, 0xf3), fill="white")
                draw.point((3, 0x08), fill="white")
                draw.point((4, 0x10), fill="white")
                draw.point((5, 0x13), fill="white")
                draw.point((6, 0x18), fill="white")
                draw.point((7, 0x00), fill="white")
                draw.point((8, 0x00), fill="white")
                draw.point((8, 0xff), fill="white")
                
        except FileNotFoundError:
            print(f"Error: FIFO {fifo} not found. Please ensure it is created.")
            break

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        pass