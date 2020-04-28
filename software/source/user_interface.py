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
from pathlib import Path

row_length = 5
input_height = 30

#ignore that this was added (i assume the "css" will cover this)
button_width = 70
textfield_width = 300

# _message_queue needs to be overwritten with
#   the correct queue to the use below function
_message_queue = None

def send_message(addr, retaddr=None, args=()):
    global _message_queue
    _message_queue.append((addr, retaddr, args))

# Class containing all code for the main window
class MainWindow(QMainWindow):

    def __init__(self, *args, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)

        self.defaults = {}
        self.robot_fields = {}

        self.SetupWindow()



    def SetupWindow(self):
        self.setWindowTitle("CASPER")

        self.mainColumn = QVBoxLayout()

        #Create the toolbar and its widgets
        toolbar = QToolBar()
        toolbar.setMovable(False)
        self.addToolBar(toolbar)

        #toolbar_spacer = QWidget()
        #toolbar_spacer.setSizePolicy(QSizePolicy())

        battery_status = QLabel("Battery: ___%")
        toolbar.addWidget(battery_status)

        toolbar.addSeparator()

        signal_strength = QLabel("Signal: Placeholder")
        toolbar.addWidget(signal_strength)


        self.left_pane = QVBoxLayout()
        self.right_pane = QVBoxLayout()
        self.pane_wrapper = QHBoxLayout()
        self.mainColumn.addLayout(self.pane_wrapper)
        self.pane_wrapper.addLayout(self.left_pane)
        self.pane_wrapper.addLayout(self.right_pane)

        # Constant fields for left_pane

        # Robot Selection
        robot_label_box = QVBoxLayout()
        robot_button_box = QHBoxLayout()

        robot_label = QLabel("Robot")

        self.robot_path = QTextEdit()
        self.robot_path.setReadOnly(True)
        self.robot_path.setMinimumHeight(input_height)
        self.robot_path.setMaximumHeight(input_height)
        self.robot_path.setMinimumWidth(textfield_width)
        self.robot_path.setMaximumWidth(textfield_width)

        robot_select = QPushButton("Select")
        robot_select.pressed.connect(self.SelectRobot)
        robot_select.setMinimumWidth(button_width)
        robot_select.setMaximumWidth(button_width)

        robot_label_box.addWidget(robot_label)
        robot_label_box.addLayout(robot_button_box)
        robot_button_box.addWidget(self.robot_path)
        robot_button_box.addWidget(robot_select)

        self.left_pane.addLayout(robot_label_box)

        # Subject Selection
        subject_label_box = QVBoxLayout()
        subject_button_box = QHBoxLayout()

        subject_label = QLabel("Subject")

        self.subject_path = QTextEdit()
        self.subject_path.setReadOnly(True)
        self.subject_path.setMinimumHeight(input_height)
        self.subject_path.setMaximumHeight(input_height)
        self.subject_path.setMinimumWidth(textfield_width)
        self.subject_path.setMaximumWidth(textfield_width)

        subject_select = QPushButton("Select")
        subject_select.pressed.connect(self.SelectSubject)
        subject_select.setMinimumWidth(button_width)
        subject_select.setMaximumWidth(button_width)

        subject_label_box.addWidget(subject_label)
        subject_label_box.addLayout(subject_button_box)
        subject_button_box.addWidget(self.subject_path)
        subject_button_box.addWidget(subject_select)

        self.left_pane.addLayout(subject_label_box)

        # Trial Selection
        trial_label_box = QVBoxLayout()
        trial_button_box = QHBoxLayout()

        trial_label = QLabel("Trial")

        self.trial_path = QTextEdit()
        self.trial_path.setReadOnly(True)
        self.trial_path.setMinimumHeight(input_height)
        self.trial_path.setMaximumHeight(input_height)
        self.trial_path.setMinimumWidth(textfield_width)
        self.trial_path.setMaximumWidth(textfield_width)

        trial_select = QPushButton("Select")
        trial_select.pressed.connect(self.SelectTrial)
        trial_select.setMinimumWidth(button_width)
        trial_select.setMaximumWidth(button_width)

        trial_label_box.addWidget(trial_label)
        trial_label_box.addLayout(trial_button_box)
        trial_button_box.addWidget(self.trial_path)
        trial_button_box.addWidget(trial_select)

        self.left_pane.addLayout(trial_label_box)

        self.procedural_pane = QVBoxLayout()
        self.left_pane.addLayout(self.procedural_pane)



        phase_status = QLabel("Phase: X")
        phase_status.setStyleSheet("QLabel { min-width: 100% }")
        self.right_pane.addWidget(phase_status)

        time_display = QLabel("Time: 00:00:00")
        self.right_pane.addWidget(time_display)



        consoleLayout = QVBoxLayout()
        self.right_pane.addLayout(consoleLayout)


        start = QPushButton("Start Trial")
        start.setMinimumWidth(50)
        start.pressed.connect(self.StartTrial)
<<<<<<< HEAD
        self.left_pane.addWidget(start)
=======
>>>>>>> 76a3e9abfc84505650d614159a5126feee0aa845
        #upperButtons.addWidget(start)

        #load = QPushButton("Load Trial Type")
        #load.setMinimumWidth(50)
        #load.pressed.connect(self.LoadTargetJSON)
        #upperButtons.addWidget(load)

        self.consoleLog = QTextEdit()
        #consoleLog.addScrollBarWidget()
        self.consoleLog.setReadOnly(True)
        self.consoleLog.setMinimumHeight(300)
        self.consoleLog.setMinimumWidth(500)
        consoleLayout.addWidget(self.consoleLog)

        self.console = QTextEdit()
        self.console.setMaximumHeight(input_height)
        #self.console.
        consoleLayout.addWidget(self.console)



        widget = QWidget()
        widget.setLayout(self.mainColumn)
        self.setCentralWidget(widget)


    def SelectRobot(self):
        selectedJSON = QFileDialog.getOpenFileName(self, caption = "Select JSON", directory = "jsons/", filter = "JSON Files (*.json *.txt)")
        print(selectedJSON)
        if selectedJSON[0] == "" or selectedJSON[0] is None:
                return

        self.robot_path.setText(selectedJSON[0])
        robot_file = open(selectedJSON[0], "r+")
        self.robot_fields = json.load(robot_file)
        robot_file.close()

    def SelectSubject(self):
        selectedJSON = QFileDialog.getOpenFileName(self, caption = "Select JSON", directory = "jsons/", filter = "JSON Files (*.json *.txt)")
        if selectedJSON[0] == "" or selectedJSON[0] is None:
                return

        self.subject_path.setText(selectedJSON[0])
        subject_file = open(selectedJSON[0], "r+")
        self.subject_fields = json.load(subject_file)
        subject_file.close()

    def SelectTrial(self):
        selectedJSON = QFileDialog.getOpenFileName(self, caption = "Select JSON", directory = "jsons/", filter = "JSON Files (*.json *.txt)")
        if selectedJSON[0] == "" or selectedJSON[0] is None:
                return

        self.trial_path.setText(selectedJSON[0])
        trial_file = open(selectedJSON[0], "r+")
        self.trial_fields = json.load(trial_file)
        trial_file.close()

    def LoadDefaults(self):
        defaults_file_path = "./defaults.json"
        defaults_file = open(defaults_file_path)
        self.defaults = json.load(defaults_file)
        defaults_file.close()

    def AssignDefaults(self):
        # Will parse through the selected robots JSON and pick out the fields to use.
        self.LoadDefaults()
        self.procedural_fields[field] = [label, entry_field]

        self.procedural_pane.addLayout(label_box)

    def LoadSubjectJSON(self, manual = True):
        if manual == True:
            selectedJSON = QFileDialog.getOpenFileName(self, caption = "Select JSON", directory = "jsons/", filter = "JSON Files (*.json *.txt)")
            if selectedJSON[0] == "" or selectedJSON[0] is None:
                return
            print(selectedJSON[0])
            with open(selectedJSON[0], "r") as json_file:
                buttons = json.load(json_file)
        else:
            with open(self.defaults["subject_path"], "r") as subject_json:
                buttons = json.load(subject_json)


        button_holder_list = []
        for _ in range(((len(buttons["button_list"])//row_length)+1)):
            button_holder_list.append(QHBoxLayout())

        using_buttons_list = []
        holder_list_index = 0
        button_index = 0
        print(len(buttons["button_list"]))

        # the key is the letter that a user sohuld be able to press on
        #   the keyboard insread of clicking the button
        for key, message in buttons["button_list"].items():
            button = "{}-{}".format(key, message)
            button_widget = QPushButton(button)
            button_holder_list[holder_list_index].addWidget(button_widget)
            if button_holder_list[holder_list_index].count() >= 5:
                using_buttons_list.append(button_holder_list[holder_list_index])
                holder_list_index = holder_list_index+1
            elif button_index == len(buttons["button_list"])-1:
                using_buttons_list.append(button_holder_list[holder_list_index])
            button_index += 1

        print(len(using_buttons_list))
        for i in range(len(using_buttons_list)):
            self.right_pane.addLayout(using_buttons_list[i])


    def StartTrial(self):
        print("Start trial when implemented")
        send_message('trial.start', None, ('trialfile_path_here', 'subject_path_here', 'robot_path_here'))

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


def qt_loop():
    global window

    app = QApplication(sys.argv)

    window = MainWindow()
    window.show()

    app.exec_()

def loop(que):
    """
    commands (proposed)
    partyl implemented
    ui.status.battery_percentage -> sets it to the first item im args
    ui.status.signal -> only sets the signal, should be passed a single string to display

    needs implememntation:
    ui.switch_pane -> switches to right pane active if args[0] is True,
        otherwise switched to the left pane.
    ui.timer.start -> resets and starts the time
    ui.timer.stop -> stops the timer
    ui.phase -> sets the phase (a number) message to arg[0]
    """
    global window
    global _message_queue
    _message_queue = que

    while True:
        #if the que's first message is adressed to user_intterface (may implememnt total loop through later)
        if len(que) and que[0][0].startswith('ui'):
            # fetch the message from the top of the que
            addr, retaddr, args  = que.pop(0)
            responce = None
            # parse the adress into just the command by spliting and disposing
            # of the first item
            cmd = addr.split('.')[1:]
            while len(cmd) < 3:
                cmd.append('')
            cmd_type = cmd[0]

            #loop example see commands speficief above
            if cmd_type == 'status':
                if len(args):
                    if cmd[1] == 'battery_percentage':
                        pass# window.battery_percentage = args[0]
                    elif cmd[1] == 'signal':
                        pass# window.signal = args[0]
                else:
                    pass # raise error
            #other command types

            if (retaddr is not None) and (respond is not None):
                #send a message, not asking for responce
                send_message(retaddr, None, responce)




if __name__ == "__main__":
    app = QApplication(sys.argv)

    window = MainWindow()
    window.show()

    app.exec_()
