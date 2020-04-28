# this is an example trial file
# each line in this file is one command
# there are two type of commands, phase commands and robot commands

#all comamands share the same format:
#  name_of_command(time_to_run_command, <some number of arguments>)
#   time_to_run_command is the numver of seconds afte rth trial starts
#       the command shoudl be sent to the robot
#   <some number of arguments> are other values taht the command shoudl take,
#       these vary depending on command. all arguments are seperated by commas

# phase commands set the phase of the trial at the time specifed

# lines that start with "#" do not send commands and can be used a comments on what you are doing

phase(0, '1') # this sets the phase to one
skee(3, 9) # executes the command skee 3 seconds after the program starts with input 9

phase(60, '2')

phase(60, '3')
