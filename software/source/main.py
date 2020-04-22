"""
The entry point to the Graphical user user interface for the capser project
trial software.

Author: Jonah Yolles-Murphy


...
FIXME: talk about the project architecture here.
...
"""
# import normal packages
import threading

#import threads
import user_interface as ui
import radio_communication as radio
import trial_interpreter as ti
#FIXME: import debug_...

#initialize the message_que for communitcation
#que is a shortening of the word queue
message_que = [] # might become a dqueue, depends

#### define the task threads ####
#   each thread is a daemon so it terminates when this file exits
threads = []
args = (message_que,)

ui_thread = threading.Thread(target=ui.loop, args=args, daemon=True)
threads.append(ti_thread)

radio_thread = threading.Thread(target=radio.loop, args=args, daemon=True)
threads.append(radio_thread)

ti_thread = threading.Thread(target=ti.loop, args=args, daemon=True)
threads.append(ti_thread)


### define main task threads resources and loop ###

def loop(que):
    """
    desc: the loop used to monitor the message_que for exit commands;
    """

    #setup goes here:
    #    instantiang classes

    while True:

        #if the que's first message is adressed to main (may implememnt total loop through later)
        if que[0].startswith('main'):
            # fetch the message from the top of the que
            addr, retaddr, args  = que.pop(0)
            # parse the adress into just the command by spitiling and disposing
            # of the first item
            cmd = addr.split('.')[1:]

            # if the head of the command is exit, exit the program
            if cmd[0] == 'exit':
                exit() #b/c the threads are daemons they will die along with this one


#if this is the name main file start all the threads, enter main loop
if __name__ == '__main__':
    ui_thread.start()
    radio_thread.start()
    ti_thread.start()

    loop(que)
