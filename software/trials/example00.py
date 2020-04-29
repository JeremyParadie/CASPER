# and example file to show what one could do ina  trial

# phase sets the current phase of the trial at a certain time
# these could be put anywhere in the file, some may prefer them
#   right above where the phase starts
# input the time first then the phase number
phase(0, 1)
phase(60, 2)
phase(120, 3)

# end sets the time when the trial should end
# input the time it should end
end(180)

#phase 2 (there is no phase 1)
swing_tail(60, 3, 0, 60)
skree(61.5)
