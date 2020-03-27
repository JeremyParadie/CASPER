"""
The Actual User interface for the CASPER Robot. 
Controls UI appearence, Importing of buttons, and importing of data fields.

Author: Per Van Dyke
"""



import sys
import json
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *

row_length = 5
input_height = 30
UI_active = False

# Class containing all code for the main window
class MainWindow(QMainWindow):

    def __init__(self, *args, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)
        
        info_dialog = UserWindow(self)
        info_dialog.exec_()

        self.SetupWindow()
        
        
        
    def SetupWindow(self):
        self.setWindowTitle("CASPER")
        
        self.mainColumn = QVBoxLayout()

        upperButtons = QHBoxLayout()
        upperButtons.setSpacing(500)
        self.mainColumn.addLayout(upperButtons)

        consoleLayout = QVBoxLayout()
        self.mainColumn.addLayout(consoleLayout)


        start = QPushButton("Start Trial")
        start.setMinimumWidth(50)
        start.pressed.connect(self.StartTrial)
        upperButtons.addWidget(start)

        load = QPushButton("Load Trial Type")
        load.setMinimumWidth(50)
        load.pressed.connect(self.LoadJSON)
        upperButtons.addWidget(load)

        self.consoleLog = QTextEdit()
        #consoleLog.addScrollBarWidget()
        self.consoleLog.setReadOnly(True)
        self.consoleLog.setMinimumHeight(300)
        consoleLayout.addWidget(self.consoleLog)

        self.console = QTextEdit()
        self.console.setMaximumHeight(input_height)
        #self.console.
        consoleLayout.addWidget(self.console)



        widget = QWidget()
        widget.setLayout(self.mainColumn)
        self.setCentralWidget(widget)
        
    def LoadJSON(self):
        print("Load JSON here when implemented")
        selectedJSON = QFileDialog.getOpenFileName(self, caption = "Select JSON", directory = "/source/jsons", filter = "JSON Files (*.json *.txt)")
        print(selectedJSON[0])
        with open(selectedJSON[0]) as json_file:
            buttons = json.load(json_file)

        button_holder_list = []
        for _ in range(((len(buttons["button_list"])//row_length)+1)):
            button_holder_list.append(QHBoxLayout())

        using_buttons_list = []
        holder_list_index = 0
        button_index = 0
        print(len(buttons["button_list"]))
        for button in buttons["button_list"]:
            button_widget = QPushButton(button)
            button_holder_list[holder_list_index].addWidget(button_widget)
            if button_holder_list[holder_list_index].count() >= 5:
                using_buttons_list.append(button_holder_list[holder_list_index])
                holder_list_index = holder_list_index+1
            elif button_index == len(buttons["button_list"])-1:
                using_buttons_list.append(button_holder_list[holder_list_index])
            button_index = button_index+1

        print(len(using_buttons_list))
        for i in range(len(using_buttons_list)):
            self.mainColumn.addLayout(using_buttons_list[i])
                



    def StartTrial(self):
        print("Start trial when implemented")

    def ConsoleCommand(self):
        command = self.console.copy()
        self.console.clear()
        self.consoleLog.append(command)



       



class UserWindow(QDialog):

    def __init__(self, *args, **kwargs):
        super(UserWindow, self).__init__(*args, **kwargs)

        # Name Window
        self.setWindowTitle("CASPER")
        
        # Create the main column
        vLayout = QVBoxLayout()
        
        # Create the rows
        hLayout1 = QHBoxLayout()
        vLayout.addLayout(hLayout1)
        
        hLayout2 = QHBoxLayout()
        vLayout.addLayout(hLayout2)

        hLayout3 = QHBoxLayout()
        vLayout.addLayout(hLayout3)

        # Create the various buttons and assign them to proper rows
        observer = QTextEdit()
        observer.setFixedHeight(input_height)
        observer.setPlaceholderText("Observer")
        hLayout1.addWidget(observer)

        recorder = QTextEdit()
        recorder.setFixedHeight(input_height)
        recorder.setPlaceholderText("Recorder")
        hLayout1.addWidget(recorder)

        videoPerson = QTextEdit()
        videoPerson.setFixedHeight(input_height)
        videoPerson.setPlaceholderText("Video Person")
        hLayout1.addWidget(videoPerson)

        date = QDateEdit()
        hLayout2.addWidget(date)

        location = QTextEdit()
        location.setFixedHeight(input_height)
        location.setPlaceholderText("Location")
        hLayout2.addWidget(location)

        submit = QPushButton("Submit")
        submit.pressed.connect(self.SubmitData)
        hLayout3.addWidget(submit)

        self.setLayout(vLayout)

    def SubmitData(self):
        self.close()

def loop(que):

    while True:
        if not UI_active:
            app = QApplication(sys.argv)

            window = MainWindow()
            window.show()

            app.exec_()

        #if the que's first message is adressed to main (may implememnt total loop through later)
        if que[0].startswith('UI'):
            # fetch the message from the top of the que
            addr, retaddr, args  = que.pop(0)
            # parse the adress into just the command by spitiling and disposing
            # of the first item
            cmd = addr.split('.')[1:]



if __name__ == "__main__":
    app = QApplication(sys.argv)

    window = MainWindow()
    window.show()

    app.exec_()
