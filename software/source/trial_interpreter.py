import os
import csv

#TODO Recieve Commands:
#Trial.load
#Trial.start
#Trial.end

# When instantiating a trial class both of the inputs, name and trial, should be included.
#Their current defaults are for demonstration/testing purposes only.

# The boolean input is for debug (not implemented?)

# Trials currently dont support new lines, so all trialFiles should be one long single line of methods with inputs in parentheses
#adjacent to their method. Methods should be separated from each other by a comma and a space (", "). The list of methods should be within
#square brackets [ ].

# Available methods are technically any executable python code if executed within Trial.py. However only the methods within Trial.py
#marked with #supportedMethod are supported use.

# If you wish too add new methods they should either return a string that the comms support as a command, or push a sequence of
#supported methods onto the exec stack that create the desired behavior

class Logger:

	def __init__(self, csvFile):
		self.log = open(csvFile, 'w')
		self.writer = csv.writer(self.log, delimiter=',')

	def write(self, time, event):
		self.writer.writerow([time, event])

class Trial: # this will be an instance of a trial, re-instantiated for each new trial

    def __init__(self, name="Default_Trial", trial=["tailSetAngle(50, 1)", "tailSetAngle(20, 1.5)", "tailSetAngle(50, 2)", "tailSetAngle(20, 2.5)", "tailSetAngle(50, 3)", "tailSetAngle(20, 3.5)", "tailSetAngle(50, 4)", "tailSetAngle(20, 4.5)", "tailSetAngle(50, 5)", "tailSetAngle(20, 5.5)", "tailSetAngle(-20, 6)", "audioPlay('track004.mp3', 7)", "audioStop(30)"], DEBUG=False):
        self.name = name
        self.execStack = list(
            map(lambda n: "self.lastOutput = Trial." + n, trial))
        self._DEBUG = DEBUG
        self.lastOutput = None
        i = 0
        csvExists = os.path.isfile("logs/" + name + str(i) + ".csv")
        while (csvExists == True):
            i = i + 1
            csvExists = os.path.isfile("logs/" + name + str(i) + ".csv")
        self.logger = Logger("logs/" + name + str(i) + ".csv")

    def getName(self):
        return self.name

    def isDone(self):
        # checks to see if the trial is over
        if len(self.execStack) == 0:
            return (True)
        return (False)

    def popNextLine(self):
        # runs next command in execStack
        if self.isDone():
            return
        exec(self.execStack.pop(0))
        return self.lastOutput

    def popAllLines(self):
        # runs everything in the execStack
        output = []
        for i in range(len(self.execStack)):
            output.append(Trial.popNextLine(self))
        return output

    #supportedMethod
    def tailGetAngle(time):
        # Note this is comm sendable
        return (time, "Robot Servo 0 getAngle")

    #supportedMethod
    def tailSetAngle(angle, time):
        # Note this is comm sendable
        return (time, "Robot Servo 0 setAngle " + str(angle))

    #supportedMethod
    def tailGoLimp(time):
        # Not this is com sendable
        return (time, "Robot Servo 0 goLimp")

    #supportedMethod
    def coverGetAngle(time):
        # Note this is comm sendable
        return (time, "Robot Servo 1 getAngle")

    #supportedMethod
    def coverSetAngle(angle, time):
        # Note this is comm sendable
        return (time, "Robot Servo 1 setAngle " + str(angle))

    #supportedMethod
    def tailGoLimp(time):
        # Not this is com sendable
        return (time, "Robot Servo 1 goLimp")

    #supportedMethod
    def audioSetVolume(percent, time):
        # Note this is comm sendable
        return (time, "Robot Audio setVolume " + str(percent))

    #supportedMethod
    def audioPlay(fileName, time):
        # Note this is comm sendable
        return (time, "Robot Audio play " + fileName)

    #supportedMethod
    def audioIsPlaying(time):
        # Note this is comm sendable
        return (time, "Robot Audio isPlaying")

    #supportedMethod
    def audioPause(time):
        # Note this is comm sendable
        return (time, "Robot Audio pause")

    #supportedMethod
    def audioResume(time):
        # Note this is comm sendable
        return (time, "Robot Audio resume")

    #supportedMethod
    def audioIsPaused(time):
        # Note this is comm sendable
        return (time, "Robot Audio isPaused")

    #supportedMethod
    def audioStop(time):
        # Note this is comm sendable
        return (time, "Robot Audio stop")

    #supportedMethod
    def audioIsStopped(time):
        # Note this is comm sendable
        return (time, "Robot Audio isStopped")

    #supportedMethod
    def audioGetFileName(time):
        # Note this is comm sendable
        return (time, "Robot Audio getFileName")

    #supportedMethod
    def audioSetVolume(percent, time):
        # Note this is comm sendable
        return (time, "Robot Audio setVolume " + str(percent) + "%")

    #supportedMethod
    def tailMotion(self, positionList, startTime, rate):
        pass
        # TODO
        # Note this is not comm sendable.
        # Adds the list of comm sendable functions that would create the intended motion to the stack then does runNextLine

   #supportedMethod
    def advancedTailMotion(WIP):
        pass
        # TODO
        # Note this is not comm sendable
        # Adds the list of comm sendable functions that would create the intended motion to the stack then does runNextLine

def loop(que):
    """
    desc: the loop used to manage talking to the trial interpreter.
    possible commands:
        'Trial.start' -> TODO define
        'Trial.end' -> closes the open log file for the current trial, and stops trial
    ;
    """
    current_trial = Trial();

    if que[0].startswith('Trial'):
        # fetch the message from the top of the que
        addr, retaddr, args  = que.pop(0)
        # parse the adress into just the command by spitiling and disposing
        # of the first item. the cmd is the address minus the module name
        cmd = addr.split('.')[1:]
        cmd_type = cmd[0] # convieneient for below

        if cmd_type == 'start':
            if len(args) == 3:
                current_trial = Trial(args[0], args[1], args[2])
            else:
                current_trial = Trial(args[0], args[1])
            que.append( (retaddr, None, current_trial.popAllLines()) )

            
        elif cmd_type == 'end':
            current_trial.logger.log.close()
        else:
            #error command not found