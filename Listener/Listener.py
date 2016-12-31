import socket

UDP_IP = "10.0.0.1"
UDP_PORT = 33333

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

print ('Listening...')

while True:
	data, addr = sock.recvfrom(1024)
	print ("received message:", data)
	
