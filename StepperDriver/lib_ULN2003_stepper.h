#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <wiringPi.h>

#define STEPS_PER_REV 4096

int get_orientation();
int rot_n_deg(int n, int is_clockwise);

static int orientation = 0;

static int outPins[4] = {4,5,6,12};
static int STEP_SEQUENCE[8][4] = {
{0,0,0,1},
{0,0,1,1},
{0,0,1,0},
{0,1,1,0},
{0,1,0,0},
{1,1,0,0},
{1,0,0,0},
{1,0,0,1}
};

int rot_n_deg(int n, int is_clockwise)
{
    int steps, step_counter;
    steps = STEPS_PER_REV * ((double)n / 360.0);
    step_counter = 0;

    if (n < 0 || n > 360) return -1;

    if (is_clockwise) {
        orientation += n % 360;
    } else {
        orientation -= n;
        if (orientation < 0) orientation = 360 - orientation;
    }



    wiringPiSetup();
    for (unsigned int i = 0; i < sizeof(outPins); i++) {
        pinMode(outPins[i], OUTPUT);
        digitalWrite(outPins[i], LOW);
    }

    if (is_clockwise) {
        step_counter = sizeof(STEP_SEQUENCE) - 1;
    } else {
        step_counter = 0;
    }
    int i;
    for (i = 0; i < steps; i++) {
        for (int pin = 0; pin < 4; pin ++) {
            int xpin = outPins[pin];
            if (STEP_SEQUENCE[step_counter][pin] != 0) {
                digitalWrite(xpin, HIGH);            
            } else {
                digitalWrite(xpin, LOW);
            }
        }
        if (is_clockwise) {
            step_counter--;
        } else {
            step_counter++;
        }
        if (step_counter > 7) step_counter = 0;
        if (step_counter < 0) step_counter = 7;

        usleep(3000);
    }

    for (unsigned int i = 0; i < sizeof(outPins); i++) {
        digitalWrite(outPins[i], LOW);
    }

    return 0;
}