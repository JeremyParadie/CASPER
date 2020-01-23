import csv

class Logger:

	def __init__(self, csvFile):
		self.log = open(csvFile, 'w')
		self.writer = csv.writer(self.log, delimiter=',')

	def write(self, time, event):
		self.writer.writerow([time, event])
