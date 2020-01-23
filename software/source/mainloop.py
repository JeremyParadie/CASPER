import Communication
import Trial
import GUI
import serial
import queue
import threading
import re
import time

"""This is the main loop for the program, everything moves through here, and this is the file which is actually run."""
class MainLoop():

    def __init__(self):
        self.main('Select Port: ')

    def main(self, portPrompt):
        # Set the port to be used by serial.
        portSelection = input(portPrompt)
        com = communication.comsClass(portSelection=portSelection)
        # Set up some GUI stuff.
        MainGUI = GUI.GUI()
        GUIq = queue.PriorityQueue()
        # Initialize trials.
        currentTrial = None
        selectedTrial = Trial.Trial()
        # Set up thread safe queues.
        commandq = queue.PriorityQueue()
        exitq = queue.Queue()
        # Start the GUI.
        GUIThread = threading.Thread(target=MainGUI.Main, name='GUIThread', args=(GUIq, commandq, exitq), daemon=True)
        GUIThread.start()
        transmissionBuffer = []
        # Set up tracking stuff for the main loop. 
        loopNumber = 0
        run = True
        trialRun = False
        startTicks = time.time()
        timeTicks = 0
        while run == True:
            timeTicks = (time.time() - startTicks)

            if currentTrial != selectedTrial:
                currentTrial = selectedTrial
                trialRun = True
                timeTicks = 0
                commandq = queue.PriorityQueue()
                commands = currentTrial.popAllLines()
                for i in range(len(commands)):
                    commandq.put(commands[i])

            # If the GUIq is not empty, see what's in it and take appropriate action.
            if GUIq.empty() == False:
                while GUIq.empty() == False:
                    fromGUIq = GUIq.get()
                    print(fromGUIq)
                    # If the trial is over, end it.
                    if fromGUIq == 'End Trial':
                        trialRun = False
                    # If a hotkey was pressed, log it.
                    if 'hotkey' in fromGUIq:
                        currentTrial.logger.writeLog(
                            timeTicks, fromGUIq['hotkey'])
                    # Ian needs to comment this bit
                    if 'Trial' in fromGUIq:
                        trialFile = open(r'trials/' + fromGUIq['Trial'], "r")
                        trialString = trialFile.read()
                        trialTrial = [x.strip()
                                      for x in trialString.split(' , ')]
                        selectedTrial = Trial.Trial(
                            fromGUIq['Trial'], trialTrial)
                    # If the user issues a manual command, send it to the robot.
                    if 'manualcommand' in fromGUIq:
                        com.send(exec("currentTrial." +
                                      fromGUIq['manualcommand']))
                        currentTrial.logger.writeLog(
                            timeTicks, fromGUIq['manualcommand'])
                    # Not implemented I think?
                    if 'observerEntry' in fromGUIq:
                        pass

            # If there are no commands left, and we are running a trial, check if comms are working?
            # Also need Ian to confirm here.
            if commandq.empty() == False and trialRun == True:
                if commandq.queue[0][0] <= timeTicks:
                    if (timeTicks - commandq.queue[0][0]) > 1:
                        print("running " + str(timeTicks - commandq.queue[0][0]) + " slow")
                    command = commandq.get()[1]
                    commandReturn = com.send(command)
                    print(commandReturn)
                    if re.match("^OK", commandReturn[0]) == None:
                        # TODO Log Error
                        print("failed reception")
                    else:
                        currentTrial.logger.writeLog(timeTicks, command)
            # If the user has closed the program, exit MainLoop cleanly.
            if exitq.empty() == False:
                break
            # print(timeTicks)
            # print(GUIThread.is_alive())
            loopNumber = loopNumber + 1


MainLoop()
