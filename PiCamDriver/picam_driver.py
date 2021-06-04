import os, sys
from picamera import PiCamera

cam = PiCamera()
#################################################
path = "/tmp/cam_driver"
#path = "/dev/cam_driver"
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
##################################################

def func_0(args): #start_preview()
    if (len(args) is 0):
        cam.start_preview()
    elif (len(args) is 1):
        cam.start_preview(str(args[0]))

def func_1(args): #stop_preview()
    if (len(args) is 0):
        cam.stop_preview()

def func_2(args): #set rotation
    if (len(args) is 1):
        cam.rotation = int(args[0]) - int('0')

def func_3(args): #capture
    if (len(args) is 1):
        cam.capture(str(args[0]))

function_map = [func_0, func_1, func_2, func_3]

def get_func_args(line):
    while (line[0] == " "):
        line = line[1:]
    arg_set = []
    while True:
        try:
            index = line.index(',')
            arg_set.append(line[0:index])
            line = line[index + 2:]
        except ValueError:
            if (len(line) != 0):
                arg_set.append(line[0:-1])
            break
    func_num = int(arg_set[0]) - int('0')
    arg_set.pop(0)
    return func_num, arg_set

def handle_line(line):
    func_indx, arg_set = get_func_args(line)
    if (func_indx >= 0 and func_indx < len(function_map)):
        function_map[func_indx](arg_set)

def read_pipe():
    while True:
        line = fifo.readline()
        if (line is not None and line is not ""):
            handle_line(line)

read_pipe()