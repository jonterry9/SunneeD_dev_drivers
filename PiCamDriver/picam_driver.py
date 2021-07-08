import sys
from picamera import *

def read_pipe():
    while True:
        line = sys.stdin.readline()
        if (line is not None and line is not ""):
            cam = PiCamera()
            try:
                to_exec = "cam."
                to_exec += line
                exec(to_exec)
            except Exception as e:
                print("Request to camera failed with: %s" %e)
            cam.close()


read_pipe()
