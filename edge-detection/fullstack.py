import cv2
import socket

server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server.bind(("0.0.0.0", 43210))

vc = cv2.VideoCapture(0)
cv2.namedWindow("Real")


if vc.isOpened(): # try to get the first frame
    rval, frame = vc.read()
else:
    rval = False

while rval: 
    key = cv2.waitKey(20)
    if key == 27: # exit on ESC
        break
    rval, frame = vc.read()
    img = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    edges = cv2.Canny(img, 100, 200, 3)
    cv2.imshow("preview", edges)
    bytes_sent = 0
    while bytes_sent < edges.size:
        x = slice(bytes_sent, bytes_sent + 1024, 1)
        server.sendto(b"gah", ("0.0.0.0", 54321))
        bytes_sent += 1024


cv2.destroyWindow("preview")
vc.release()