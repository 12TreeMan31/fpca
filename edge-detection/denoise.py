import numpy as np
import cv2 as cv
from matplotlib import pyplot as plt
 

img = cv.imread('tests/IMG_5674.jpg')
 
dst = cv.fastNlMeansDenoisingColored(img,None,10,10,7,21)
 
cv.imwrite("tests/IMG_5674_NR.jpg", dst)

plt.subplot(121),plt.imshow(img)
plt.subplot(122),plt.imshow(dst)

plt.show()