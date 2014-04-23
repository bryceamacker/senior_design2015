Motivation:

> Note that the Gaussian window transform magnitude is precisely a parabola on a dB scale. As a result, quadratic spectral peak interpolation is *exact* under the Gaussian window. Of course, we must somehow remove the infinitely long tails of the Gaussian window in practice, but this does not cause much deviation from a parabola
- [Quadratic Interpolation of Spectral Peaks](https://ccrma.stanford.edu/~jos/sasp/Quadratic_Interpolation_Spectral_Peaks.html)

Apparently this produces error if the peak is not exactly at the edge or center of a bin.  Interpolating by zero-padding before the FFT does not produce this kind of error, but is more computationally expensive.  So a good trade-off is to do some zero-padding interpolation and then follow with parabolic interpolation:

> While we could choose our zero-padding factor large enough to yield any desired degree of accuracy in peak frequency measurements, it is more efficient in practice to combine zero-padding with parabolic interpolation (or some other simple, low-order interpolator). In such hybrid schemes, the zero-padding is simply chosen large enough so that the bias due to parabolic interpolation is negligible.
- [Bias of Parabolic Peak Interpolation ](https://ccrma.stanford.edu/~jos/sasp/Bias_Parabolic_Peak_Interpolation.html)

Here's [a Matlab function that does the same thing](https://ccrma.stanford.edu/~jos/sasp/Matlab_Parabolic_Peak_Interpolation.html).

Actually this function could be done with the `polyfit()` function, instead, but it requires more steps:

	f = [2, 3, 1, 6, 4, 2, 3, 1]

	In [8]: parabolic(f, argmax(f))
	Out[8]: (3.2142857142857144, 6.1607142857142856)

	In [9]: a, b, c = polyfit([2,3,4],[1,6,4],2)
	In [10]: x = -0.5*b/a

	In [11]: x
	Out[11]: 3.2142857142857695

	In [12]: a*x**2 + b*x + c
	Out[12]: 6.1607142857143025

`parabolic()` could be updated to use this, and then it will work for non-uniform sampled input as well.

Alternative estimators with even lower error: [How to Interpolate the Peak Location of a DFT or FFT if the Frequency of Interest is Between Bins](http://www.dspguru.com/dsp/howtos/how-to-interpolate-fft-peak)
