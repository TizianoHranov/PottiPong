#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>  // Include this header for mkfifo

//IT WORKED!!!!

int main() {
    char *fifo = "my_fifo";
    int data[1][32];
    char ack[10];

    // Create the named pipe (FIFO) if it doesn't exist
    if (access(fifo, F_OK) == -1) {
        if (mkfifo(fifo, 0666) == -1) {
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
    }

    int cnt = 0;

    while (1) {
        // Initialize data with some values
        for (int i = 0; i < 32; i++) {
            for(int j = 0; j < i; j++){
                data[0][i] += 2^j;  // 0x00, 0x01, 0x02, .... 0xff
            }
        }


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

        // Delay for 1 second
        sleep(1);
    }

    return 0;
}