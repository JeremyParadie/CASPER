"""
The file which handles I/O functions for the CASPER project.

Author: Per Van Dyke
"""
import os
import serial
import queue
import time
from example_commands import examples



def port_scan():
    #find all '/dev/tty.usbmodem#####' of of all the devices in /dev/
    found_ports = [f"/dev/{file}" for file in os.listdir('/dev/') if file.startswith('tty.usbmodem')]
    #if len(available_serial_ports) > 1:
    #    raise Exception(f"more than one robot realy plugged in. found {found_ports}")
    #else:
    path = found_ports[0]
    return path


def loop(que):
    """
    desc: the loop used to manage talking to the radio.
    possible commands:
        'radio.robot' -> sends the first item in args as a message to the radio, adds a responce message to the queue
        'radio.relay' -> sends the first item in args as a message to the relay, adds a responce message to the queue

    ;
    """
    # que is short for queue, it is the main message question
    # Note on implememntation: the final version fo this will have a sub que but
    #   in the beggining we won;t implement that in most threads

    #instatiate any objects the loop will manage:
    radio = serial.Serial(port=port_scan(), baudrate=9600, timeout=.15)

    while True:
        # if the que's first message is adressed to radio (may implememnt total loop through later)
        if que[0].startswith('radio'):
            # fetch the message from the top of the que
            addr, retaddr, args  = que.pop(0)
            # parse the adress into just the command by spitiling and disposing
            #   of the first item. the cmd is the adress minus the module name
            cmd = addr.split('.')[1:]
            cmd_type = cmd[0] # convieneient for below

            # check each option for a cmd_type and exexcute the coresponding code (then respond)
            if cmd_type == 'robot' or cmd_type == 'relay':
                # the first item of args should always be the message to send to the robot
                radio.write(f"{cmd_type} {args[0]}")

                # here will need to cahgned to be non-vlocking and scan the
                # message que (for now that will be omitted to make the data flow more clear)

                # try to fetcha responce, otherwise don't send  responce
                # (this might change depending on how we think the system sould behave)
                # might be empty
                responce = radio.readline()

                # append the responce to the message into the queue
                if retaddr is not None:
                    que.append( (retaddr, None, responce) )
