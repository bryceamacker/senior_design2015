senior_design2015
=================

### API
API files for all the different hardware used on the bot. This includes photo cells, servos, and seven-segment-displays. See each individual file for more detail. The servo API includes functionality for almost any use of servos.

### Card
Functionality to pick up the card using the game arm.

### Etch
Functionality to draw 'IEEE' using to continuous rotation servos attached to the platform.

### GameArm
Functionality for various movements with the game arm.

### lib
All the PIC24 library files provided by the MSU ECE department.

### Navigation
Includes API files for using QTR-8RC sensor arrays and using Pololu 37D motors with encoders. This folder also includes a robust line following system using both sensor arrays and the motors with encoders. Uses a PID to follow straight lines and enters static routines with 'hard' right angles are encountered. This code is specifically tailored to SECON 2015 but can be modified to other uses.

### PicCode
This is the code that is actually compiled into hex files that will then be put on PICs. There is a couple of files that are used together for running SECON 2015 along with a few controller files used for testing.

### Platform
Functionality for various movements with the platform that holds the Etch-a-Sketch servos and the Rubiks servos.

### Rubiks
Functionality to twist the Rubiks Cube using the game arm and the platform.

### secon_site
The entire MSU SECON 2015 website. Built upon Playbook.

### Simon
Functionality to record Simon lights using photo transistors, then to hit the buttons using servos. This code uses a timer to stop Simon play after 15 seconds.

### Strategy
Simple little timer to add up points for a given run.
