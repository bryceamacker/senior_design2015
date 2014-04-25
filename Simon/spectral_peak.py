from numpy import sin, linspace, pi, maximum
from pylab import plot, show, title, xlabel, ylabel, subplot, xlim
from scipy import fft, arange
from scipy.io.wavfile import read

def plotSpectrum(y,Fs):
 """
 Plots a Single-Sided Amplitude Spectrum of y(t)
 """
 n = len(y) # length of the signal
 k = arange(n)
 T = float(n)/float(Fs)
 frq = k/T # two sides frequency range line 13 zero division
 frq = frq[range(n/2)] # one side frequency range

 Y = fft(y)/n # fft computing and normalization
 Y = Y[range(n/2)]
 #xlim([0, 2000])
 max_y = max(abs(Y))
 max_x = frq[abs(Y).argmax()]
 print(max_x)
 plot(frq,abs(Y),'r') # plotting the spectrum
 xlabel('Freq (Hz)')
 ylabel('|Y(freq)|')
 
rate, data = read('Blue mono.wav')
ff = 5;   # frequency of the signal


Fs = rate;  # sampling rate
Ts = 1.0/Fs; # sampling interval
t = arange(0,1,1.0/len(data)) # time vector
t_y = arange(0, 1, (1.0/150.0))
y = sin(2*pi*ff*t_y)
subplot(2,1,1)
plot(t,data)
xlabel('Time')
ylabel('Amplitude')

# THE GOOD PART
subplot(2,1,2)
plotSpectrum(data,Fs)
show()