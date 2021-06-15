#include <stdio.h>
#include <string.h>
#include <math.h>
#include "lib_ULN2003_stepper.h"

int main(int argc, char *argv[])
{
    int stepper_fd, position, num_digits;

    position = 0;
    if (argc != 2) {
        printf("Provide one argument\n");
        exit(1);
    }
    num_digits = strlen(argv[1]);
    for (int i = 0; i < num_digits; i++) {
        position += pow(10.0, num_digits - i - 1) * ((int) *(argv[1] + i) - (int)'0');
    }
    printf("arg: %d\n",position);
    if (argc != 2 || position < 0 || position > 360 ) {
        printf("Provide exactly one integer in range [0,360)\n");
        exit(1);
    }

    if ((stepper_fd = open("/tmp/stepper", O_WRONLY)) == -1) {
        printf("err\n");
        exit(1);
    }

    write(stepper_fd,&position,sizeof(position));
    close(stepper_fd);
    exit(0);
}