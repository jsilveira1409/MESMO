import socket
import struct
import time
import io
from ultralytics import YOLO
import cv2
import threading


model = YOLO('yolov8n-seg.pt')  # load an official model

cmds = {
    "SendPicture":     b'\x00\x00\x00\x02',
    "SendStatus":      b'\x00\x00\x00\x03',
    "Predict"   :      b'\x00\x00\x00\x04',
    "Analyse"   :      b'\x00\x00\x00\x05',
    "Follow"    :      b'\x00\x00\x00\x06',
    "NONE":            b'\x00\x00\x00\x99'
}

camera = cv2.VideoCapture(0)
camera.rotation = 180

frame_width, frame_height = 640, 480

def decode_cmd(data, conn):
    global camera, rawCapture, model
    if data == cmds["SendPicture"]:
        print("Sending Picture")
        stream = io.BytesIO()
        camera.capture(stream, format='jpeg')
        stream.seek(0)  
        return_data = stream.read()
        stream.close()

    
    elif data == cmds["SendStatus"]:
        print("Sending Current Status")
        return_data = "Payload Status: Doing great".encode()

    elif data == cmds["PredictStream"]:
        print("Predicting")
        return_data = model.predict("test.jpg", show=False, save=True, save_conf=True, retina_masks=True, boxes=True)

    elif data == cmds["Analyse"]:
        print("Analysing")
        return_data = "Analysing".encode()

    elif data == cmds["Follow"]:
        print("Following")
        while True:

            rval, image = camera.read()
            if rval == False:
                break
            
            results = model(image)
            detected_persons = results[0].boxes
            highest_confidence = 0
            best_box = None

            for obj in detected_persons:
                if int(obj.cls) == 0 and obj.confidence > highest_confidence:  # Class ID 0 for persons
                    highest_confidence = obj.confidence
                    best_box = obj.xyxyn[0]

            # Draw box for the person with the highest confidence
            if best_box is not None:
                x1, y1, x2, y2 = best_box
                # Convert normalized coordinates to pixel coordinates
                x1, y1, x2, y2 = int(x1 * frame_width), int(y1 * frame_height), int(x2 * frame_width), int(y2 * frame_height)
                center_x = struct.pack ('>i' ,int((x1 + x2) / 2))
                center_y = struct.pack ('>i' ,int((y1 + y2) / 2))
                data_label = "center"
                data = data_label.encode() + center_x + center_y
                data = b'\xde\xad\xbe\xef' + struct.pack('>I', len(data)) + data
                conn.send(struct.pack('>I', int(data)))


            # if receive data from client, break
            if conn.recv(4) == b'\xde\xad\xbe\xef':
                
                break
        
    # Send the data with header and size
    if return_data:
        data = b'\xde\xad\xbe\xef' + struct.pack('>I', len(return_data)) + return_data
        conn.send(data)
    else:
        conn.send(b'\xde\xad\xbe\xef' + struct.pack('>I', 0))

def runTCP():
    print("Starting TCP Server")
    TCP_IP = '0.0.0.0'
    TCP_PORT = 50001

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 1048576)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, 1048576)
    s.bind((TCP_IP, TCP_PORT))
    s.listen(1)

    while True:
        print("Waiting for connection")
        conn, addr = s.accept()
        try:
            print(f'Connection address: {addr}')
            while True:                
                start_frame = conn.recv(4)
                if start_frame == b'\xde\xad\xbe\xef':
                    data_length = conn.recv(4)
                    data = conn.recv(int.from_bytes(data_length, "big"))
                    print(f"Received {data}")
                    decode_cmd(data, conn)
        finally:
            conn.close()
            print("Connection closed")


if __name__ == "__main__":
    runTCP()
