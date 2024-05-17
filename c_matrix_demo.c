#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    char *fifo = "my_fifo";
    int data[32];
    char ack[10];

    // Initialize data with some values
    for (int i = 0; i < 32; i++) {
        data[i] = i;  // Example pattern
    }

    while (1) {
        // Open FIFO for writing
        int fd = open(fifo, O_WRONLY);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        // Write data to FIFO
        if (write(fd, data, sizeof(data)) == -1) {
            perror("write");
            close(fd);
            exit(EXIT_FAILURE);
        }
        close(fd);

        // Open FIFO for reading acknowledgment
        fd = open(fifo, O_RDONLY);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        // Read acknowledgment from FIFO
        if (read(fd, ack, sizeof(ack)) == -1) {
            perror("read");
            close(fd);
            exit(EXIT_FAILURE);
        }
        close(fd);

        // Print acknowledgment
        printf("Received acknowledgment: %s\n", ack);

        // Delay for 1 second
        sleep(1);
    }

    return 0;
}