import os, sys
import signal
from picamera import *

#############################
cam = PiCamera()
#############################
path = "/tmp/cam_driver"
try:
    fifo = open(path, 'r')
except FileNotFoundError as e:
    try:
        os.mkfifo(path)
    except OSError as e:
        print("Failed to create FIFO: %s" %e)
        exit(1)
    else:
        fifo = open(path, 'r')
#############################

def handle_exit(_signo, _stack_frame):
    print("Camera driver exiting")
    cam.close()
    fifo.close()
    sys.exit(0)

def read_pipe():
    while True:
        line = fifo.readline()
        if (line is not None and line is not ""):
            try:
                to_exec = "cam."
                to_exec += line
                exec(to_exec)
            except Exception as e:
                print("Request to camera failed with: %s" %e)

signal.signal(signal.SIGINT, handle_exit)
signal.signal(signal.SIGTERM, handle_exit)
read_pipe()
