"""
The file which handles I/O functions for the CASPER project.

Author: Per Van Dyke
"""
import serial
import queue
import time

class Radio():

    def __init__(self, port, baudrate=9600, timeout=0, Q):
        self.serial_port = serial.Serial(port=port, baudrate=baudrate, timeout=timeout)
        commsLoop(Q)


    def commsLoop(self, Q):
    	while(true):
    		if(Q.peek()[0] == "radio"):
    			command = queue.get()
    			if(command[2] == "send"):
    				send(command[3])
    				response = receive()
    				Q.put((command[1],"radio","response",response))

    #Takes one argument (The string to be sent), adds a newline, and encodes/sends it.
    def send(self, command):
        """
        send a command to the radio, returns the radio's responce or error.
        :param str command: the command sequence to send to the radio.
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

        #wait until no longer waitng for chars to come in or timeout has elapsed
        while not self.serial_port.in_waiting: # had been == 0
            time.sleep(.001) #FIXME: why have the wait? legacy or helps w/ hardware?
            #check to see if timed out
            if (time.monotonic() - start_time) >= timeout:
                return(["Message Timeout Error"])

        for _ in range(500): #FIXME: why 500? what does it signify
            incoming = self.serial_port.read(size=1)
            incoming = incoming.decode('utf-8')
            response += incoming
            if response.endswith('\n'):
                return([response])
        
        return(["Message Read Error"])
