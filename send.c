#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10000

int main(int argc, char *argv[])
{
	msg t;
	int i, j, res;
	
	printf("[SENDER] Starting.\n");	
	init(HOST, PORT);

	printf("[SENDER]: BDP=%d\n", atoi(argv[1]));
	int bdp = atoi(argv[1]);
	// calculate window_size
	// ws = (bw * delay * 1000)/ (sizeof(FRAME)*8)
	int window_size = (bdp * 1000)/(MSGSIZE * 8);
	printf("window size = %d\n", window_size);

	for (i = 0; i < window_size; i++) {
		/* cleanup msg */
		memset(&t, 0, sizeof(msg));
		// send window_size messages
		printf("[send] Sending message %d\n", i);
		sprintf(t.payload,"%s", "message");
		t.len = strlen(t.payload)+1;
		t.suma_control = t.payload[0]^t.payload[1]^t.payload[2]^t.payload[3]^
							t.payload[4]^t.payload[5]^t.payload[6];
		/* send msg */
		res = send_message(&t);
		if (res < 0) {
			perror("[SENDER] Send error. Exiting.\n");
			return -1;
		}
	}	

	for (j = window_size; j < COUNT; j++) {
		/* wait for ACK */
		res = recv_message(&t);
		if (res < 0) {
			perror("[SENDER] Receive error. Exiting.\n");
			return -1;
		}
		memset(&t, 0, sizeof(msg));
		// when ACK is recieved, send new message
		printf("[send] Sending message %d\n", j);
		sprintf(t.payload,"%s", "message");
		t.len = strlen(t.payload)+1;
		t.suma_control = t.payload[0]^t.payload[1]^t.payload[2]^t.payload[3]^
							t.payload[4]^t.payload[5]^t.payload[6];
		res = send_message(&t);
		if (res < 0) {
			perror("[SENDER] Send error. Exiting.\n");
			return -1;
		}
	}
	for( i = 0; i < window_size; i++) {
		/* wait for ACK */
		res = recv_message(&t);
		if (res < 0) {
			perror("[SENDER] Receive error. Exiting.\n");
			return -1;
		}
	}
	
	printf("[SENDER] Job done, all sent.\n");
		
	
	// for (i = 0; i < COUNT; i++) {
	// 	/* cleanup msg */
	// 	memset(&t, 0, sizeof(msg));
		
	// 	/* gonna send an empty msg */
	// 	t.len = MSGSIZE;
		
	// 	/* send msg */
	// 	res = send_message(&t);
	// 	if (res < 0) {
	// 		perror("[SENDER] Send error. Exiting.\n");
	// 		return -1;
	// 	}
		
	// 	/* wait for ACK */
	// 	res = recv_message(&t);
	// 	if (res < 0) {
	// 		perror("[SENDER] Receive error. Exiting.\n");
	// 		return -1;
	// 	}
	// }

		
	return 0;
}
