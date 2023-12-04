import socket
import struct
import time
import io
import cv2
import threading
import queue

cmds = {
    "SendPicture":     b'\x00\x00\x00\x02',
    "SendStatus":      b'\x00\x00\x00\x03',
    "NONE":            b'\x00\x00\x00\x99'
}
current_cmd = cmds["NONE"]



TCP_IP = '0.0.0.0'
TCP_PORT = 50001

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 1048576)
s.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, 1048576)
s.bind((TCP_IP, TCP_PORT))
s.listen(1)
conn, addr = s.accept()



def process():
    global current_cmd
    while True:
        if current_cmd == cmds["SendPicture"]:
            
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
                if start_frame == b'\xde\xad\xbe\xef':
                    data_length = conn.recv(4)
                    data = conn.recv(int.from_bytes(data_length, "big"))
                    current_cmd = data
        finally:
            conn.close()
            print("Connection closed")


if __name__ == "__main__":
    send_queue = queue.Queue()
    recv_queue = queue.Queue()
    recv_thread = threading.Thread(target=receiveTCP)
    processing = threading.Thread(target=process)

    recv_thread.start()
    processing.start()

    recv_thread.join()    
    processing.join()

