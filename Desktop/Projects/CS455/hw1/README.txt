Ryan "Bob" Dean
CS 455 - Networking
9/8/2014

Simple Hack to a FTP Server

files included:

	ftpclient_hw.py - ftp server hacking script

	rockyou_light.txt - list of potential passwords

	rockyou_short.txt - a shorter list for testing

files created:

	<filename.txt> - the file pulled from the server


Phases:

	Phase 1, Reconnisance:

		I scan ports 1 - 65535, checking for a connection and a message from the server. 
		If 'FTP' is found in th messive recieved, I know I have the right port and disconnect.
	
	Phase 2, Attacking:

		I first load the entire list from file and place each potential password into a list. 
		Then I run through the list, checking each password and disconnecting on failure.
		This is a very slow process, which is why I have the shorter list for sanity and testing purposes.

	Phase 3, Exploitation:

		I reconnect using the password and port found in the first 2 steps. Then, I send a PASV command and pull the data socket IP and port from the response.
		After opening a data socket with that, I request a LIST of the files on the server, and recieve the list through the data socket. 
		I then redo the data socket connection, and pull the file containing "programming" and "cs455" in the title from the server and save it locally.


To run: in terminal, run 'python ftpclienthw.py'. The file submitted is set to use the password list "rockyou_short.txt". 
	I'd suggest using that, but if you want to test the entire list, change line 11 to "PASSFILE = 'rockyou_light.txt'. It will take a while, take a nap or something.


Should FTP should use only one TCP connection for both data and control packets?

	No. Data might get held up because it is waiting for a response, if you only use one connection. 
	It's much more obvious when content is sent recieves with seperate connections for specific functions.