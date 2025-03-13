import cv2 
import numpy as np 
  
  
# Loading the image  
img = cv2.imread('Images/result.png') 
  
# preprocess the image 
gray_img = cv2.cvtColor(img ,  
                        cv2.COLOR_BGR2GRAY) 
  
# Applying 7x7 Gaussian Blur 
# blurred = cv2.GaussianBlur(gray_img, (7, 7), 0) 
# Applying threshold 
threshold = cv2.threshold(gray_img, 0, 255, 
    cv2.THRESH_BINARY_INV | cv2.THRESH_OTSU)[1]  
  
# Apply the Component analysis function 
analysis = cv2.connectedComponentsWithStats(gray_img,  
                                            8,  
                                            cv2.CV_32S) 
(totalLabels, label_ids, values, centroid) = analysis 
  
# Initialize a new image to 
# store all the output components 
output = np.zeros(gray_img.shape, dtype="uint8") 
  
# Loop through each component 
for i in range(1, totalLabels): 
    
      # Area of the component 
    area = values[i, cv2.CC_STAT_AREA]  
      
    if (area > 140) and (area < 400): 
        # Create a new image for bounding boxes 
        new_img=img.copy() 
          
        # Now extract the coordinate points 
        x1 = values[i, cv2.CC_STAT_LEFT] 
        y1 = values[i, cv2.CC_STAT_TOP] 
        w = values[i, cv2.CC_STAT_WIDTH] 
        h = values[i, cv2.CC_STAT_HEIGHT] 
          
        # Coordinate of the bounding box 
        pt1 = (x1, y1) 
        pt2 = (x1+ w, y1+ h) 
        (X, Y) = centroid[i] 
          
        # Bounding boxes for each component 
        cv2.rectangle(new_img,pt1,pt2, 
                      (0, 255, 0), 3) 
        cv2.circle(new_img, (int(X), 
                             int(Y)),  
                   4, (0, 0, 255), -1) 
  
        # Create a new array to show individual component 
        component = np.zeros(gray_img.shape, dtype="uint8") 
        componentMask = (label_ids == i).astype("uint8") * 255
  
        # Apply the mask using the bitwise operator 
        component = cv2.bitwise_or(component,componentMask) 
        output = cv2.bitwise_or(output, componentMask) 
          
        # Show the final images 
        cv2.imshow("Image", new_img) 
        cv2.imshow("Individual Component", component) 
        cv2.imshow("Filtered Components", output) 
        cv2.waitKey(0)
