
def command(time, command):
    """
    desc: this the base funtion used to send commands to a robot, the first
        argument is the number of seconds after the trial starts the command
        should be run. the second argument is the command that should be sent,
        surround the command in quotes;
    """
    global _robot_commands
    _robot_commands.append((time, command))

def phase(time, name):
    """
    desc: this the base function used to send current phase, the first
        argument is the number of seconds after the trial starts that
        this phase should start. the second argument is the name of the phase
        in quotes;
    """
    _phase_commands.append((time, name))
