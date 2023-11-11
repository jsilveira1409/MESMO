import socket
import struct
import time
import io
import picamera

cmds = {
    "TakePicture":     b'\x00\x00\x00\x01',
    "SendPicture":     b'\x00\x00\x00\x02',
    "SendStatus":      b'\x00\x00\x00\x03',
    "NONE":            b'\x00\x00\x00\x99'
}

camera = picamera.PiCamera()
camera.rotation = 180

def decode_cmd(data, conn):
    if data == cmds["SendPicture"]:
        print("Sending Picture")
        stream = io.BytesIO()
        camera.capture(stream, format='jpeg')
        stream.seek(0)  # Go to the start of the stream
        return_data = stream.read()
        stream.close()
    elif data == cmds["TakePicture"]:
        print("Taking Picture")
        return_data = "Taking Picture".encode()
        print("Picture taken")
    elif data == cmds["SendStatus"]:
        print("Sending Current Status")
        return_data = "Payload Status: Doing great".encode()
    
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
