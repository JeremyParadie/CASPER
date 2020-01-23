"""
The main file for the CASPER project. Manages GUI threads, I/O, etc

Authors: Per Van Dyke, Ian MacPherson
"""
import communication
import Trial
import GUIX
import serial
import queue
import threading
import re
import time


def main(self, port):
    """
    docs here
    """
    #make the object representing the relay
	com = communication.Radio(port=port)

	GUI = GUIX.GUI()
	GUIq = queue.PriorityQueue()

	currentTrial = None
	selectedTrial = Trial.Trial()
	commandq = queue.PriorityQueue()
	exitq = queue.Queue()

	GUIThread = threading.Thread(target=GUI.Main, name='GUIThread', args=(GUIq,commandq,exitq), daemon=True)
	GUIThread.start()

    transmissionBuffer = []
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

		while GUIq.empty() == False:
			fromGUIq = GUIq.get()
			print(fromGUIq)
			# If trial is over, end it
			if fromGUIq == 'End Trial':
				trialRun = False
			# If queue contains a hotkey command, log it
			if 'hotkey' in fromGUIq:
				currentTrial.logger.writeLog(timeTicks, fromGUIq['hotkey'])
			# Open the selected trial from the GUI and set it accordingly in main
			if 'Trial' in fromGUIq:
				trialFile = open(r'trials/' + fromGUIq['Trial'], "r")
				trialString = trialFile.read()
				trialTrial = [x.strip() for x in trialString.split(' , ')]
				selectedTrial = Trial.Trial(fromGUIq['Trial'], trialTrial)
			# If the user issues a manual command, send it
			if 'manualcommand' in fromGUIq:
				com.send(exec("currentTrial." + fromGUIq['manualcommand']))
				currentTrial.logger.writeLog(timeTicks, fromGUIq['manualcommand'])
			# Not yet implemented
			if 'observerEntry' in fromGUIq:
				pass

			# If there are no commands, but trial should be continuing, check connection.
			# Need Ian to confirm this
			if commandq.empty() == False and trialRun == True:
				if commandq.queue[0][0] <= timeTicks:
					if (timeTicks - commandq.queue[0][0]) > 1:
						print("running " + str (timeTicks - commandq.queue[0][0]) + " slow")
					command = commandq.get()[1]
					commandReturn = com.send(command)
					print(commandReturn)
					if re.match("^OK", commandReturn[0]) == None:
						#TODO Log Error
						print("failed reception")
					else:
						currentTrial.logger.writeLog(timeTicks, command)
			# If the user closes the GUI, exit cleanly
			if exitq.empty() == False:
				break
			#print(timeTicks)
			#print(GUIThread.is_alive())
			loopNumber = loopNumber + 1

if __name__ == '__main__':
    port = input('select the port for the Relay:')
    main(port)
