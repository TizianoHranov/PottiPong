#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

int main() {
    char *fifo = "my_fifo";
    int data[32][8];
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

        for (int i = 0; i < 32; i++) {
            for (int j = 0; j < 8; j++) {
                data[i][j] = 0;
            }
        }

        for (int i = 0; i < 32; i++) {
            for (int j = 0; j < 8; j++) {
                if(i%2 == 0 && j%2 == 1){
                    data[i][j] = 1;
                }
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
        usleep(1000 * 100);
    }

    return 0;
}