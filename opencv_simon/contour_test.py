import numpy as np
import cv2

cap = cv2.VideoCapture(0)

cv2.namedWindow('frame')
lower = np.array([0, 65, 65], dtype=np.uint8)
upper = np.array([179,255,255], dtype=np.uint8)
buttons = []

while(True):
    # Capture frame-by-frame
    ret, frame = cap.read()
    frame = cv2.blur(frame, (3,3))

    edges = cv2.Canny(frame,50,150)
    contours, hierarchy = cv2.findContours(edges,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    if len(contours) > 4:
	    by_area = sorted(contours, key=cv2.contourArea, reverse = True)
	    buttons = by_area[0:8]
	    """areas = map(cv2.contourArea, by_area)
	    max_var = (float('Inf'), 1)
	    for i in xrange(1, len(areas) - 2):
	    	variance = np.var(areas[i-1:i+3])
	    	if variance < max_var[0]:
	    		max_var = (variance, i)
	    buttons = by_area[max_var[1] - 1: max_var[1] + 3]"""



    cv2.drawContours(frame, buttons, -1, (0,255,0), 3)



    cv2.imshow('frame', frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()