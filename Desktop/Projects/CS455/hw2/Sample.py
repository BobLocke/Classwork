# CS 455 - Programming homework 2

from socket import *
import sys, threading
#can't import any http related library here.
#please double check with me if you do

if len(sys.argv) <= 2:
    print 'Usage : "python HttpProxy.py server_port \n"'
    sys.exit(2)


# Create a server socket, bind it to a port and start listening
proxy_socket = socket(AF_INET, SOCK_STREAM)

# Prog2: student to implement here

while 1:
    # waitng for connections from clients
    client_socket, addr = proxy_socket.accept()
    
    ### This is the logic for handling a single request. Students must modify the program 
    ### to make the server support multiple concurent requests. It's time to learn how 
    ### to use threads in python
    
    # Prog2: read the request from client 
    client_request =
    # Extract the filename from the request
    request_file = 
    
    file_exist = "false"
    # Prog2: 
    # - check the "cache" to see if the file is there. 
    #        If it is, return the file. 
    #       Otherwise, request the web server for the file
    
    # Important: Need to think about
    # - What data structure to allow efficient checking of file presence? 
    # - How to control the cache size?
    # - What to do when the cache is full? 
    # - How to make the cache threadsafe; make it consistent under concurrency?
    # - How to make sure the file in cache is not outdated
    # Review concepts about hashtable and take a look at dictionaries in python

    if file_exist = "true"
        # ProxyServer finds a cache hit and generates a response message
        client_socket.send("HTTP/1.1 200 OK\r\n")
        client_socket.send("Content-Type:text/html\r\n")
        
        #Prog2: send the rest of the HTTP response
        # - fill the rest of the http header 
        # - make sure to turn off keep-alive option

    else #file is not in the cache
        # Create a socket on the proxyserver
        try:
            # Connect to the server on the default port 80 to request the file
            server_socket = 

            #Prog2: 
            # - send the rest of the HTTP request to the server
            # - read response from the server
            # - send the response to the client
            # - update cache
        except:
            print "Illegal request"

    # Close the client and the server sockets
    client_socket.close()

#Prog2: clean up
