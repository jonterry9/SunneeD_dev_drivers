#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include "lib_ULN2003_stepper.h"

int orientation_file, cur_orientation;

void sig_handler(int signo)
{
    if (ftruncate(orientation_file, 0) != 0) {
        fprintf(stderr, "ERR: truncate fail\n");
        exit(1);
    }
    if (write(orientation_file, &cur_orientation, sizeof(cur_orientation)) == -1) {
        fprintf(stderr, "ERR: write fail\n");
        exit(1);
    }
    close(orientation_file);
    exit(0);
}

int main(void)
{
    int fifo_fd, position, last_read, dif, dif2, ret;
    const char *path = "/dev/stepper";
    static char *orientation_path = "orientation_rec.bin";
    last_read = ret = -1;
    cur_orientation = 0;

    if (signal(SIGTERM, sig_handler) == SIG_ERR || signal(SIGINT, sig_handler) == SIG_ERR) {
        fprintf(stderr, "ERR: couldn't register signal handlers");
        exit(1);
    }

    if ((orientation_file = open(orientation_path,O_RDWR)) == -1) {
        if ((orientation_file = open(orientation_path, O_CREAT | O_RDWR, S_IRWXU | S_IWOTH)) == -1) {
            fprintf(stderr, "ERR: stepper lib can't create file\n");
            exit(1);
        }
    } else if (read(orientation_file, &cur_orientation, 2) == 0) {
        fprintf(stderr, "ERR: stepper lib can't get orentation");
        close(orientation_file);
        exit(1);
    }
    if ((fifo_fd = open(path, O_RDONLY))  == -1) { /* open pipe for reading */
        if (mkfifo(path, S_IRWXU | S_IWOTH) == -1) { /* if pipe doesn't exist, create named pipe */
            fprintf(stderr, "ERR: stepper driver could not create pipe\n");
            exit(1);
        }
        chmod(path, S_IWOTH);
        if ((fifo_fd = open(path, O_RDONLY))  == -1) {
            fprintf(stderr, "ERR: stepper driver could not open pipe");
            exit(1);
        }
    }   
    while (1) {
        if (read(fifo_fd, &position, sizeof(int)) != 0) {
            if (position >= 0 && position <= 360 && position != last_read) { /* check for position validity & pointless calls (dont move motor)*/

                dif = cur_orientation - position;
                dif2 = 360 - abs(dif);
                cur_orientation = position;
                if (dif < 0) {
                        if (dif2 < abs(dif)) {
                                rot_n_deg(dif2, 0);
                        } else {
                                rot_n_deg(abs(dif), 1);
                        }
                } else {
                        if (dif2 < dif) {
                                rot_n_deg(dif2, 1);
                        } else {
                                rot_n_deg(dif, 0);
                        }
                }
                last_read = position;
            }
        }
    }
    exit(1); /* should never be reached */
}