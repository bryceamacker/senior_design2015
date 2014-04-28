senior_design2015
=================

### capture.c
I was playing with the idea of using image recognition for Simon button detection, lining up with the toys, etc. 

This is a file that will take video from a usb web cam (this is the one I used: http://www.logitech.com/en-us/product/hd-webcam-c310). I've modified it to add support for the MJPG codec standard, which is what the c310 webcam uses. 

This is very early work I haven't expanded on, but might be useful in the future. Check out webcam_config.txt for more instructions on how its used. ffmpeg is used to get us an actual video. If we wanted to start image recongition, we'd want to just grab a single image. 

### Simon
This is the work done so far for playing Simon says. 

Currently, we can detect all 4 buttons with the same webcam, using their different audio tones.

### serial_monitor.py

This is a small proof of concept of how we could communicate with an Arduino over the serial ports of the Beaglebone. This isn't up to date, and will be modified later. 
