#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#define PORT 8000
int main()
{
    int server_fd, new_socket, valread,arg;
    struct sockaddr_in address;  
    int opt = 1;
    int addrlen = sizeof(address);
    char *buffer = (char *) calloc(1000, sizeof(char));
    char *hello = (char *) calloc(1000, sizeof(char));

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)  // creates socket, SOCK_STREAM is for TCP. SOCK_DGRAM for UDP
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    // This is to lose the pesky "Address already in use" error message
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))) // SOL_SOCKET is the socket layer itself
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;  // Address family. For IPv6, it's AF_INET6. 29 others exist like AF_UNIX etc. 
    address.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP address - listens from all interfaces.
    address.sin_port = htons( PORT );    // Server port to open. Htons converts to Big Endian - Left to Right. RTL is Little Endian
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    // Port bind is done. You want to wait for incoming connections and handle them in some way.
    // The process is two step: first you listen(), then you accept()
    if (listen(server_fd, 3) < 0) // 3 is the maximum size of queue - connections you haven't accepted
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    // returns a brand new socket file descriptor to use for this single accepted connection. Once done, use send and recv
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    valread = read(new_socket , buffer, 1000);  // read infromation received into the buffer
    printf("%s\n",buffer);
    strcpy(hello,"Hello from server");
    send(new_socket , hello , strlen(hello) , 0 );  // use sendto() and recvfrom() for DGRAM
    printf("Hello message sent\n");
    while(1)
    {
    	char *count = (char *) calloc(1000, sizeof(char));
    	char *buff = (char *) calloc(1000, sizeof(char));
	    valread = read(new_socket , count, 1000);//reading the number of arguments of command
	    arg = atoi(count);
	    //printf("%d\n",arg);
	    strcpy(hello,"fine");
	    send(new_socket , hello , strlen(hello) , 0 );  // use sendto() and recvfrom() for DGRAM
	    if(arg==1)
	    {
	        valread = read(new_socket , buff , 1000);
	        send(new_socket , hello , strlen(hello) , 0 ); 
	        if(strcmp(buff,"exit")==0)
	        {
	        	printf("Server is closing because client breaks the connection.\n");
	        	break;
	        }
	        else
	        {
	        	printf("Server has to stop because of wrong command in client.\n");
	        }
	    }
	    else if(arg>1)
	    {
	    	valread = read(new_socket , buff , 1000);
	    	//printf("%s\n",buff);
	    	send(new_socket , hello , strlen(hello) , 0 ); 
	        if(strcmp(buff,"get")==0)
	        {
	            for(int i=1;i<arg;i++)
	            {
	            	float perc;
	            	char *buff1 = (char *) calloc(1000, sizeof(char));
	            	char *file = (char *) calloc(1000, sizeof(char));
	            	valread = read(new_socket , file , 1000);
	            	printf("Client has requested to download %s\n",file);
	            	char *flag = (char *) calloc(1000, sizeof(char));
	            	int flg;
	            	FILE *fp = fopen(file,"r");
	            	if(fp==NULL)
	            	{
	            		flg=0;
	            	}
	            	else
	            	{
	            		flg=1;
	            	}
	            	sprintf(flag,"%d",flg);
	            	strcpy(hello,flag);
	            	//printf("%s %d\n",flag,flg);
	           		send(new_socket , hello , strlen(hello) , 0 );  // send the message.
	           		if(flg==1)
	           		{
	           			FILE *ab = fopen(file,"r");
	           			fseek(ab,0L,SEEK_END);
	           			long int size = ftell(ab);
	           			long int sz = 0;
	           			valread = read(new_socket,buffer,1000);
	           			sprintf(flag,"%ld",size);
	            	    strcpy(hello,flag);
	            	    //printf("%s\n",flag);
	           			send(new_socket , hello , strlen(hello) , 0 );  // send the message.
	           			valread = read(new_socket,buffer,1000);
	           			//printf("%s\n",buffer);
	           			int a=0;
	           			int k=0,l;
	           			int fd = open(file,O_RDONLY);
	           			while(sz<size)
	           			{
	           				char *c = (char *) calloc(30000, sizeof(char));
	           				lseek(fd,k, SEEK_CUR);
							l = (int)read(fd,c,30000);
							sz=sz+l;
							//printf("%d\n",l);
							if(send(new_socket,c,l,0)==-1)
	           				{
	           					printf("error in sending\n");
	           					break;
	           				}
	           				perc = (float)((float)sz/(float)size)*100;
                            printf("Percentage Transferred %0.2f%c\r",perc,'%');
	           			}
	           			if(size==0)
	           			{
	           				printf("Percentage Transferred 100.00%c\r",'%');
	           			}
	           			printf("\n");
	           			printf("File is sent\n");	           			
	           		}
	           		else
	           		{
	           			printf("Error in reading file\n");
	           		}
	            }
	        }
	        else
	        {
	            printf("Server stopped because of wrong command %s in client.\n",buff);
	        }
	    }
	    else
	    {
	        printf("Server has to stop because of wrong command %s in client.\n",buff);
	    } 
	}
	close(new_socket);
    return 0;
}
