#include <stdio.h>
#include <string.h>      //strlen
#include <sys/socket.h>
#include <arpa/inet.h>   //inet_addr
#include <unistd.h>      //write
#include <netinet/in.h>
#include <sys/types.h>
#include <stdlib.h>

#define IP_PROTOCOL 0
#define PORT_NO 15050
#define DATA_SIZE 32
#define cipherKey 'S'
#define sendrecvflag 0
#define nofile "File Not Found!"
  
// function to clear buffer
void clearBuf(char* e)
{
    int i;
    for (i = 0; i < DATA_SIZE; i++)
        e[i] = '\0';
}
  
// function to encrypt
char Cipher(char cp)
{
    return cp ^ cipherKey;
}
  
// function sending file
int sendFile(FILE* fp, char* buf, int s)
{
    int i, len;
    if (fp == NULL) {
        strcpy(buf, nofile);
        len = strlen(nofile);
        buf[len] = EOF;
        for (i = 0; i <= len; i++)
            buf[i] = Cipher(buf[i]);
        return 1;
    }
  
    char cp, cp2;
    for (i = 0; i < s; i++) {
        cp = fgetc(fp);
        cp2 = Cipher(cp);
        buf[i] = cp2;
        if (cp == EOF)
            return 1;
    }
    return 0;
}
  
// driver code
int main()
{
    int socket_desc;
    int  nBytes;
    struct sockaddr_in server;
    int addrlen = sizeof(server);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT_NO);
    server.sin_addr.s_addr = INADDR_ANY;
    char net_buf[DATA_SIZE];
    FILE* fp;
  
    // socket()
    socket_desc = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL);
  
    if (socket_desc < 0)
        printf("\nfile descriptor not received!!\n");
    else
        printf("\nfile descriptor %d received\n", socket_desc);
  
    // bind()
    if (bind(socket_desc, (struct sockaddr*)&server, sizeof(server)) == 0)
        printf("\nSuccessfully binded!\n");
    else
        printf("\nBinding Failed!\n");
  
    while (1) {
        printf("\nWaiting for file name...\n");
  
        // receive file name
        clearBuf(net_buf);
  
        nBytes = recvfrom(socket_desc, net_buf,DATA_SIZE, sendrecvflag,(struct sockaddr*)&server, &addrlen);
  
        fp = fopen(net_buf, "r");
        printf("\nFile Name Received: %s\n", net_buf);
        if (fp == NULL)
            printf("\nFile open failed!\n");
        else
            printf("\nFile Successfully opened!\n");
  
        while (1) {
  
            // process
            if (sendFile(fp, net_buf, DATA_SIZE)) {
                sendto(socket_desc, net_buf, DATA_SIZE,sendrecvflag, (struct sockaddr*)&server, addrlen);
                break;
            }
  
            // send
            sendto(socket_desc, net_buf, DATA_SIZE, sendrecvflag,(struct sockaddr*)&server, addrlen);
            clearBuf(net_buf);
        }
        if (fp != NULL)
            fclose(fp);
    }
    return 0;
}

