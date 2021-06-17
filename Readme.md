# Client-Server Socket Programming
## How to run  
 - Open two windows of terminal. 
 - In one window run these commands.
```
 cd server
 gcc -o server server.c
 ./server
```
- In other window run these commands.
```
 cd client 
 gcc -o client  client .c
 ./client 
```
- On client side, a `client>` prompt will appear. 
- Type command `get <filename1> <filename2>` (here client can ask for maximum of two files) and it will download the files from server to client.
- If file is not present in server, both server and client will show the error messages.
- It will also show Percentage progress on both client and server if the file is present in server. 
- Type command `exit` then it will close its connection with server.
- If in any case other commands are typed it will give error message in both server and client.
 
