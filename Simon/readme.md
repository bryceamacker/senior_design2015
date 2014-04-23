A few simple frequency estimation methods in Python

These are the methods that everyone recommends when someone asks about 
frequency estimation or pitch detection.  Such as here: 

- [Music - How do you analyse the fundamental frequency of a PCM or WAV sample](http://stackoverflow.com/questions/65268/music-how-do-you-analyse-the-fundamental-frequency-of-a-pcm-or-wac-sample/)
- [CCRMA Pitch detection methods review](https://ccrma.stanford.edu/~pdelac/154/m154paper.htm)
- [Pitch Detection Algorithms (Middleton)](http://cnx.org/content/m11714/latest/)

So these are my attempts at implementation.  Initially I was trying to measure the frequency of long sine waves with high accuracy (to indirectly measure clock frequency), then added methods for other types of signals later.

None of them work well in all situations, these are "offline", not real-time, and I am sure there are much better methods "in the literature", but here is some sample code for the simple methods at least.

### Count zero-crossings, divide average period by time to get frequency
* Works well for long low-noise sines, square, triangle, etc.
* Supposedly this is how cheap guitar tuners work
* Using interpolation to find a "truer" zero-crossing gives better accuracy
* Pro: Fast
* Pro: Accurate (increasing with signal length)
* Con: Doesn't work if there are multiple zero crossings per cycle, low-frequency baseline shift, noise, etc.

### Do FFT and find the peak
* Using parabolic interpolation to find a truer peak gives better accuracy
* Accuracy also increases with signal/FFT length
* Con: Doesn't find the right value if harmonics are stronger than fundamental, which is common.  Better method would try to be smarter about identifying the fundamental, like template matching using the ["two-way mismatch" (TWM) algorithm](http://ems.music.uiuc.edu/beaucham/papers/JASA.04.94.pdf).
* Pro: Accurate, usually even more so than zero crossing counter (1000.000004 Hz for 1000 Hz, for instance).  Due to [parabolic interpolation being a very good fit](https://ccrma.stanford.edu/~jos/sasp/Quadratic_Interpolation_Spectral_Peaks.html) for windowed log FFT peaks?

### Do autocorrelation and find the peak
* Pro: Best method for finding the true fundamental of any repetitive wave, even with weak or missing fundamental (finds GCD of all harmonics present)
* Con: Inaccurate result if waveform isn't perfectly repeating, like inharmonic musical instruments (piano, guitar, ...), however:
 * Pro: This inaccurate result more closely matches the pitch that humans perceive :)
* Con: Not as accurate as other methods for precise measurement of sine waves
* Con: This implementation has trouble with finding the true peak

### Calculate harmonic product spectrum and find the peak
* Pro: Good at finding the true fundamental even if weak or missing

See also https://github.com/endolith/waveform-analyzer/blob/master/frequency_estimator.py, which is mostly the same thing, maybe more up-to-date.  I need to keep them both in sync with each other or delete one.