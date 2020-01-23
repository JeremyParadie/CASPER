"""
The file which creates and runs the GUI for the CASPER project.

Author: Per Van Dyke
"""
from tkinter import *
from tkinter import ttk
from tkinter import messagebox
import os
import csv
import queue

class GUI():

	def __init__(self):
		self.observer = ''
		self.date = ''
		self.recorder = ''
		self.videoPerson = ''
		self.location = ''


	#Programs main loop, cycles as fast as possible to keep GUI responsive.
	def Main(self, GUIq, commandq, exitq):
		self.GUIq = GUIq
		self.commandq = commandq
		self.exitq = exitq
		self.root = Tk()
		self.root.title('Robot Squirrel Mk2')

		#setup self.mainframe
		self.mainframe = ttk.Frame(self.root, padding='6 6 12 12')
		self.mainframe.grid(column=0, row=0, sticky=(N, W, E, S))
		self.root.columnconfigure(0, weight=1)
		self.root.rowconfigure(0, weight=1)

		#set the different kinds of trials
		options = os.listdir('trials/')
		self.trialTypes = StringVar()
		self.trialTypes.set('select trial type')

		#Create dropdown to select type of trial
		self.trialType = OptionMenu(self.mainframe, self.trialTypes, options)
		self.trialType.grid(column=1, row=8, sticky=(N))

		#create label for list of trial types
		label3 = ttk.Label(self.mainframe, text= 'Select trial type below.')
		label3.grid(column=1, row=7, sticky=(S))

		#create begin trial button
		beginTrial = ttk.Button(self.mainframe, text='Begin Trial', command=self.trial)
		beginTrial.grid(column=2, row=4)

		#create end trial button
		endTrial = ttk.Button(self.mainframe, text='End Trial', command=self.endTrial)
		endTrial.grid(column=2, row=5)

		#create label for Observer entry
		observerLable = ttk.Label(self.mainframe, text = 'Observer')
		observerLable.grid(column=1, row=0, sticky=(S))

		#create Observer entry
		observer = StringVar()
		self.entry1 = ttk.Entry(self.mainframe, textvariable=observer)
		self.entry1.grid(column=1, row=1, sticky=(N))		
		
		#create label for date entry
		dateLable = ttk.Label(self.mainframe, text= 'Date')
		dateLable.grid(column=2, row=0, sticky=(S))

		#create date entry
		date = StringVar()
		self.entry2 = ttk.Entry(self.mainframe, textvariable=date)
		self.entry2.grid(column=2, row=1, sticky=(N))
		
		#create label for recorder entry
		recorderLable = ttk.Label(self.mainframe, text= 'Recorder')
		recorderLable.grid(column=1, row=2, sticky=(S))

		#create recorder entry
		recorder = StringVar()
		self.recorderEntry = ttk.Entry(self.mainframe, textvariable=recorder)
		self.recorderEntry.grid(column=1, row=3, sticky=(N))

		#create label for video person entry
		dateLable = ttk.Label(self.mainframe, text= 'Video Person')
		dateLable.grid(column=1, row=4, sticky=(S))

		#create video person entry
		videoPerson = StringVar()
		self.videoPersonEntry = ttk.Entry(self.mainframe, textvariable=videoPerson)
		self.videoPersonEntry.grid(column=1, row=5, sticky=(N))

		#create label for date entry
		locationLable = ttk.Label(self.mainframe, text= 'Location')
		locationLable.grid(column=2, row=2, sticky=(S))

		#create date entry
		location = StringVar()
		self.locationEntry = ttk.Entry(self.mainframe, textvariable=location)
		self.locationEntry.grid(column=2, row=3, sticky=(N))

		#create label for notes
		noteLabel = ttk.Label(self.mainframe, text= 'Enter any misc notes below.')
		noteLabel.grid(column=4,row=1)

		#create note textbox
		self.notes = Text(self.mainframe, width=50, height=3, wrap='word')
		self.notes.grid(column=4, row=2, padx=(10,0), rowspan=2)

		#create button for submitting textbox
		submitNotes = ttk.Button(self.mainframe, text='Submit Notes', command=self.manualSend)
		submitNotes.grid(column=5, row=2, sticky=(W))

		#Create Log field
		#self.log = Text(self.mainframe, width=50, height=10, wrap='word', state='disabled')
		#self.log.grid(column=4, row=4, padx=(10,0), pady=5, rowspan=3)

		#Create Scrollbar for Log field
		#self.logScroll = ttk.Scrollbar(self.mainframe, orient=VERTICAL, command=self.log.yview)
		#self.logScroll.grid(column=5, row=4, sticky=(W, N, S), rowspan=3)
		#self.log['yscrollcommand'] = self.logScroll.set

		#Create log refresh button
		#logRefresh = ttk.Button(self.mainframe, text='Refresh Log', command=self.refreshLog)
		#logRefresh.grid(column=5, row=3, sticky=(W))

		#Create advanced options button
		advOptions = ttk.Button(self.mainframe, text='Advanced Options', command=self.advOptFrm)
		advOptions.grid(column=1, row=9, sticky=(N))

		#Create all of the hotkeys to press to log squirrel behavior.

		treeBehavior = ttk.Button(self.mainframe, text='t - tree', command=self.treeBehaviorDef)
		treeBehavior.grid(column=6, row=0)
		treeBehavior.bind_all('t', self.treeBehaviorDef)

		quaaBehavior = ttk.Button(self.mainframe, text='q - Quaa', command=self.quaaBehaviorDef)
		quaaBehavior.grid(column=6, row=1)
		quaaBehavior.bind_all('q', self.quaaBehaviorDef)

		flickBehavior = ttk.Button(self.mainframe, text='z - flick', command=self.flickBehaviorDef)
		flickBehavior.grid(column=6, row=2)
		flickBehavior.bind_all('z', self.flickBehaviorDef)

		bipedBehavior = ttk.Button(self.mainframe, text='b - biped', command=self.bipedBehaviorDef)
		bipedBehavior.grid(column=6, row=3)
		bipedBehavior.bind_all('b', self.bipedBehaviorDef)

		descendBehavior = ttk.Button(self.mainframe, text='d - descend', command=self.descendBehaviorDef)
		descendBehavior.grid(column=6, row=4, sticky=(N))
		descendBehavior.bind_all('d', self.descendBehaviorDef)

		runBehavior = ttk.Button(self.mainframe, text='r - run', command=self.runBehaviorDef)
		runBehavior.grid(column=6, row=5, sticky=(N))
		runBehavior.bind_all('r', self.runBehaviorDef)

		backBehavior = ttk.Button(self.mainframe, text='p - back in view', command=self.backBehaviorDef)
		backBehavior.grid(column=6, row=6)
		backBehavior.bind_all('p', self.backBehaviorDef)

		groundBehavior = ttk.Button(self.mainframe, text='g - ground', command=self.groundBehaviorDef)
		groundBehavior.grid(column=7, row=0)
		groundBehavior.bind_all('g', self.groundBehaviorDef)

		othervocBehavior = ttk.Button(self.mainframe, text='j - other voc', command=self.othervocBehaviorDef)
		othervocBehavior.grid(column=7, row=1)
		othervocBehavior.bind_all('j', self.othervocBehaviorDef)

		flagBehavior = ttk.Button(self.mainframe, text='x - flag', command=self.flagBehaviorDef)
		flagBehavior.grid(column=7, row=2)
		flagBehavior.bind_all('x', self.flagBehaviorDef)

		gttBehavior = ttk.Button(self.mainframe, text='y - go to tree', command=self.gttBehaviorDef)
		gttBehavior.grid(column=7, row=3)
		gttBehavior.bind_all('y', self.gttBehaviorDef)

		groomBehavior = ttk.Button(self.mainframe, text='m - groom', command=self.groomBehaviorDef)
		groomBehavior.grid(column=7, row=4, sticky=(N))
		groomBehavior.bind_all('m', self.groomBehaviorDef)

		stillBehavior = ttk.Button(self.mainframe, text='s - still', command=self.stillBehaviorDef)
		stillBehavior.grid(column=7, row=5, sticky=(N))
		stillBehavior.bind_all('s', self.stillBehaviorDef)

		interactionBehavior = ttk.Button(self.mainframe, text='i - interaction', command=self.interactionBehaviorDef)
		interactionBehavior.grid(column=7, row=6)
		interactionBehavior.bind_all('i', self.interactionBehaviorDef)

		kukBehavior = ttk.Button(self.mainframe, text='k - kuk', command=self.kukBehaviorDef)
		kukBehavior.grid(column=8, row=0)
		kukBehavior.bind_all('k', self.kukBehaviorDef)

		sleepBehavior = ttk.Button(self.mainframe, text='l - sleep', command=self.sleepBehaviorDef)
		sleepBehavior.grid(column=8, row=1)
		sleepBehavior.bind_all('l', self.sleepBehaviorDef)

		quadBehavior = ttk.Button(self.mainframe, text='v - quad', command=self.quadBehaviorDef)
		quadBehavior.grid(column=8, row=2)
		quadBehavior.bind_all('v', self.quadBehaviorDef)

		upBehavior = ttk.Button(self.mainframe, text='a - up', command=self.upBehaviorDef)
		upBehavior.grid(column=8, row=3)
		upBehavior.bind_all('a', self.upBehaviorDef)

		forageBehavior = ttk.Button(self.mainframe, text='f - forage', command=self.forageBehaviorDef)
		forageBehavior.grid(column=8, row=4, sticky=(N))
		forageBehavior.bind_all('f', self.forageBehaviorDef)

		outBehavior = ttk.Button(self.mainframe, text='o - out of view', command=self.outBehaviorDef)
		outBehavior.grid(column=8, row=5, sticky=(N))
		outBehavior.bind_all('o', self.outBehaviorDef)

		splayedBehavior = ttk.Button(self.mainframe, text='w - splayed', command=self.splayedBehaviorDef)
		splayedBehavior.grid(column=8, row=6)
		splayedBehavior.bind_all('w', self.splayedBehaviorDef)
		
		self.root.protocol("WM_DELETE_WINDOW", self.exitWindow)
		self.root.mainloop()
		#for child in self.mainframe.winfo_children(): child.grid_configure(padx=5, pady=5)

	def exitWindow(self):
		self.exitq.put('close')
		self.root.destroy()

	def trial(self):
		self.hotKeySend({'Trial':self.trialTypes.get()})
		self.trialData()

	

	def treeBehaviorDef(self, _event=None):
		self.hotKeySend({'hotkey':'tree'})

	def quaaBehaviorDef(self, _event=None):
		self.hotKeySend({'hotkey':'Quaa'})

	def flickBehaviorDef(self, _event=None):
		self.hotKeySend({'hotkey':'flick'})

	def bipedBehaviorDef(self, _event=None):
		self.hotKeySend({'hotkey':'biped'})

	def descendBehaviorDef(self, _event=None):
		self.hotKeySend({'hotkey':'descend'})

	def runBehaviorDef(self, _event=None):
		self.hotKeySend({'hotkey':'run'})

	def backBehaviorDef(self, _event=None):
		self.hotKeySend({'hotkey':'back in view'})

	def groundBehaviorDef(self, _event=None):
		self.hotKeySend({'hotkey':'ground'})

	def othervocBehaviorDef(self, _event=None):
		self.hotKeySend({'hotkey':'other voc'})

	def flagBehaviorDef(self, _event=None):
		self.hotKeySend({'hotkey':'flag'})

	def gttBehaviorDef(self, _event=None):
		self.hotKeySend({'hotkey':'go to tree'})

	def groomBehaviorDef(self, _event=None):
		self.hotKeySend({'hotkey':'groom'})

	def stillBehaviorDef(self, _event=None):
		self.hotKeySend({'hotkey':'still'})

	def interactionBehaviorDef(self, _event=None):
		self.hotKeySend({'hotkey':'interaction'})

	def kukBehaviorDef(self, _event=None):
		self.hotKeySend({'hotkey':'kuk'})

	def sleepBehaviorDef(self, _event=None):
		self.hotKeySend({'hotkey':'sleep'})

	def quadBehaviorDef(self, _event=None):
		self.hotKeySend({'hotkey':'quad'})

	def upBehaviorDef(self, _event=None):
		self.hotKeySend({'hotkey':'up'})

	def forageBehaviorDef(self, _event=None):
		self.hotKeySend({'hotkey':'forage'})

	def outBehaviorDef(self, _event=None):
		self.hotKeySend({'hotkey':'out of view'})

	def splayedBehaviorDef(self, _event=None):
		self.hotKeySend({'hotkey':'splayed'})


	def endTrial(self):
		self.GUIq.put('End Trial')
		self.hotKeyFrame.destroy()
		
	
	def trialData(self, event):
		self.hotKeySend({'observerEntry':self.entry1.get(),'dateEntry':self.entry2.get(),'recorderEntry':self.recorderEntry.get(),
			'videoPersonEntry':self.videoPersonEntry.get(),'locationEntry':self.locationEntry.get()})
		
	#Takes whatever is entered in the notes field, appends it to the txt file, and updates the log in the GUI.
	#def saveNotes(self): 
		#if self.name == '' or self.date == '':
			#entryError = messagebox.showinfo(message='Please fill all required fields.', icon='error', title='Error')
			#entryError
			#return(0)
		#else:
			#with open('Squirrel Notes.txt', 'a') as sq:
				#sq.write('--------------------------------------------------')
				#sq.write('\n')
				#sq.write(self.name)
				#sq.write(':')
				#sq.write(self.date)
				#sq.write(': ')
				#sq.write(self.notes.get('1.0', END))
			#self.notes.delete('1.0', END)
			#with open('Squirrel Notes.txt') as sq:
				#self.log.configure(state='normal')
				#self.log.delete('1.0', END)
				#for line in sq:
					#self.lastEntry = line.rstrip()
					#self.log.insert(END, chars=self.lastEntry)
					#self.log.insert(END, '\n')
				#self.log.configure(state='disabled')

	#Reads the txt file, and sets the log box to show whatever is in it.
	#def refreshLog(self):
		#with open('Squirrel Notes.txt') as sq:
			#self.log.configure(state='normal')
			#self.log.delete('1.0', END)
			#for line in sq:
				#self.lastEntry = line.rstrip()
				#self.log.insert(END, chars=self.lastEntry)
				#self.log.insert(END, '\n')
			#self.log.configure(state='disabled')

	def writeComsLog(self, message):
		self.comsLog.configure(state='normal')
		self.comsLog.insert(END, chars=message)
		self.comsLog.configure(state='disabled')

	#Opens the advanced options menu which allows the user to control the squirrel outside of a trial.
	def advOptFrm(self):
		#set up advanced options
		self.advancedframe = Toplevel(self.mainframe)
		
		#Create label for Coms Log
		comsLogLabel = ttk.Label(self.advancedframe, text='Communication Log')
		comsLogLabel.grid(column=2, row=1)

		#Create Coms Log
		self.comsLog = Text(self.advancedframe, width=50, height=4, wrap='word', state='disabled')
		self.comsLog.grid(column=2, row=2)

		#Create Coms Entry Field
		entryVar = StringVar()
		self.comsEntry = ttk.Entry(self.advancedframe, textvariable=entryVar, width=66)
		self.comsEntry.grid(column=2, row=3)
		
		#Create Coms Entry Button
		comsButton = ttk.Button(self.advancedframe, text='Submit', command=self.manualCommand)
		comsButton.grid(column=3, row=3)

	def manualSend(self):
		entryVar = self.notes.get('1.0', END)
		self.writeComsLog(entryVar)
		print('Command entered: ', entryVar)
		self.GUIq.put({'manualnote':entryVar})
		self.lastEntry.delete(0, END)

	def manualCommand(self):
		entryVar = self.comsEntry.get()
		self.writeComsLog(entryVar)
		print('Command entered: ', entryVar)
		self.GUIq.put({'manualcommand':entryVar})
		self.comsEntry.delete(0, END)

	def hotKeySend(self, code):
		self.GUIq.put((code))