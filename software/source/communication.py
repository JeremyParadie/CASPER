import serial
import queue
import time


class comsClass():

    def __init__(self, portSelection):
        self.usedSerial = serial.Serial(
            port=portSelection, baudrate=9600, timeout=0)

    # Takes one argument (The string to be sent), adds a newline, and encodes/sends it.
    def send(self, Transmission):
        Transmission = Transmission + '\n'
        print('Transmission: ', Transmission)
        encodedTransmission = Transmission.encode('utf-8')
        self.usedSerial.write(encodedTransmission)
        return(self.receive())

    # Recieves data from the serial port in bytes, and converts to a string.
    def receive(self):
        receptionBuffer = ''
        i = 0
        while self.usedSerial.in_waiting == 0:
            time.sleep(.001)
            i = i+1
            if i > 250:
                return(["Message Timeout Error"])
        for i in range(500):
            incoming = self.usedSerial.read(size=1)
            incoming = incoming.decode('utf-8')
            receptionBuffer = receptionBuffer + incoming
            if receptionBuffer.endswith('\n') == True:
                return([receptionBuffer])

        return(["Message Read Error"])
