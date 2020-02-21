import sys
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *

# Subclass QMainWindow to customise your application's main window
class MainWindow(QMainWindow):

    def __init__(self, *args, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)
        
        infoDialog = UserWindow(self)
        infoDialog.exec_()

        self.SetupWindow()
        
        
        
    def SetupWindow(self):
        self.setWindowTitle("CASPER")
        
        mainColumn = QVBoxLayout()

        upperButtons = QHBoxLayout()
        upperButtons.setSpacing(500)
        mainColumn.addLayout(upperButtons)

        consoleLayout = QVBoxLayout()
        mainColumn.addLayout(consoleLayout)


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
        self.console.setMaximumHeight(25)
        #self.console.
        consoleLayout.addWidget(self.console)



        widget = QWidget()
        widget.setLayout(mainColumn)
        self.setCentralWidget(widget)
        
    def LoadJSON(self):
        print("Load JSON here when implemented")
        selectedJSON = QFileDialog.getOpenFileName(self, caption = "Select JSON", directory = "/source/jsons", filter = "JSON Files (*.json *.txt)")

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
        observer.setFixedHeight(25)
        observer.setPlaceholderText("Observer")
        hLayout1.addWidget(observer)

        recorder = QTextEdit()
        recorder.setFixedHeight(25)
        recorder.setPlaceholderText("Recorder")
        hLayout1.addWidget(recorder)

        videoPerson = QTextEdit()
        videoPerson.setFixedHeight(25)
        videoPerson.setPlaceholderText("Video Person")
        hLayout1.addWidget(videoPerson)

        date = QDateEdit()
        hLayout2.addWidget(date)

        location = QTextEdit()
        location.setFixedHeight(25)
        location.setPlaceholderText("Location")
        hLayout2.addWidget(location)

        submit = QPushButton("Submit")
        submit.pressed.connect(self.SubmitData)
        hLayout3.addWidget(submit)

        self.setLayout(vLayout)

    def SubmitData(self):
        self.close()


if __name__ == "__main__":
    app = QApplication(sys.argv)

    window = MainWindow()
    window.show()

    app.exec_()
