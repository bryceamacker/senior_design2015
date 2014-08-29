import numpy as np
import cv2

def nothing(x):
    pass

cap = cv2.VideoCapture(0)

# Create a black image, a window
#img = np.zeros((300,512,3), np.uint8)
cv2.namedWindow('raw')
cv2.namedWindow('blue')
cv2.namedWindow('red')
cv2.namedWindow('green')
cv2.namedWindow('yellow')

while(True):
    # Capture frame-by-frame
    ret, frame = cap.read()

    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    lower_blue = np.array([100, 50, 50], dtype=np.uint8)
    upper_blue = np.array([130,255,255], dtype=np.uint8)
    b_thr = cv2.inRange(hsv, lower_blue, upper_blue)

    lower_red1 = np.array([170, 50, 50], dtype=np.uint8)
    upper_red1 = np.array([179,255,255], dtype=np.uint8)
    lower_red2 = np.array([0, 50, 50], dtype=np.uint8)
    upper_red2 = np.array([10,255,255], dtype=np.uint8)
    r_thr1 = cv2.inRange(hsv, lower_red1, upper_red1)
    r_thr2 = cv2.inRange(hsv, lower_red2, upper_red2)
    r_thr = cv2.bitwise_or(r_thr1, r_thr2)

    lower_green = np.array([50, 50, 40], dtype=np.uint8)
    upper_green = np.array([65,250,250], dtype=np.uint8)
    g_thr = cv2.inRange(hsv, lower_green, upper_green)

    lower_yellow = np.array([25, 50, 50], dtype=np.uint8)
    upper_yellow = np.array([35,250,250], dtype=np.uint8)
    y_thr = cv2.inRange(hsv, lower_yellow, upper_yellow)

    # Display the resulting frame
    cv2.imshow('raw',frame)
    cv2.imshow('blue',b_thr)
    cv2.imshow('red',r_thr)
    cv2.imshow('green',g_thr)
    cv2.imshow('yellow',y_thr)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()