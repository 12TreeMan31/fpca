import cv2

cv2.namedWindow("I See You!!!")
vc = cv2.VideoCapture(0)

if vc.isOpened(): # try to get the first frame
    rval, frame = vc.read()
else:
    rval = False

while rval:
    key = cv2.waitKey(20)
    if key == 27: # exit on ESC
        break
    rval, frame = vc.read()

    #img = cv2.fastNlMeansDenoisingColored(frame, None, 10, 10, 7, 21)
    img = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    # img = cv2.resize(img, None, img, fx=0.4, fy=0.4)
    edges = cv2.Canny(img, 200, 700 , apertureSize=3)
    
    cv2.imshow("I See You!!!", edges)

cv2.destroyWindow("preview")
vc.release()