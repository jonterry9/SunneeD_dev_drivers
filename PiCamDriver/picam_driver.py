import os, sys
from picamera import PiCamera
import enum

accepted_formats_pic = ['jpeg', 'png', 'gif', 'bmp', 'yuv', 'rgb', 'rgba', 'bgr', 'bgra', 'raw']
accepted_formats_vid = ['h264', 'mjpeg', 'yuv', 'rgb', 'rgba', 'bgr', 'bgra']

resize_val = 'None'
splitter_val = '0'
format_val = 'jpeg'
format_set = usePort_set = resize_set = splitter_set = bayer_set = motionOutput_set = False
useVidPort_val = 'False'
motionOutput_val = ''
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

def get_capture_args(args, capture_type):
    global format_set, usePort_set, resize_set, splitter_set, bayer_set, motionOutput_set, resize_val, splitter_val, format_val, useVidPort_val, motionOutput_val
    for param in args:
            if (not format_set and param[0:6] == 'format='):
                if (capture_type == 'pic'):
                    for good_val in accepted_formats_pic:
                        if (param[7:] == good_val):
                            format_set = True
                            format_val = param[7:]
                            break
                elif (capture_type == 'vid'):
                    for good_val in accepted_formats_vid:
                        if (param[7:] == good_val):
                            format_set = True
                            format_val = param[7:]
                            break
            elif (not usePort_set and param[0:14] == 'use_video_port='):
                if (param[15:] == 'True'):
                    usePort_set = True
                    useVidPort_val = 'True'
                elif (param[16:] == 'False'):
                    usePort_set = True
                    useVidPort_val = 'False'
            elif (not resize_set and param[0:6] == 'resize='):
                if (param[7] == '(' and param[-1] == ')'): # check argument format
                    idx = param[7:-1].index(',')
                    if (idx != -1):
                        resize_set = True
                elif (param[7:] == 'None'):
                    resize_set = True
                if (resize_set): resize_val = param[7:]
            elif (not splitter_set and param[0:13] == 'splitter_port='):
                try:
                    splitter_val = int(param[14:])
                    splitter_set = True
                except ValueError:
                    splitter_set = False
                    #invalid argument -- no number provided after '='
            elif (not bayer_set and param[0:6] == 'bayer='):
                if (param[7:] == 'True'):
                    set_bayer = True
                    bayer_val = 'True'
                elif (param[7:] == 'False'):
                    set_bayer = True
                    bayer_val = 'False'

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
    global format_set, usePort_set, resize_set, splitter_set, bayer_set, motionOutput_set, resize_val, splitter_val, format_val, useVidPort_val, motionOutput_val
    if (len(args) is 1):
        cam.capture(str(args[0]))
    else:
        code_to_exec = 'cam.capture(' + '"' + args[0] + '"'
        get_capture_args(args, 'pic')
        if (format_set): code_to_exec = code_to_exec + ', ' + format_val
        if (usePort_set): code_to_exec = code_to_exec + ', ' + useVidPort_val
        if (resize_set): code_to_exec = code_to_exec + ', ' + resize_val
        if (splitter_set): code_to_exec = code_to_exec + ', ' + splitter_val
        if (bayer_set): code_to_exec = code_to_exec + ', ' + bayer_val

        code_to_exec = code_to_exec + ')'
        exec(code_to_exec)

        format_set = usePort_set = resize_set = splitter_set = bayer_set = False

def func_4(args): #start_recording
    global format_set, usePort_set, resize_set, splitter_set, bayer_set, motionOutput_set, resize_val, splitter_val, format_val, useVidPort_val, motionOutput_val
    if (len(args) is 1):
        cam.start_recording(args[0])
    else:
        code_to_exec = 'cam.start_recording(' + '"' + args[0] + '"'
        get_capture_args(args, 'vid')
        if (format_set): code_to_exec = code_to_exec + ', ' + format_val
        if (usePort_set): code_to_exec = code_to_exec + ', ' + useVidPort_val
        if (resize_set): code_to_exec = code_to_exec + ', ' + resize_val
        if (splitter_set): code_to_exec = code_to_exec + ', ' + splitter_val

        code_to_exec = code_to_exec + ')'
        exec(code_to_exec)

        format_set = usePort_set = resize_set = splitter_set = False

def func_5(args): #stop_recording
    if (len(args) is 0):
        cam.stop_recording()
    elif (len(args) is 1):
        if (args[0][0:13] == 'splitter_port='):
            try:
                port = int(args[0][14:])
            except:
                port = None    
                #invalid arg for port -- not an int
            else:
                cam.stop_recording(splitter_port=port)

def func_6(args): #capture_continuous
    global format_set, usePort_set, resize_set, splitter_set, bayer_set, motionOutput_set, resize_val, splitter_val, format_val, useVidPort_val, motionOutput_val
    if (len(args) is 1):
        cam.capture_continuous(str(args[0]))
    else:
        code_to_exec = 'cam.capture_continuous(' + '"' + args[0] + '"'
        get_capture_args(args, 'pic')
        if (format_set): code_to_exec = code_to_exec + ', ' + format_val
        if (usePort_set): code_to_exec = code_to_exec + ', ' + useVidPort_val
        if (resize_set): code_to_exec = code_to_exec + ', ' + resize_val
        if (splitter_set): code_to_exec = code_to_exec + ', ' + splitter_val
        if (bayer_set): code_to_exec = code_to_exec + ', ' + bayer_val

        code_to_exec = code_to_exec + ')'
        exec(code_to_exec)

        format_set = usePort_set = resize_set = splitter_set = bayer_set = False

def func_7(args): #capture_sequence
    global format_set, usePort_set, resize_set, splitter_set, bayer_set, motionOutput_set, resize_val, splitter_val, format_val, useVidPort_val, motionOutput_val
    if (len(args) is 1):
        cam.capture_sequence(str(args[0]))
    else:
        code_to_exec = 'cam.capture_sequence(' + '"' + args[0] + '"'
        get_capture_args(args, 'pic')
        if (format_set): code_to_exec = code_to_exec + ', ' + format_val
        if (usePort_set): code_to_exec = code_to_exec + ', ' + useVidPort_val
        if (resize_set): code_to_exec = code_to_exec + ', ' + resize_val
        if (splitter_set): code_to_exec = code_to_exec + ', ' + splitter_val
        if (bayer_set): code_to_exec = code_to_exec + ', ' + bayer_val

        code_to_exec = code_to_exec + ')'
        exec(code_to_exec)

        format_set = usePort_set = resize_set = splitter_set = bayer_set = False

def func_8(args): #record_sequence
    global format_set, usePort_set, resize_set, splitter_set, bayer_set, motionOutput_set, resize_val, splitter_val, format_val, useVidPort_val, motionOutput_val
    if (len(args) is 1):
        cam.record_sequence(args[0])
    else:
        code_to_exec = 'cam.record_sequence(' + '"' + args[0] + '"'
        get_capture_args(args, 'vid')
        if (format_set): code_to_exec = code_to_exec + ', ' + format_val
        if (usePort_set): code_to_exec = code_to_exec + ', ' + useVidPort_val
        if (resize_set): code_to_exec = code_to_exec + ', ' + resize_val
        if (splitter_set): code_to_exec = code_to_exec + ', ' + splitter_val

        code_to_exec = code_to_exec + ')'
        exec(code_to_exec)

        format_set = usePort_set = resize_set = splitter_set = False

def func_9(args): #split_recording
    global format_set, usePort_set, resize_set, splitter_set, bayer_set, motionOutput_set, resize_val, splitter_val, format_val, useVidPort_val, motionOutput_val
    if (len(args) is 1):
        cam.split_recording(args[0])
    else:
        code_to_exec = 'cam.split_recording(' + '"' + args[0] + '"'
        get_capture_args(args, 'vid')
        if (format_set): code_to_exec = code_to_exec + ', ' + format_val
        if (motionOutput_set): code_to_exec = code_to_exec + ', ' + motionOutput_val
        if (splitter_set): code_to_exec = code_to_exec + ',' + splitter_val

        code_to_exec = code_to_exec + ')'
        exec(code_to_exec)

        format_set = usePort_set = resize_set = splitter_set = motionOutput_set = False

def func_10(args): #wait_recording
    if (len(args) is 0): return
    try:
        timeout = int(args[0])
    except ValueError:
        #invalid argument -- not an int
        return
    if (len(args) is 1):
        cam.wait_recording(timeout)
    elif (len(args) is 2):
        if (args[1][0:13] == 'splitter_port='):
            try:
                port = int(args[1][14:])
            except ValueError:
                #invalid argument -- not an int
                return
            else:
                cam.wait_recording(timeout, splitter_port=port)

function_map = [func_0, func_1, func_2, func_3, func_4, func_5, func_6, func_7, func_8, func_9, func_10]

def get_func_args(line):
    #line = line.replace(" ", "")
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