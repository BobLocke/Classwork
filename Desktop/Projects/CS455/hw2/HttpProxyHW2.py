
# Ryan "Bob" Dean
# CS 455 - Programming homework 2

from socket import *
from collections import OrderedDict
import sys, threading, time

cache = OrderedDict()


"""
recieves until buffer is empty
"""

def receive(socket):
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


"""
removes the oldest half of the cache if it gets too big
"""

def manageCache():
    if len(cache) > 50:
        print "Cache management activated."
        for x in range(0,len(cache)/2):
            cache.popitem(last=True)
        print "Cache management successful."
        return


"""
Checks if a file is in the cache. If it is, it checks if it has been modified since last cached before pullig it from the cache.
Otherwise, the server response is cached and cache management is called
"""

def checkCache(clientRequest):
    parseMessage = clientRequest.split()
    fileRequest = ""
    for x in range(0, len(parseMessage)):
        if "GET" in parseMessage[x]:
            fileRequest = parseMessage[x+1].strip()  
        if "Host:" in parseMessage[x]:
            host = parseMessage[x+1].strip()
    if fileRequest in cache:
        print "File in cache!"
        parseMessage = clientRequest.split("\n")
        for x in range(0, len(parseMessage)):
            if "Modified-Since" in parseMessage[x]:
                parseMessage[x] = cache[fileRequest][0]
        IfModified = "\n".join(parseMessage)
        if "304" in sendRequest(IfModified, 80):
            print "Cached file up to date!"
            return cache[fileRequest][1] 
        else:
            print "File outdated."  
    serverResponse = sendRequest(clientRequest, 80)
    parseMessage = clientRequest.split("\n")
    g = False
    for x in range(0, len(parseMessage)):
        if "Modified-Since" in parseMessage[x]:
            date = parseMessage[x]
            g = True
    if g is True:      
        cache[fileRequest] = (date, serverResponse)
        print "File cached from {}".format(host)
        manageCache()
    return serverResponse
    


"""
sends a request form the proxy to the server
"""

def sendRequest(clientRequest, port):
    try: 
        parseMessage = clientRequest.split()
        for x in range(0, len(parseMessage)):
            if "Host" in parseMessage[x]:
                host = parseMessage[x+1]
                print  "Request sent to {}".format(host)
        serverSocket = socket(AF_INET,SOCK_STREAM)
        serverSocket.connect((host, port))
        serverSocket.send(clientRequest)
        return receive(serverSocket)
    except BaseException, e:
        print " ??? "

"""
Handler function for new threads created by requests from client.

"""
def handleRequest(clientSocket, addr):
    clientSocket.settimeout(10)
    try:
        clientRequest = receive(clientSocket).replace("keep-alive", "close")
        if clientRequest.strip() == None or clientRequest.strip() == "":
            clientSocket.close()
            return
        serverResponse = checkCache(clientRequest)
        try:
            clientSocket.send(serverResponse)     
        except TypeError:
            print "clientRequest: {}".format(clientRequest)
            print "serverResponse: {}".format(serverResponse)
        clientSocket.close()
        return
    except timeout:
        print "Message from client not received.\n"


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
clientSocket.close()