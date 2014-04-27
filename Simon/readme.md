###Peak Frequency estimation in Python

In the file 'spectral_peak.py', I am doing 2 things:
1. Recording audio for a specified amount of time from the system's default audio input
2. Doing a FFT on the resulting wav file, and finding the highest magnitude frequency and seeing if they match with the known frequencies of Simon's buttons

You can specify how long to record, and how many tones to expect (equal to however many buttons you need to press that round)
This method of spectrum peak analyses works, but there is something called Enhanced Autocorrelation, a feature in the popular open source audio software Audacity.
"Enhanced Autocorrelation" is a homegrown implementation of the Autocorrelation algorithm by the Audacity team, which produces great results when used to analyse the sounds of Simon. I will include
some screenshots and comparisons of a spectrum analyses to the results of a enhanced autocorrelation analyses on the team website http://sites.google.com/site/msstatesecon2015/home/.
Since Audacity is open source, I've been able to find the actual C++ code that implements this feature, and am trying to port it over to this python script. Unfortunately, its pretty confusing and complex.
Running this on my beefy machine, the analyses is done in no time at all. I can't say for sure how times will compare when we move to a smaller platform like an RPi, but I think they will be ok.
If the times turn out to be too long, there is a GPU accelerated FFT library for the RPi we could take advantage of: http://www.raspberrypi.org/accelerating-fourier-transforms-using-the-gpu/


###Included Sounds
If you don't have a Simon handy, I've included sound files of all the buttons, along with a file that contains all of them, labelled. I recommend using Audacity, and taking a look at 
'red blue yellow green red.aup' (Creative name, I know). It has each sound labelled so you know which one is which, along with their peak frequency per spectral analyses. And if you want to experiment 
with them yourself, highlight one of the sounds, go to Analyze->Plot Spectrum, and you'll see what spectral_peak is doing behind the scenes. spectral_peak is basically implementing the functionality of 
hovering your mouse of a peak in the Audacity window and it telling you the nearest peak at the bottom. Change 'Algorithm' to Enhanced Autocorrelation to see what that looks like and how it could benefit 
detection. All sounds being recorded have a sample rate of 44100, and spectral_peak needs files to be in mono. 

###Portability
Since Python is such a portable language, and we're using some of the most common libraries for it, port this to either a raspberry pi or beagleboard should be a breeze. There will have to be some slight 
modification to the record.py script, but nothing too drastic. This currently works fine under Windows 7 with a usb webcam as the main audio source. You will have to install quite a few libraries to 
get up and running, however.


###I would recommend looking here for ways to start improving the detection:
https://gist.github.com/endolith/255291#file-readme-md
