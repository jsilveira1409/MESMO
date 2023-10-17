import socket
import time
import struct
import socket
import struct
import time
import picamera
import picamera.array
import cv2
import numpy as np



cmds = {
    "TakePicture":     b'\x00\x00\x00\x01',
    "SendPicture":    b'\x00\x00\x00\x02',
    "SendStatus":     b'\x00\x00\x00\x03',
    "NONE":             b'\x00\x00\x00\x99'
}

current_cmd = cmds["NONE"]
img_data = None
camera = picamera.PiCamera()
camera.rotation = 180

def decode_cmd(data):
    global img, img_data, cmds, take_picture, video_capture
    return_data = None
    if data == cmds["SendPicture"]:
        print("Sending Picture")
        return_data = img_data
    elif data == cmds["TakePicture"]:
        print("Taking Picture")
        return_data = "Taking Picture"
        output = picamera.array.PiRGBArray(camera)
        camera.capture(output, 'rgb')
        img_data = output.array

        # Convert numpy array to JPEG in memory using OpenCV
        retval, buffer = cv2.imencode('.jpg', img_data)
        return_data = buffer.tobytes()
    elif data == cmds["SendStatus"]:
        print("Sending Current Status")
        return_data = "Payload Status: Doing great"
        return_data = return_data.encode()
    
    return return_data


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

    while 1:
        print("Waiting connexion")
        conn, addr = s.accept()
        try:
            print(f'Connection address: {addr} \n')
            while True:                
                start_frame = conn.recv(4)
                if start_frame == b'\xde\xad\xbe\xef':
                    data_lentgh = conn.recv(4)
                    data = conn.recv(int.from_bytes(data_lentgh, "big"))
                    print(f"Received {data}")
                    return_data = decode_cmd(data)
                    data = b'\xde\xad\xbe\xef' + struct.pack('>I', len(return_data)) + return_data
                    print(f"Sending {len(return_data)}")
                    conn.send(data)
                time.sleep(0.2)                
        finally:
            conn.close()
            print("Connection closed")
        

if __name__ == "__main__":
    runTCP()
