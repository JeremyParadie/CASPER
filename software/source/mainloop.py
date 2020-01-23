import communication
import Trial
import GUIX
import serial
import queue
import threading
import re
import time


class mainLoop():
	
	def __init__(self):
		self.main('Select Port: ')
	
	def main(self, portPrompt):
		portSelection = input(portPrompt)
		com = communication.comsClass(portSelection=portSelection)
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

			if GUIq.empty() == False:
				while GUIq.empty() == False:
					fromGUIq = GUIq.get()
					print(fromGUIq)
					if fromGUIq == 'End Trial':
						trialRun = False
					if 'hotkey' in fromGUIq:
						currentTrial.logger.writeLog(timeTicks, fromGUIq['hotkey'])
					if 'Trial' in fromGUIq:
						trialFile = open(r'trials/' + fromGUIq['Trial'], "r")
						trialString = trialFile.read()
						trialTrial = [x.strip() for x in trialString.split(' , ')]
						selectedTrial = Trial.Trial(fromGUIq['Trial'], trialTrial)
					if 'manualcommand' in fromGUIq:
						com.send(exec("currentTrial." + fromGUIq['manualcommand']))
						currentTrial.logger.writeLog(timeTicks, fromGUIq['manualcommand'])
					if 'observerEntry' in fromGUIq:
						pass

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
			if exitq.empty() == False:
				break
			#print(timeTicks)
			#print(GUIThread.is_alive())
			loopNumber = loopNumber + 1



mainLoop()