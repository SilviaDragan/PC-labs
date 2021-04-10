#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10001

int main(void)
{
	msg r;
	int i, res;
	
	printf("[RECEIVER] Starting.\n");
	init(HOST, PORT);
	
	//write in this file
	int f_write = open("output_file", O_WRONLY | O_CREAT);

	for (i = 0; i < COUNT; i++) {
		/* wait for message */
		res = recv_message(&r);
		// printf("sunt in receiver\n");
		if (res < 0) {
			perror("[RECEIVER] Receive error. Exiting.\n");
			return -1;
		}

		char control_sum = r.payload[0] ^ r.payload[1] ^ r.payload[2] ^ r.payload[3] ^ 
					r.payload[4]^ r.payload[5]^ r.payload[6];
		if(control_sum != r.suma_control) printf("[recv] ERROR control_sum\n");
		
		write(f_write, r.payload, 7);	

		/* send dummy ACK */
		res = send_message(&r);
		if (res < 0) {
			perror("[RECEIVER] Send ACK error. Exiting.\n");
			return -1;
		}
	}

	printf("[RECEIVER] Finished receiving..\n");
	return 0;
}
