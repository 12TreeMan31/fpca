# https://stackoverflow.com/questions/43053179/how-to-detect-change-in-colours-in-the-image-below
import cv2

# load the image
img = cv2.imread("results/IMG_5671/sat.jpg")
# convert to grayscale
img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
# resize for the visualization purposes
img = cv2.resize(img, None, img, fx=0.4, fy=0.4)
# find edges with Canny
edges = cv2.Canny(img, 200, 700 , apertureSize=3)
# show and save the result
cv2.imshow("edges", edges)
cv2.imshow("edges", img)
cv2.imshow("edges", edges)
cv2.imshow("edges", img)
cv2.waitKey(0)
cv2.imwrite("result.png", edges)
