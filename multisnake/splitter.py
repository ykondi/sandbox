import Tkinter as tk
from Tkinter import *
import threading
import time

# Global variables:

maxThreads = 100	# Upper bound on number of threads and hence numver of splits
threads = 0		# Count of how many ids have been blocked
trailLength = 10		# Set the units of the trail
trailCounter = -1
keepMoving = False
hudreference = {}	# Dictionary for HUD reference

# Variables to capture state of lines:
lines = []
activeLine = 0
lineState = []	# Items of the form indices of the line, [direction, strength]
pixels = []

# Reference Lists

colours = ['red', 'green', 'blue', 'orange', 'yellow']

bindings = [['w', 'a', 's', 'd'],	# Convention: [up, left, down, right]
		['y','g','h','j']]

movements = ['w', 'a', 's', 'd']
directions = {'w':0,'a':1,'s':2,'d':3}


def changePath(line, direction):
	global lineState
	if lineState[line][0] == direction:		# If the same direction, increment strength of turn
		lineState[line][1] = lineState[line][1] + 1
	else:
		lineState[line] = [direction, lineState[line][1]]
	
def updatePositions():
	global w
	global trailCounter
	global lineState
	global pixels
	global keepMoving
	global maxThreads
	while 1:
		while keepMoving:
			trailCounter = (trailCounter + 1) % maxThreads
			for line in lines:
				lastPixel = w.coords( pixels[line][(trailCounter-1) % maxThreads] )
				if lineState[line][0] == 'w':	# Move up
					w.coords( pixels[line][trailCounter], (lastPixel[2], lastPixel[3], lastPixel[2], lastPixel[3]-lineState[line][1]))
				elif lineState[line][0] == 's':	# Move down
					w.coords( pixels[line][trailCounter], (lastPixel[2], lastPixel[3], lastPixel[2], lastPixel[3]+lineState[line][1]))
				elif lineState[line][0] == 'a':	# Move left
					w.coords( pixels[line][trailCounter], (lastPixel[2], lastPixel[3], lastPixel[2]-lineState[line][1], lastPixel[3]))
				elif lineState[line][0] == 'd':	# Move right
					w.coords( pixels[line][trailCounter], (lastPixel[2], lastPixel[3], lastPixel[2]+lineState[line][1], lastPixel[3]))
				# Gradually reduce to straight movement
				if lineState[line][1] > 1:
					lineState[line][1] = lineState[line][1] - 0.5
			time.sleep(0.05)


def appendTrail():
	global w
	global hud
	global lineState
	global lines
	global pixels
	global hudreference
	global hud_selector
	lineState.append(['d', 1])	# Initialize movement of direction to right with minimal strength
	lines.append(len(lines))	# Set new ID number
	pixels.append([w.create_line(0, 0, 1, 1) for i in range(0, maxThreads)])	# Create pixels on canvas
	# Set colour of this thread
	for i in pixels[len(pixels)-1]:
		w.itemconfig(i, fill=colours[(len(pixels)-1)%maxThreads])
	# Initialize Heads Up Display (HUD):
	if len(lines) == 1:
		print 'selector made'
		hud_selector = hud.create_rectangle(50, 25, 150, 75)
		hud.itemconfig(hud_selector, fill='black')
	temp = hudreference['line'+str(len(lines)-1)] = hud.create_rectangle(0, (len(lines)-1)*100, 200, (len(lines)-1)*100 + 99)
	hud.itemconfig(temp, fill=colours[(len(pixels)-1)%maxThreads])

def updateLineSelection(last):
	global activeLine
	global hud
	global hud_selector
	global hudreference
	hud.itemconfig(hudreference['line'+str(activeLine)], width = 20)
	hud.itemconfig(hudreference['line'+str(last)], width = 1)


# Event Handlers:
def KeyPressHandler(event):
	global keepMoving
	key = event.char
	if key in movements:
		changePath(activeLine, key)
	elif key == 'n':
		appendTrail()
	elif key == 'x':
		keepMoving = not keepMoving

def MouseWheelHandler(event):
	global activeLine
	if event.num == 2:
		appendTrail()
	if len(lines) > 0:
		last = activeLine
		if event.num == 4:
			activeLine = (activeLine - 1) % len(lines)
		if event.num == 5:
			activeLine = (activeLine + 1) % len(lines)
		updateLineSelection(last)

def main():
	global w
	global hud
	root = tk.Tk()
	root.geometry('800x600')
	root.bind('<KeyPress>', KeyPressHandler)
	root.bind('<Button-4>', MouseWheelHandler)
	root.bind('<Button-5>', MouseWheelHandler)
	root.bind('<Button-2>', MouseWheelHandler)
	w = Canvas(root, width=600, height=600, bg='white')
	w.place(x = 0 , y = 0)
	hud = Canvas(root, width=200, height=600)
	hud.place(x = 600 , y = 0)
	updateThread = threading.Thread( target = updatePositions )
	updateThread.daemon = True
	updateThread.start()
	root.mainloop()

main()
