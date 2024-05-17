#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

int main() {
    char *fifo = "my_fifo";
    int data[32][8];

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
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 32; x++) {  
                if(x%2 == 0 && y%2 == 0){
                    data[x][y] = 1;
                } else if (x%2 != 0 && y%2 == 1){
                    data[x][y] = 1;
                } else {
                    data[x][y] = 0;
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
