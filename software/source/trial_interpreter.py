import os
import sys
import subprocess
import csv

# When instantiating a trial class both of the inputs, name and trial, should be included.
#Their current defaults are for demonstration/testing purposes only.

class Logger:

    def __init__(self, csvFile):
        self.log = open(csvFile, 'w+')
        self.writer = csv.writer(self.log, delimiter=',')

    def write(self, time, event):
        self.writer.writerow([time, event])

class Trial: # this will be an instance of a trial, re-instantiated for each new trial

    def __init__(self, name="Default_Trial", trial= "./trials/default_trial.py", robot="./robots/example_squirrel.json"):
        self._name = name
        self.trial = trial
        self.robot = robot # loop over all the function definitions in this py file so that arg 2 can use it
        i = 0
        csvExists = os.path.isfile("logs/" + name + str(i) + ".csv")
        while (csvExists == True):
            i = i + 1
            csvExists = os.path.isfile("logs/" + name + str(i) + ".csv")
        self.logger = Logger("logs/" + name + str(i) + ".csv")

    @property
    def name(self):
        return self._name

    def run(self):
        pass
        #return subprocess.check_output([sys.executable, self.trial])

    def compile_trial(self):
        """
        desc: runs the trial and necesarry support scripts to ouput the
        list of commands to be sorted then sent to the robot;
        returns tuple: (robot_cmmands, phase_commands) each command is a tuple.
         	the first item is the time it should run (in seconds
        since the start of the trial) and the second is the string
        that is the command to send;
        """

        # the output of the trial file. should be sorted then sent to radio
        robot_commands = [] # a list of tuples
        phase_commands = []

        # the locals the robot_scripts shoudl deposit funcitons into
        trial_locals = {'_robot_commands' : robot_commands,
                        '_phase_commands' : phase_commands
                        }

        # load the base most python functions for the trial script
        with open('./robots/base_robot_commands.py') as file:
            exec(file.read(), locals=trial_locals)

        # load the base most python functions for the trial script
        with open('./robots/base_robot_commands.py') as file:
            exec(file.read(), locals=trial_locals)

        # load the robot specific  python functions for the trial script
        #repalce
        robot_command_file_path = self.robot.replace('.json','.py')
        if os.exits(robot_command_file_path):
            with open(robot_command_file_path) as file:
                exec(file.read(), locals=trial_locals)

        # run the trial and collects its output
        with open(self._trial) as file:
            exec(file.read(), locals=trial_locals)

        return robot_commands, phase_commands

def loop(que):
    """
    desc: the loop used to manage talking to the trial interpreter.
    possible commands:
        'trial.start' -> runs the specified trial file and returns its outputs
    arg inputs are it the order ('trialfile_path', 'subject_path', 'robot_path', {dict of subject information})
        'trial.end' -> closes the open log file for the current trial, and stops trial
    """
    current_trial = Trial()

    if len(que) and que[0][0].startswith('trial'):
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
