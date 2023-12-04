import multiprocessing
from pyomyo import Myo, emg_mode
import os
import struct
import socket


print("Starting TCP Server for MYO")
TCP_IP = '0.0.0.0'
TCP_PORT = 50002

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 1048576)
s.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, 1048576)
#s.setblocking(False)
s.bind((TCP_IP, TCP_PORT))
s.listen(1)
conn, addr = s.accept()


def cls():
    os.system('cls' if os.name=='nt' else 'clear')

# ------------ Myo Setup ---------------
q = multiprocessing.Queue()

def worker(q):
	m = Myo(mode=emg_mode.FILTERED)
	m.connect()
	
	def add_to_queue(pose):
		q.put(pose)

	m.add_pose_handler(add_to_queue)
	
	# Orange logo and bar LEDs
	m.set_leds([128, 128, 0], [128, 128, 0])
	# Vibrate to know we connected okay
	m.vibrate(1)
	
	"""worker function"""
	while True:
		m.run()
	#print("Worker Stopped")


# -------- Main Program Loop -----------
if __name__ == "__main__":
	p = multiprocessing.Process(target=worker, args=(q,))
	p.start()
#	print("Starting TCP Server")

	try:
		while True:
			while not(q.empty()):
				data = q.get().name
				print(data)
				
				conn.send(b'\xde\xad\xbe\xef' + struct.pack('>I', len(data)) + data.encode())
	finally:
		conn.close()
#		print("Connection closed")
