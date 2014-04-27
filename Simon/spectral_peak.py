import numpy
from scipy import arange, fft
from scipy.io.wavfile import read, write

def detectColor(y, rate):
	"""
	Plots a Single-Sided Amplitude Spectrum of y(t)
	"""
	Fs = rate;  # sampling rate
	#Ts = 1.0 / Fs; # sampling interval
	#t = arange(0,1, 1.0 / len(data)) # time vector

	n = len(y) # length of the signal
	k = arange(n)
	T = float(n)/float(Fs)
	frq = k/T # two sides frequency range line 13 zero division
	frq = frq[range(n/2)] # one side frequency range

	Y = fft(y)/n # fft computing and normalization
	Y = Y[range(n/2)]
	max_y = max(abs(Y))
	max_x = frq[abs(Y).argmax()]
	print(max_x)
	color = ""
	if (1074 - 60 <= max_x <= 1074 + 60):
		color = "Blue"
	if (967 - 60 <= max_x <= 967 + 60):
		color = "Red"
	if (1257 - 60 <= max_x <= 1257 + 60):
		color = "Green"
	if (777 - 60 <= max_x <= 777 + 60):
		color = "Yellow"

	return color

if __name__ == '__main__':
	rate, data = read("red blue yellow green red.wav")
	numOfTones = 5
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