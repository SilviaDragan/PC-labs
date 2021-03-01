#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{

	int miner_sursa;
	char *filename = malloc(100*sizeof(char)); 
	char buf[1024];
	scanf("%s", filename);

	miner_sursa = open(filename, O_RDONLY);
	if(miner_sursa > 0) {                            
		while(read(miner_sursa, buf, 1)) {
			write(STDOUT_FILENO, buf, 1);
		}
	}
	close(miner_sursa);
	return 0;
}