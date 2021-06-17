2019101078
  |
  |
  |---server
  |     |---sever.c
  |
  |---client
  		|---client.c

 To run this code 
 open two windows of terminal ,
 in server directory type `gcc -o server server.c`
 and then `./server`
 in client directory type `gcc -o client client.c`
 and then `./client`
 Now a client> prompt will appear type command `get <filename1> <filename2>` and it will download the files from server to client if file is not present both server and client will show this error message.
 type command `exit` then it will close its connection with server
 If in any case other commands are typed it will give error message in both server and client.