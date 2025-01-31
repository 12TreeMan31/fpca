import cv2
import numpy as np
import matplotlib.pyplot as plt

# Open the picture
img = cv2.imread('IMG_5521_SAT.JPG', cv2.IMREAD_GRAYSCALE)

# Apply the Sobel magic
sobel_x = cv2.Sobel(img, cv2.CV_64F, 1, 0, ksize=5)
sobel_y = cv2.Sobel(img, cv2.CV_64F, 0, 1, ksize=5)

# Merge the x and y gradient's power
sobel = np.hypot(sobel_x, sobel_y)

# Normalize between 0 and 255
sobel = (sobel / sobel.max()) * 255

# Show the before and after pictures
plt.subplot(1, 2, 1), plt.imshow(img, cmap='gray')
plt.title('Before'), plt.xticks([]), plt.yticks([])

plt.subplot(1, 2, 2), plt.imshow(sobel, cmap='gray')
plt.title('After'), plt.xticks([]), plt.yticks([])

plt.show()