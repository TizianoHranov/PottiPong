import os
import struct
from luma.led_matrix.device import max7219
from luma.core.interface.serial import spi, noop
from luma.core.render import canvas

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
                data = f.read(256)  # Read 256 bytes (32*8)
                # Unpack data into a 2D array
                points = struct.unpack('256B', data)
                points = [points[i:i+8] for i in range(0, len(points), 8)]
                print("Received data:")
                for row in points:
                    print(row)

                # Draw received points on LED matrix
                with canvas(device) as draw:
                    for x in range(len(points)):
                        for y in range(len(points[x])):
                            if points[x][y] != 0:
                                draw.point((x, y), fill="white")

        except FileNotFoundError:
            print(f"Error: FIFO {fifo} not found. Please ensure it is created.")
            break

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        pass