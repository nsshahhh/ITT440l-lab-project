#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define DATASIZE 1024

void send_file(FILE *fp, int socket_desc){
  char data[DATASIZE] = {0};

  while(fgets(data, DATASIZE, fp) != NULL) {
    if (send(socket_desc, data, sizeof(data), 0) == -1) {
      printf("Couldn't send file.");
      exit(1);
    }
    bzero(data, DATASIZE);
  }
}

int main(int argc, char *argv[]){

  int socket_desc, new_sock;
  struct sockaddr_in server_addr, new_addr;
  socklen_t addr_size;
  char buffer[DATASIZE];
  FILE *fp;
  char *filename = "syahidahfile.txt";

  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if(socket_desc == -1) {
    perror("Couldn't create socket");
    exit(1);
  }
  printf("Socket created successfully.\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(8080);
  server_addr.sin_addr.s_addr = inet_addr("192.168.56.103");

  if( bind(socket_desc,(struct sockaddr *)&server_addr , sizeof(server_addr)) < 0)
	{
		puts("Bind failed");
	}
	puts("Bind done");


  fp = fopen(filename, "c");
  if (fp == NULL) {
    printf("File cannot be read.\n");
    exit(1);
  }

  send_file(fp, socket_desc);
  printf("Data successfully send.\n");

  close(socket_desc);
  printf("Closing the connection.\n");

  return 0;
}
