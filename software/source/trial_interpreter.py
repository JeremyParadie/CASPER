import os
import sys
import subprocess
import csv


# When instantiating a trial class both of the inputs, name and trial, should be included.
#Their current defaults are for demonstration/testing purposes only.

class Logger:

	def __init__(self, csvFile):
		self.log = open(csvFile, 'w')
		self.writer = csv.writer(self.log, delimiter=',')

	def write(self, time, event):
		self.writer.writerow([time, event])

class Trial: # this will be an instance of a trial, re-instantiated for each new trial

    def __init__(self, name="Default_Trial", trial= "./trial_files/default_trial.py", robot=""):
        self.name = name
        self.trial = trial
        self.robot = robot # loop over all the function definitions in this py file so that arg 2 can use it
        i = 0
        csvExists = os.path.isfile("logs/" + name + str(i) + ".csv")
        while (csvExists == True):
            i = i + 1
            csvExists = os.path.isfile("logs/" + name + str(i) + ".csv")
        self.logger = Logger("logs/" + name + str(i) + ".csv")

    def getName(self):
        return self.name

    def run(self):
        return subprocess.check_output([sys.executable, self.trial])

def loop(que):
    """
    desc: the loop used to manage talking to the trial interpreter.
    possible commands:
        'trial.start' -> runs the specified trial file and returns its outputs
        'trial.end' -> closes the open log file for the current trial, and stops trial
    """
    current_trial = Trial()

    if que[0].startswith('trial'):
        # fetch the message from the top of the que
        addr, retaddr, args  = que.pop(0)
        # parse the adress into just the command by spitiling and disposing
        # of the first item. the cmd is the address minus the module name
        cmd = addr.split('.')[1:]
        cmd_type = cmd[0] # convieneient for below

        if cmd_type == 'start':
            current_trial = Trial(args[0], args[1], arg[2])

            que.append((retaddr, None, current_trial.run()))

            
        elif cmd_type == 'end':
            current_trial.logger.log.close()

            que.append((retaddr, None, "Trial Ended"))
        else:
            que.append((retaddr, None, "error command not found"))