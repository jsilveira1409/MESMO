import socket
import struct
import time
import io
from ultralytics import YOLO
import cv2 

model = YOLO('yolov5nu.pt')  
#img = cv2.imread("test.jpg")

# Perform detection using YOLO
#results = model(img
#cv2.namedWindow("preview")
vc = cv2.VideoCapture(0)

if vc.isOpened():
    rval, frame = vc.read()
else:
    rval = False

while rval:
    rval, frame = vc.read()
    if rval == False:
        print("No image")
        break
    print("flag2")
    results = model(frame)
    print("flag3")
    frame_height, frame_width = frame.shape[:2]
    detected_persons = results[0].boxes  # xyxy format for bounding boxes

    # Find the person with the highest confidence
    highest_confidence = 0
    best_box = None
    for obj in detected_persons:
        if int(obj.cls) == 0 and obj.conf > highest_confidence:  # Class ID 0 for persons
            highest_confidence = obj.conf
            best_box = obj.xyxyn[0]

    # Draw box for the person with the highest confidence
    if best_box is not None:
        x1, y1, x2, y2 = best_box
        # Convert normalized coordinates to pixel coordinates
        x1, y1, x2, y2 = int(x1 * frame_width), int(y1 * frame_height), int(x2 * frame_width), int(y2 * frame_height)
        cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)




    if key == 27: # exit on ESC
        break
    #cv2.imshow("preview", frame)
vc.release()
#cv2.destroyWindow("preview")

