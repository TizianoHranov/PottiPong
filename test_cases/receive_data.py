import os
import struct

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