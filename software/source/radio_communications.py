"""
The file which handles I/O functions for the CASPER project.

Author: Per Van Dyke
"""
import serial
import queue
import time

def function_to_auto_scan_for_port():
    ...

def wait_scan(duration):
    """
    desc: a non-blocking wiat funtion that scans the message que from radio
        radio adressed messages, this is for when a radio specific que is
        implemented;
    arg int, float duration: the time to wait until;
    """
    ...

#setup classes
class Radio():

    def __init__(self, port, baudrate=9600, timeout=0, Q):
        self.serial_port = serial.Serial(port=port, baudrate=baudrate, timeout=timeout)
        commsLoop(Q)

    #Takes one argument (The string to be sent), adds a newline, and encodes/sends it.
    def send(self, command):
        """
        desc: send a command to the radio, returns the radio's responce or error;
        arg str command: the command sequence to send to the radio;
        """
        if not command.endswith('\n'):
            command += '\n'

        print("command sent to robot: '"+command+"'")

        #ensure command is compaitlbe with the serial_port
        command = command.encode('utf-8')
        self.serial_port.write(command)

        return(self.receive())

    #Recieves data from the serial port in bytes, and converts to a string.
    def receive(self, timeout=.25):
        """
        """
        response = ''
        start_time = time.monotonic()

        # wait until all chars come in or timeout has elapsed
        while not self.serial_port.in_waiting: # had been == 0
            wait_scan(.001) # waits while polling message que
            # check to see if timed out
            if (time.monotonic() - start_time) >= timeout:
                raise Exception('Radio responce timed out')

        for _ in range(500): #FIXME: why 500? what does it signify
            incoming = self.serial_port.read(size=1)
            incoming = incoming.decode('utf-8')
            if incoming == '\n':
                return response
            else:
                response += incoming

        return response

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
    port = function_to_auto_scan_for_port()
    radio = Radio()

    # if the que's first message is adressed to radio (may implememnt total loop through later)
    if que[0].startswith('radio'):
        # fetch the message from the top of the que
        addr, retaddr, args  = que.pop(0)
        # parse the adress into just the command by spitiling and disposing
        # of the first item. the cmd is the adress minus the module name
        cmd = addr.split('.')[1:]
        cmd_type = cmd[0] # convieneient for below

        # check each option for a cmd_type and exexcute the coresponding code (then respond)
        if cmd_type == 'robot' or cmd_type == 'relay':
            # the first item of args should always be the message to send to the robot
            radio.send(f"{cmd_type} {args[0]}")

            # here will need to cahgned to be non-vlocking and scan the
            # message que (for now that will be omitted to make the data flow more clear)

            # try to fetcha responce, otherwise don't send  responce
            # (this might change depending on how we think the system sould behave)
            try:
                responce = radio.receive()
            except:
                continue

            # append the responce to the message into the queue
            if retaddr is not None:
                que.append( (retaddr, None, responce) )

        elif cmd_type == 'example_command':
            # this is an example of another command (in a ctiallity the radio
            #   commands will require no sub-adressing)
            pass# que.append('temp responce')
        elif cmd_type == 'nesting_argument':
            sub_cmd = cmd[1]
            if sub_cmd == 'option1':
                pass # execute option 1
                # que.append('temp responce')
            elif sub_cmd == 'option2':
                pass # execute option 2
                # que.append('temp responce')
