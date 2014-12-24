# Wrapper for the RGB space coordinates

class RGB:
	red = 0.0
	green = 0.0
	blue = 0.0


	def __init__(self):
		self.red = self.green = self.blue = 1.0

	def __init__(self, colour_list):
		self.red = colour_list[0] * 1.0
		self.green = colour_list[1] * 1.0
		self.blue = colour_list[2] * 1.0

	def __sub__(self, other):
		ret_list = []
		ret_list.append(self.red - other.red)
		ret_list.append(self.green - other.green)
		ret_list.append(self.blue - other.blue)
		return RGB(ret_list)

	def __add__(self, other):
		ret_list = []
		ret_list.append(self.red + other.red)
		ret_list.append(self.green + other.green)
		ret_list.append(self.blue + other.blue)
		return RGB(ret_list)

	def __mul__(self, const):
		ret_list = []
		ret_list.append(self.red * const)
		ret_list.append(self.green * const)
		ret_list.append(self.blue * const)
		return RGB(ret_list)

	def __div__(self, const):
		ret_list = []
		ret_list.append(self.red / const)
		ret_list.append(self.green / const)
		ret_list.append(self.blue / const)
		return RGB(ret_list)

	def write(self, sys):
		red = int(self.red - 0.5)
		green = int(self.green - 0.5)
		blue = int(self.blue - 0.5)
		print red, green, blue
		sys.stdout.write(chr(red)+chr(green)+chr(blue))
