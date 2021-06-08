#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

int main(void)
{
    int cam_fifo;
    const char *path = "/tmp/cam_driver";
    
    char *data = "0\n";

    if ( (cam_fifo = open(path, O_WRONLY)) == -1) {
        fprintf(stderr, "ERR: couldn't open FIFO\n");
        exit(1);
    }

    if (write(cam_fifo, data, strlen(data)) <= 0) {
        fprintf(stderr, "ERR: could not write to FIFO\n");
        exit(1);
    }
    printf("wrote '0(start_preview())'\n");
    sleep(2);
    data = "3, /home/pi/Sunneed_testDir/img.jpg\n";
    if (write(cam_fifo, data, strlen(data)) <= 0) {
        fprintf(stderr, "ERR: could not write to FIFO\n");
        exit(1);
    }
    printf("wrote '3(capture()), /home/pi/Sunneed_testDir/img.jpg'\n");
    data = "1\n";
    if (write(cam_fifo, data, strlen(data)) <= 0) {
        fprintf(stderr, "ERR: could not write to FIFO\n");
        exit(1);
    }
    printf("wrote '1'(stop_preview())\n");

    data = "4, test_rec, format=mjpeg\n";
    if (write(cam_fifo, data, strlen(data)) <= 0) {
        fprintf(stderr, "ERR: could not write to FIFO\n");
        exit(1);
    }

    data = "5\n";
     if (write(cam_fifo, data, strlen(data)) <= 0) {
        fprintf(stderr, "ERR: could not write to FIFO\n");
        exit(1);
    }

    close(cam_fifo);
    return 0;
}