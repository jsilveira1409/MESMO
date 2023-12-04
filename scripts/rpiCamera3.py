import socket
import struct
import time
import io
from ultralytics import YOLO
import cv2
import threading
import queue

model = YOLO('yolov8n-seg.pt')  # load an official model
cmds = {
    "SendPicture":     b'\x00\x00\x00\x02',
    "SendStatus":      b'\x00\x00\x00\x03',
    "Follow"    :      b'\x00\x00\x00\x06',
    "NONE":            b'\x00\x00\x00\x99'
}
current_cmd = cmds["NONE"]


print("Starting TCP Server")
TCP_IP = '0.0.0.0'
TCP_PORT = 50001

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 1048576)
s.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, 1048576)
#s.setblocking(False)
s.bind((TCP_IP, TCP_PORT))
s.listen(1)
conn, addr = s.accept()



def process():
    global current_cmd
    print("Processing")
    while True:
        
        if current_cmd == cmds["Follow"]:
            frame_width, frame_height = 640, 480
            camera = cv2.VideoCapture(0)
            if camera.isOpened() == False:
                print("Error opening video stream or file")
            camera.set(cv2.CAP_PROP_FRAME_WIDTH, frame_width)
            camera.set(cv2.CAP_PROP_FRAME_HEIGHT, frame_height)

            while True:
                rval, image = camera.read()
                print("flag1 ")
                if rval == False: 
                    print("No image")
                    break
                print("flag2")
                
                results = model(image)
                print("flag3")
                detected_persons = results[0].boxes
                print("flag4")
                highest_confidence = 0
                best_box = None
                print("flag5")
                for obj in detected_persons:
                    if int(obj.cls) == 0 and obj.confidence > highest_confidence:  # Class ID 0 for persons
                        highest_confidence = obj.confidence
                        best_box = obj.xyxyn[0]

                if best_box is not None:
                    x1, y1, x2, y2 = best_box
                    x1, y1, x2, y2 = int(x1 * frame_width), int(y1 * frame_height), int(x2 * frame_width), int(y2 * frame_height)
                    center_x = struct.pack ('>i' ,int((x1 + x2) / 2))
                    center_y = struct.pack ('>i' ,int((y1 + y2) / 2))
                    data_label = "center"
                    data = data_label.encode() + center_x + center_y
                    print(f"Center : ({(x1 + x2) /2},{(y1 + y2) /2})")
                    if len(data) > 6:
                        data = b'\xde\xad\xbe\xef' + struct.pack('>I', len(data)) + data
                        conn.send(data)
                time.sleep(0.1)

            camera.release()

        elif current_cmd == cmds["SendPicture"]:
            print("Sending picture")
            camera = cv2.VideoCapture(0)
            if camera.isOpened() == False:
                print("Error opening video stream or file")
            rval, image = camera.read()
            if rval == False: 
                print("No image")
                break
            data = cv2.imencode('.jpg', image)[1].tobytes()
            data = b'\xde\xad\xbe\xef' + struct.pack('>I', len(data)) + data
            conn.send(data)
            current_cmd = cmds["NONE"]
            camera.release()

        elif current_cmd == cmds["SendStatus"]:
            data = "Payload Status: Doing great".encode()
            data = b'\xde\xad\xbe\xef' + struct.pack('>I', len(data)) + data
            conn.send(data)
            current_cmd = cmds["NONE"]
        time.sleep(0.1)
    
def receiveTCP():
    global current_cmd
    while True:
        print("Waiting for connection")
        try:
            print(f'Connection address: {addr}')
            s.setblocking(False)
            while True:               
                start_frame = conn.recv(4)
                print(start_frame)
                if start_frame == b'\xde\xad\xbe\xef':
                    data_length = conn.recv(4)
                    data = conn.recv(int.from_bytes(data_length, "big"))
                    print(f"Received {data}")
                    current_cmd = data
        finally:
            conn.close()
            print("Connection closed")




if __name__ == "__main__":
    send_queue = queue.Queue()
    recv_queue = queue.Queue()
    recv_thread = threading.Thread(target=receiveTCP)
    #send_thread = threading.Thread(target=sendTCP, args=(send_queue,))
    processing = threading.Thread(target=process)

    recv_thread.start()
    #send_thread.start()
    processing.start()

    recv_thread.join()
    #send_thread.join()
    processing.join()

