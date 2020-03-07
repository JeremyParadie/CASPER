#this is a file of default commands and proceedures the "basic" robot can perform
# send_cmd it the function used to send a command to the robot at a specific time
#this example is coded as if a squire is attached
#   the tail angle is attached to servo 2

def set_tail_angle(time, angle):
    angle = int(angle)
    send_cmd(time, f"Servo 2 setAngle {angle}")

def swing_tail(time, duration, start_angle, final_angle, interval = 1/10):
    for interval_time in range(0, duration, interval):
        angle = (interval_time/duration)*(start_angle-final_angle) + start_angle
        set_tail_angle(time + interval_time, angle)

# using skree as a standing for some other noise name
def skree(time):
    send_cmd(time, "Audio play skree00.mp3")
