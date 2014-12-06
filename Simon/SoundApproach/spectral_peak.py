import numpy
from numpy.fft import *
import os
from pylab import plot, subplot, show
from scipy import arange
from scipy import signal
from scipy.io.wavfile import read, write
from record import record

def detectColor(y, rate):
	"""
	Plots a Single-Sided Amplitude Spectrum of y(t)
	"""
	Fs = rate;  # sampling rate
	Ts = 1.0 / Fs; # sampling interval
	#t = arange(0,1, 1.0 / len(data)) # time vector
	y_n = numpy.array(y)
	n = len(y) # length of the signal
	k = arange(n)
	T = float(n)/float(Fs)
	frq = k/T # two sides frequency range line 13 zero division
	frq = frq[range(n/2)] # one side frequency range

	Y = fft(y)/n # fft computing and normalization
	Y = Y[range(n/2)]
	Y = abs(Y)

	Y[300:len(Y)].fill(0)
	frq[300:len(frq)].fill(0)
	max_x = frq[Y.argmax()]
	print(max_x)
	index_multiplier = Y.argmax() / max_x
	color = ""
	if (1074 - 20 <= max_x <= 1074 + 20):
		color = "Blue"
	if (967 - 20 <= max_x <= 967 + 20):
		color = "Red"
	if (1240 <= max_x <= 1320):
		# cut out the "Green" section, check if the yellow spike is there
		frq[index_multiplier*1200:index_multiplier*1400].fill(0)
		Y[index_multiplier*1200:index_multiplier*1400].fill(0)
		max_x = frq[Y.argmax()]
		if (768 - 20 <= max_x <= 768 + 20):
			color = "Yellow"
		else:
			color = "Green"

	# subplot(2,1,1)
	# plot(frq, Y, 'r')
	# show()

	return color

if __name__ == '__main__':
	record(time=.5)
	rate, data = read("output.wav")
	numOfTones = 1
	toneLengthSec = .232 # Approximate length of each tone
	toneLengthIndex = float(rate) * toneLengthSec
	timeBetweenTones = .165
	skipToNextTone = float(rate) * timeBetweenTones
	length = float(len(data)) / float(rate) # length of file
	# Take the amount of time it takes for the first tone (.293 s)
	# and slice the data array to start at that time (the beginnig of the red tone)
	beginningIndex = float(rate) * .293 
	data = data[beginningIndex:]
	toneList = []
	i = 0
	toneFile = []
	for tones in range(numOfTones):
		toneList.append(data[:toneLengthIndex])
		data = data[toneLengthIndex + skipToNextTone + i*.005:]
		print(detectColor(toneList[tones], rate))