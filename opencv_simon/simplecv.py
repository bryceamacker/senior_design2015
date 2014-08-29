from SimpleCV import *
# Initialize the camera
cam = Camera()
template = Image("temp2.png")
a = None
# Loop to continuously get images
while True:
    # Get Image from camera
	img = cam.getImage()
	fs = img.findKeypointMatch(template)
	if fs is not None:
		print fs
		fs.crop()
	img.show()