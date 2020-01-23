import Logger
import os


class Trial:

    def __init__(self, name="Default_Trial", trial=["tailSetAngle(50, 1)", "tailSetAngle(20, 1.5)", "tailSetAngle(50, 2)", "tailSetAngle(20, 2.5)", "tailSetAngle(50, 3)", "tailSetAngle(20, 3.5)", "tailSetAngle(50, 4)", "tailSetAngle(20, 4.5)", "tailSetAngle(50, 5)", "tailSetAngle(20, 5.5)", "tailSetAngle(-20, 6)", "audioPlay('track004.mp3', 7)", "audioStop(30)"], boolean=False):
        self.name = name
        self.execStack = list(
            map(lambda n: "self.lastOutput = Trial." + n, trial))
        self.debugMode = boolean
        self.lastOutput = None
        i = 0
        csvExists = os.path.isfile("logs/" + name + str(i) + ".csv")
        while (csvExists == True):
            i = i + 1
            csvExists = os.path.isfile("logs/" + name + str(i) + ".csv")
        self.logger = Logger.Logger("logs/" + name + str(i) + ".csv")

    def getName(self):
        return self.name

    def isDone(self):
        # checks to see if the trial is over
        if len(self.execStack) == 0:
            return (True)
        return (False)

    def popNextLine(self):
        # runs next command in execStack
        if self.isDone():
            return
        exec(self.execStack.pop(0))
        return self.lastOutput

    def popAllLines(self):
        # runs everything in the execStack
        output = []
        for i in range(len(self.execStack)):
            output.append(Trial.popNextLine(self))
        return output

    def tailGetAngle(time):
        # Note this is comm sendable
        return (time, "Robot Servo 0 getAngle")

    def tailSetAngle(angle, time):
        # Note this is comm sendable
        return (time, "Robot Servo 0 setAngle " + str(angle))

    def tailGoLimp(time):
        # Not this is com sendable
        return (time, "Robot Servo 0 goLimp")

    def coverGetAngle(time):
        # Note this is comm sendable
        return (time, "Robot Servo 1 getAngle")

    def coverSetAngle(angle, time):
        # Note this is comm sendable
        return (time, "Robot Servo 1 setAngle " + str(angle))

    def tailGoLimp(time):
        # Not this is com sendable
        return (time, "Robot Servo 1 goLimp")

    def audioSetVolume(percent, time):
        # Note this is comm sendable
        return (time, "Robot Audio setVolume " + str(percent))

    def audioPlay(fileName, time):
        # Note this is comm sendable
        return (time, "Robot Audio play " + fileName)

    def audioIsPlaying(time):
        # Note this is comm sendable
        return (time, "Robot Audio isPlaying")

    def audioPause(time):
        # Note this is comm sendable
        return (time, "Robot Audio pause")

    def audioResume(time):
        # Note this is comm sendable
        return (time, "Robot Audio resume")

    def audioIsPaused(time):
        # Note this is comm sendable
        return (time, "Robot Audio isPaused")

    def audioStop(time):
        # Note this is comm sendable
        return (time, "Robot Audio stop")

    def audioIsStopped(time):
        # Note this is comm sendable
        return (time, "Robot Audio isStopped")

    def audioGetFileName(time):
        # Note this is comm sendable
        return (time, "Robot Audio getFileName")

    def audioSetVolume(percent, time):
        # Note this is comm sendable
        return (time, "Robot Audio setVolume " + str(percent) + "%")

    def tailMotion(self, positionList, startTime, rate):
        pass
        # This doesn't work
        # Note this is not comm sendable.
        # Adds the list of comm sendable functions that would create the intended motion to the stack then does runNextLine

    def advancedTailMotion(WIP):
        pass
        # Note this is not comm sendable
        # Adds the list of comm sendable functions that would create the intended motion to the stack then does runNextLine
