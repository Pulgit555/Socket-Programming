#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#define PORT 8000

int main()
{
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char command[250][1000];
    char *hello = (char *) calloc(1000, sizeof(char));
    char *buffer = (char *) calloc(1000, sizeof(char));
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    memset(&serv_addr, '0', sizeof(serv_addr)); // to make sure the struct is empty. Essentially sets sin_zero as 0 which is meant to be, and rest is defined below
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    // Converts an IP address in numbers-and-dots notation into either a 
    // struct in_addr or a struct in6_addr depending on whether you specify AF_INET or AF_INET6.
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)  // connect to the server address
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    strcpy(hello,"hello from Client");
    send(sock , hello , strlen(hello) , 0 );  // send the message.
    valread = read( sock , buffer, 1000);  // receive message back from server, into the buffer
    printf("%s\n",buffer);
    while(1)
    {
        int z=0;
        char *buff = (char *) calloc(1000, sizeof(char));
        printf("client>");
        fgets(buff,1000,stdin);
        buff[strlen(buff)-1]='\0';
        char *com = strtok(buff," ");
        while(com!=NULL)
        {
            strcpy(command[z],com);
            //printf("%s\n",command[z]);
            z++;
            com = strtok(NULL," ");
        }
        char *argc = (char *) calloc(100, sizeof(char));
        sprintf(argc,"%d",z);
        strcpy(hello,argc);
        send(sock , hello , strlen(hello) , 0 );  // send the number of arguments.
        valread = read( sock , buffer, 1000);
        if(z==1)
        {
            strcpy(hello,command[0]);
            send(sock , hello , strlen(hello) , 0 );  // send the message.
            valread = read( sock , buffer, 1000);
            if(strcmp(command[0],"exit")==0)
            {
                printf("Connection is closed from client side\n");
                break;
            }
            else
            {
                printf("%s command is not available\n",command[0]);
            }
        }
        else if(z>1)
        {
            strcpy(hello,command[0]);
            send(sock , hello , strlen(hello) , 0 );  // send the message.
            //printf("%s\n",hello);
            if(strcmp(command[0],"get")==0)
            {
                char *fl = (char *) calloc(1000, sizeof(char));
                valread = read(sock,fl,1000);
                for(int i=1;i<z;i++)
                {
                    int part=0;
                    float perc;
                    char *buf = (char *) calloc(1000, sizeof(char));                    
                    strcpy(hello,command[i]);
                    //printf("%s\n",hello);
                    send(sock , hello , strlen(hello) , 0 );  // send the message.
                    printf("Client requested %s from server.\n",hello);
                    int flag;
                    char *flg = (char *) calloc(100, sizeof(char));
                    valread = read(sock , flg , 100);
                    flag = atoi(flg); 
                    //printf("%s %d\n",flg,flag);
                    if(flag==1)
                    {
                        int n;
                        strcpy(hello,"fine");
                        send(sock , hello , strlen(hello) , 0 ); 
                        printf("File started downloading.\n");
                        valread = read(sock , buf , 1000);
                        int size = atoi(buf);
                        //printf("%d\n",size);
                        int fd1 = open(command[i],O_WRONLY | O_TRUNC | O_CREAT, 0644);
                        //FILE *fp = fopen(command[i],"w");
                        send(sock , hello , strlen(hello) , 0 );
                        while(1)
                        {
                            if(size==0)
                            {
                                break;
                            }
                            char *buff1 = (char *) calloc(30000, sizeof(char));
                            //printf("%s\n",hello);
                            n = read(sock,buff1,30000);
                            //fprintf(stderr,"%s\n",buff1);
                            part=part+n;
                            //fprintf(stderr,"%d %d %d\n",size,n,part);
                            if(n<=0)
                            {
                                break;
                            }
                            int k = write(fd1,buff1,n);
                            //strcpy(hello,"fine");
                            //send(sock , hello , strlen(hello) , 0 ); 
                            perc = (float)((float)part/(float)size)*100;
                            printf("Percentage Received %0.2f%c\r",perc,'%');
                            if(part==size)
                            {
                                break;
                            }
                        }
                        if(size==0)
                        {
                            printf("Percentage Received 100.00%c\r",'%');
                        }
                        printf("\n");
                        printf("File is downloaded\n");
                    }
                    else
                    {
                        printf("File is not present.\n");
                    }
                }
            }
            else
            {            
                printf("Type the correct command from Readme file.\n");
            }
        }
        else
        {
            printf("Type the correct command from Readme file.\n");
        }
    }
    close(sock);
    return 0;
}
