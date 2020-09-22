/*
 * |Aisha Hasan|
 * 
 * The |App/sum_buffer.c| file. 
 *
 */

#include "sum_buffer.h"
#include "EnclaveBridge.h"

int main(int ac, char *av[])
{
	int megabytes = 0;
	int loops = 0;
	int numints = 0;

	if (ac < 4) {
		printf("Too few args. Use ./sumbuffer [bufsize-in-MB] [loops] [numints].\n");
		exit(1);
	}

	loops = atoi(av[2]);
	if ((loops <= 0) || (loops > 10000000)) {
		printf("Loops should be greater than 0 and smaller than 10,000,000.\n");
		printf("Use ./sumbuffer [bufsize-in-MB] [loops] [numints].\n");
		exit(1);
	}

	megabytes = atoi(av[1]);
	if ((megabytes <= 0) || (megabytes > 100)) {
		printf("Megabytes should be greater than 0 and smaller than 100.\n");
		printf("Use ./sumbuffer [bufsize-in-MB] [loops] [numints].\n");
		exit(1);
	}

	int mod;
	int bufsz = megabytes * 1024 * 1024; //make sure this is mulitple of 4
	int inbufsz = bufsz;

	if (mod = (inbufsz % sizeof(int)))
		inbufsz += (sizeof(int) - mod);
	if (inbufsz > bufsz)
		inbufsz = bufsz;

	printf("%s LINE:%d inbuf[%d].\n",
		   __FILE__, __LINE__, inbufsz);

	numints = atoi(av[3]);
	if (numints <= 0 || ((inbufsz / sizeof(int)) % numints)) {
		printf("Numints should divide number of integers in buffer. "
			   "numints=[%d] number of ints=[%lu].\n",
			   numints, inbufsz / sizeof(int));
		printf("Use ./sumbuffer [bufsize-in-MB] [loops] [numints].\n");
		exit(1);
	}

	send_params(inbufsz, loops, numints);

	printf("Info: SampleEnclave successfully returned.\n");
	return (0);
}
