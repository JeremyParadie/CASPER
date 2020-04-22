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
        self.logger = logger.Logger("logs/" + name + str(i) + ".csv")

loop = lambda *args, **kwargs: None
