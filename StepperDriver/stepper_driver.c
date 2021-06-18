#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include "lib_ULN2003_stepper.h"

int orientation_file, cur_orientation;

void sig_handler(int signo)
{
    if (lseek(orientation_file, 0, SEEK_SET) != 0) {
        fprintf(stderr, "ERR: lseek fail\n");
        exit(1);
    }
    if (write(orientation_file, &cur_orientation, sizeof(cur_orientation)) == -1) {
        fprintf(stderr, "ERR: write fail\n");
        exit(1);
    }
    close(orientation_file);
    exit(0);
}

int main(void) {
    unsigned char byte_1;
    int fifo_fd, position, last_read, dif, dif2, ret, byte_2;
    const char *path = "/tmp/stepper";
    static char *orientation_path = "/tmp/orientation_rec.bin";
    last_read = ret = -1;
    cur_orientation = 0;

    if (signal(SIGTERM, sig_handler) == SIG_ERR || signal(SIGINT, sig_handler) == SIG_ERR) {
        fprintf(stderr, "ERR: couldn't register signal handlers");
        exit(1);
    }

    if ((orientation_file = open(orientation_path,O_RDWR)) == -1) {
        if ((orientation_file = open(orientation_path, O_CREAT | O_RDWR)) == -1) {
            fprintf(stderr, "ERR: stepper lib can't create file -- %s\n",strerror(errno));
            exit(1);
        }
    } else if (read(orientation_file, &cur_orientation, 2) == 0) {
        fprintf(stderr, "ERR: stepper lib can't get orentation");
        close(orientation_file);
        exit(1);
    }
    while (1) {
        if (read(0, &byte_1, 1) != 0) {
            if (byte_1 != '+' && byte_1 != '-') {
		if (byte_1 == 'o') {
   		    if (write(1, &cur_orientation, sizeof(cur_orientation)) <= 0) {
		        fprintf(stderr, "ERR: write to sunneed data pipe failed\n");
		        exit(1);
		    }
		    continue;
		} else if (read(0, &byte_2, 3) != 0) {
    		        position = (byte_2 << 8) | byte_1;
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
            } else if (byte_1 != 'o'){
                read(0, &position, sizeof(int));
                if (position >= 0 && position <= 360) {
                    if (byte_1 == '+') {
                        rot_n_deg(position, 1);
                    } else {
                        rot_n_deg(position, 0);
                    }
                }
            }
	}  
    }
    exit(1); /* should never be reached */
}
