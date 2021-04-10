#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "link_emulator/lib.h"

#define HOST "127.0.0.1"
#define PORT 10001


int main(int argc,char** argv){
  msg r;
  init(HOST,PORT);
  int file_size = 0;

  if (recv_message(&r)<0){
    perror("Receive message");
    return -1;
  }

  file_size = atoi(r.payload);

  printf("[recv] Got msg with payload: <%s>, sending ACK...\n", r.payload);
  // Send ACK:
  sprintf(r.payload,"%s", "ACK");
  r.len = strlen(r.payload) + 1;
  send_message(&r);
  printf("[recv] ACK sent\n");

  int destination_fd = open(r.payload, O_WRONLY | O_CREAT);
  lseek(destination_fd, 0, SEEK_SET);
  int size = 0;

  while ((recv_message(&r) > 0) && (size = write(destination_fd, r.payload, r.len)) ) {
  	file_size -= size;
  	if (size < 1400 || file_size <= 0 ) {
  		// printf("ERROR\n");
  		sprintf(r.payload, "%s", "ACK");
	  	r.len = strlen(r.payload) + 1;
		send_message(&r);
		printf("[recv] ACK sent\n");
		break;

  	}

  	sprintf(r.payload, "%s", "ACK");
  	r.len = strlen(r.payload) + 1;
	send_message(&r);
	printf("[recv] ACK sent\n");
  }
  close(destination_fd);
  // // Send ACK:
  // sprintf(r.payload,"%s", "ACK");
  // r.len = strlen(r.payload) + 1;
  // send_message(&r);
  // printf("[recv] ACK sent\n");

  return 0;
}
