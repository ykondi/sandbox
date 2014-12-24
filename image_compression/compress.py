#!/usr/bin/python

import sys
from math import ceil
from math import log

# Set constants
box_side = 4

if len(sys.argv) < 2:
	# Input stream is assumed to be a PNM6 image
	# Discard the first line
	raw_input()

	# Read and extract dimensions
	dimensions = raw_input().split()
	width = int(dimensions[0]) 
	height = int(dimensions[1])
	print width, height, box_side

	# Get maximum value for a colour. Currently useless
	max_val = log(int(raw_input())+1, 2)

	for i in range(0, int(height / (box_side))):
		# Get relevant samples in the row
		samples = sys.stdin.read(width * 3)
		for j in range(0, width*3, (box_side-1)*3):
			sys.stdout.write(samples[j] + samples[j+1] + samples[j+2])
		# Discard the next box_side - 1 lines
		sys.stdin.read(width * (box_side-1) * 3)
