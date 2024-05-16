#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
    // Define your 8x32 integer array
    int data[8][32] = {
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32},
        {33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63},
        // Add remaining rows as needed
    };

    // Calculate the size of the array in bytes
    size_t size = sizeof(data);

    // Open the named pipe for writing
    int pipe_fd = open("/tmp/my_pipe", O_WRONLY);
    if (pipe_fd == -1) {
        perror("open");
        return 1;
    }

    // Write data to the pipe
    if (write(pipe_fd, data, size) != size) {
        perror("write");
        return 1;
    }

    // Close the pipe
    close(pipe_fd);

    // Open the acknowledgment pipe for reading
    int ack_fd = open("/tmp/ack_pipe", O_RDONLY);
    if (ack_fd == -1) {
        perror("open");
        return 1;
    }

    // Read acknowledgment from the pipe (loop until successful)
    char ack_message[100];
    ssize_t bytes_read;
    while ((bytes_read = read(ack_fd, ack_message, sizeof(ack_message))) <= 0) {
        // Handle read error or empty acknowledgment message
        if (bytes_read == -1) {
            perror("read");
            close(ack_fd);
            return 1;
        }
        // Wait a short interval before attempting to read again
        usleep(100000);  // 100 milliseconds
    }

    // Close the acknowledgment pipe
    close(ack_fd);

    // Print acknowledgment message
    printf("Acknowledgment received: %s\n", ack_message);

    return 0;
}