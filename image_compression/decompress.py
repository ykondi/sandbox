#!/usr/bin/python

import sys
from math import ceil
from math import log
from rgb import RGB

# Set constants

if len(sys.argv) < 2:
	# Input stream is assumed to be a slice-compressed image
	print "P6"
	# Read and extract dimensions, box size
	dimensions = raw_input().split()
	width = int(dimensions[0]) 
	height = int(dimensions[1])
	box_side = int(dimensions[2])
	print width, height
	print "255"

	# Read first row (border case)
	samples = sys.stdin.read((width/(box_side-1) + 1) * 3)
	last_row = [RGB([ord(k) for k in samples[j:j+3]]) for j in range(0, len(samples), 3)]

	for i in range(1, height):
		# Get relevant samples in the row
		samples = sys.stdin.read((width/(box_side-1) + 1) * 3)
		row = [RGB([ord(k) for k in samples[j:j+3]]) for j in range(0, len(samples), 3)]

		# chunk stores a full row of slices, and is flushed after
		# interpolating completely (by default to stdout)
		null = [0, 0, 0]
		chunk = [[RGB(null) for k in range(0, width*(box_side-1)+1)] for j in range(0, box_side)]

		top_vertices = [last_row[0], last_row[1]]
		bottom_vertices = [row[0], row[1]]
		for j in range(2, len(row)):
			offset = (j-2) * (box_side - 1)
			if j%2 == 0:
				# Update chunk every two iterations
				chunk[0][offset] = top_vertices[0]
				chunk[0][offset+box_side-1] = top_vertices[1]
				chunk[box_side-1][offset] = bottom_vertices[0]
				chunk[box_side-1][offset+box_side-1] = bottom_vertices[1]

			# Interpolate diagonals and sides
			primary_diagonal = (top_vertices[0] - bottom_vertices[1])
			secondary_diagonal = (top_vertices[1] - bottom_vertices[0])
			left_side = (top_vertices[0] - bottom_vertices[0])
			right_side = (top_vertices[1] - bottom_vertices[1])
			top_side = (top_vertices[0] - top_vertices[1])
			bottom_side = (bottom_vertices[0] - bottom_vertices[1])
			'''
			for k in range(1, box_side-1):
				chunk[k][offset+box_side+k-1] = bottom_vertices[1] + primary_diagonal / k
				chunk[box_side-k-1][offset+box_side+k-1] = top_vertices[1] + secondary_diagonal / k
				chunk[k][offset] = bottom_vertices[0] + left_side / k
				chunk[k][offset+box_side-1] = bottom_vertices[1] + right_side / k
				chunk[0][offset+k] = top_vertices[1] + top_side / k
				chunk[box_side-1][offset+k] = bottom_vertices[1] + bottom_side / k
			'''
			
			# Update top and bottom vertices
			top_vertices[0] = top_vertices[1]
			top_vertices[1] = last_row[j]
			bottom_vertices[0] = bottom_vertices[1]
			bottom_vertices[1] = row[j]

		# Flush chunk to stdout
		for j in range(0, box_side):
			for k in range(0, width):
				#if chunk[j][k].blue>= 256:
				#	print "Indices: ", i, j, k
				chunk[j][k].write(sys)

		last_row = row
