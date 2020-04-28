# this is a file of robot specific comamands
# these are customized commands that make using a specific robot easier

# a madeup action for a swquire to do
def skee(time, angle):
    command(time, str(angle)) # str(angle) puts quotes around whatever angle they input if it doens't alearyd have quotes
