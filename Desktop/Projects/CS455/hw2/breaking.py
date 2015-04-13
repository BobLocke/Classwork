
# Ryan "Bob" Dean
# CS 455 - Programming homework 2

from socket import *
from collections import OrderedDict
import sys, threading

#can't import any http related library here.
#please double check with me if you do

def recieve(socket):
    returnMessage = ""
    if socket:
        while 1:
            try:
                socket.settimeout(3)
                temp = socket.recv(1024)
                if temp:
                    returnMessage += temp
                else: 
                    break
            except:
                break
        return returnMessage
    else:
        print "socket does not exist"
        sys.exit(1)


def parse(source, key):

    #print "source: {} \nkey: {}".format(source, key)
    parseMessage = source.split(" ")
    for x in range(0, len(parseMessage)):
        if key in parseMessage[x]:
            return parseMessage[x+1]
    #print "Parse error on {}: key {} not found".format(source, key)
    return ""

def checkCache(clientRequest):
    if cache:
        host = parse(clientRequest, "Host")
        getRequest = parse(clientRequest, "GET")
        print fileRequest
        fileExist = "false"
    else:
        print "???"

def sendRequest(clientRequest, port):
    try: 
        host = parse(clientRequest, "Host:")
        print host
        serverSocket = socket(AF_INET,SOCK_STREAM)
        serverSocket.connect((host, port))
        serverSocket.send(clientRequest)
        return recieve(serverSocket)
    except BaseException, e:
        print e

"""
Handler function for new threads created by requests from client.

"""
def handleRequest(clientSocket, addr):
    clientSocket.settimeout(10)
    try:
        #print "attempting to recieve message\n"
        clientRequest = recieve(clientSocket).replace("keep-alive", "close")
        if clientRequest == None:
            print "Client Request empty!!!!!!!!\n\n\n\n\n\n"
        #print clientRequest
        #print "passed recieve\n"
        serverResponse = sendRequest(clientRequest, 80)

        try:
            clientSocket.send(serverResponse)     
        except TypeError:
            #print "clientRequest: {}".format(clientRequest)
            #print "serverResponse: {}".format(serverResponse)
            return
        #checkCache(clientRequest)
        return
    except timeout:
        print "Message from client not recieved.\n"


def main():
    if len(sys.argv) != 2:
        print 'Usage : "python HttpProxy.py server_port \n"'
        sys.exit(2)
    serverPort =  int(sys.argv[1])
    proxySocket = socket(AF_INET, SOCK_STREAM)
# Create a server socket, bind it to a port and start listening        
    proxySocket.bind(("", serverPort))
    proxySocket.listen(5)
    print "proxy currently listening on port {}".format(serverPort)
    while 1:
        clientSocket, addr = proxySocket.accept()
        thread = threading.Thread(target = handleRequest, args = (clientSocket, addr))
        thread.daemon = True
        thread.start()

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print "Keyboard Interrupt"
        sys.exit(1)

#Prog2: clean up
