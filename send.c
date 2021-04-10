#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "link_emulator/lib.h"

#define HOST "127.0.0.1"
#define PORT 10000


int main(int argc,char** argv){
  init(HOST,PORT);
  msg t;

  //Send dummy message:
  // printf("[send] Sending dummy...\n");
  // sprintf(t.payload,"%s", "This is a dummy.");
  // t.len = strlen(t.payload)+1;
  // send_message(&t);

  int dimensiune = 0;
  int max_size = 1400;
  char buff[max_size];
  
  // open file
  int fd = open("file.txt", O_RDONLY);
  if(fd < 0) {
    printf("error\n");
    return -2;
  }

  // send the size of file
  int size = lseek(fd, 0, SEEK_END);
  sprintf(t.payload, "%d", size);
  t.len = strlen(t.payload) + 1;
  send_message(&t);
   // Check response:
  if (recv_message(&t)<0){
    perror("Receive error ...");
    return -1;
  }
  else {
    printf("[send] Got reply with payload: %s\n", t.payload);
  }

  // reset file descriptor
  lseek(fd, 0, SEEK_SET);
  // read from file, send to receiver
  while((dimensiune = read(fd, buff, max_size))) {
  	if(dimensiune <= 0){
  		printf("error\n");
  		return -1;
  	}
  	printf("Sending message :)\n");
    sprintf(t.payload, "%s", buff);
    t.len = strlen(t.payload);
    send_message(&t);
    printf("[send] message dim:%d message:%s\n", t.len, buff);
    // Check response:
    if (recv_message(&t)<0){
        perror("Receive error ...");
        return -1;
    }
    else {
        printf("[send] Got reply with payload: %s\n", t.payload);
    }

    memset(buff, 0, sizeof(buff));
  }

  close(fd);
 

  return 0;
}
