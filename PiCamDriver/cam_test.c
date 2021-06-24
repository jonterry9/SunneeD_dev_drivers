#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

int
main(void)
{
    int dev_fd;
    const char *path = "/tmp/cam_driver";
    char *request;

    if ( (dev_fd = open(path, O_WRONLY)) == -1) {
	fprintf(stderr, "ERR: could not open device\n");
	exit(1);
    }

    printf("TEST TAKING PICTURE: ...\n");

    request = "start_preview()\n";
    if (write(dev_fd, request, strlen(request)) == -1) {
	fprintf(stderr, "ERR: could not write to device\n");
	exit(1);
    }

    request = "capture('img.jpg')\n";
    if (write(dev_fd, request, strlen(request)) == -1) {
	fprintf(stderr, "ERR: could not write to device\n");
	exit(1);
    }
    
    request = "stop_preview()\n";
    if (write(dev_fd, request, strlen(request)) == -1) {
	fprintf(stderr, "ERR: could not write to device\n");
	exit(1);
    }

    printf("\tDone\n");

    printf("TESTING TAKING VIDEO(5sec video): ...\n");

    printf("\tSetting resolution: ...\n");
    request = "resolution = (640, 480)\n";
    if (write(dev_fd, request, strlen(request)) == -1) {
	fprintf(stderr, "ERR: could not write to device\n");
	exit(1);
    }
    printf("\t\tDone\n");
    
    request = "start_preview()\n";
    if (write(dev_fd, request, strlen(request)) == -1) {
	fprintf(stderr, "ERR: could not write to device\n");
	exit(1);
    }

    request = "start_recording('vid.h264')\n";
    if (write(dev_fd, request, strlen(request)) == -1) {
	fprintf(stderr, "ERR: could not write to device\n");
	exit(1);
    }
    
    sleep(5);

    request = "stop_recording()\n";
    if (write(dev_fd, request, strlen(request)) == -1) {
	fprintf(stderr, "ERR: could not write to device\n");
	exit(1);
    }

    request = "stop_preview()\n";
    if (write(dev_fd, request, strlen(request)) == -1) {
	fprintf(stderr, "ERR: could not write to device\n");
	exit(1);
    }

    printf("TESTING INVALID PiCamera SYNTAX: ...\n");

    request = "capture()\n";
    if (write(dev_fd, request, strlen(request)) == -1) {
	fprintf(stderr, "ERR: could not write to device\n");
	exit(1);
    }

    printf("\tDone\n");

    printf("TESTING GARBAGE WRITE TO PiCam: ...\n");

    request = "abcdefg(&'";
    if (write(dev_fd, request, strlen(request)) == -1) {
	fprintf(stderr, "ERR: could not write to device\n");
	exit(1);
    }

    printf("\tDone\n");
}
