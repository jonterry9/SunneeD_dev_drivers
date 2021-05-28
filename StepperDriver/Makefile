CC = gcc
RM = rm -rf

all: lib_ULN2003_stepper stepper_driver c_test

lib_ULN2003_stepper: lib_ULN2003_stepper.h
	@$(CC) -o $@ $^

stepper_driver: stepper_driver.c
	@$(CC) -o $@ $^ -lwiringPi

c_test: c_test.c
	@$(CC) -o $@ $^ -lwiringPi -lm

run: all 
	./c_test

clean:
	@$(RM) lib_ULN2003_stepper c_test stepper_driver *.bin