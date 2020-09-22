/*
 * |Aisha Hasan|
 * 
 * The |App/sum_buffer.c| file. 
 *
 */

#include "sum_buffer.h"
#include "EnclaveBridge.h"

void usage();

int main(int ac, char *av[])
{
	int inbufsize = 0;
	int outbufsize = 0;
	int inloops = 0;
	int outloops = 0;
	int numintsread = 0;
	int mod;

	if (ac < 6) {
		printf("Too few args.\n");
		usage();
	}

	//inbuf
	inbufsize = atoi(av[1]);
	if ((inbufsize <= 0) || (inbufsize > 100)) {
		printf("inbufsize in MB must be greater than 0 & smaller than 100.\n");
		usage();
	}

	inbufsize = inbufsize * 1024 * 1024; //make sure this is mulitple of 4

	if (mod = (inbufsize % sizeof(int)))
		inbufsize += (sizeof(int) - mod);

	//inloops
	inloops = atoi(av[2]);
	if ((inloops <= 0) || (inloops > 10000000)) {
		printf("inloops should be greater than 0 & smaller than 10,000,000.\n");
		usage();
	}

	//ints-per-read
	numintsread = atoi(av[3]);
	if (numintsread <= 0 || ((inbufsize / sizeof(int)) % numintsread)) {
		printf("Numints  should divide number of ints in enclave buffer. "
			   "numintsread=[%d] num-ints-in-enclave=[%lu].\n",
			   numintsread, inbufsize / sizeof(int));
		usage();
	}

	//outbuf
	outbufsize = atoi(av[4]);
	if ((outbufsize <= 0) || (outbufsize > 100)) {
		printf("outbufsize in MB must be greater than 0 & smaller than 100.\n");
		usage();
	}

	outbufsize = outbufsize * 1024 * 1024; //make sure this is mulitple of 4

	if (mod = (outbufsize % sizeof(int)))
		outbufsize += (sizeof(int) - mod);

	//outloops
	outloops = atoi(av[5]);
	if ((outloops <= 0) || (outloops > 10000000)) {
		printf("outloops should be greater than 0 & smaller than 10,000,000.\n");
		usage();
	}

	printf("%s LINE:%d inbufsize[%d] outbufsize[%d] "
		   "inloops[%d] outloops[%d] numintsread[%d].\n",
		   __FILE__, __LINE__,
		   inbufsize, outbufsize, inloops, outloops, numintsread);

	send_params(inbufsize, outbufsize, inloops, outloops, numintsread);

	printf("Info: SampleEnclave successfully returned.\n");
	return (0);
}

void usage()
{
	char message[512] = "Use ./sumbuffer [in-bufsize-in-MB] [in-loops] [ints-per-read] "
						"[out-bufsize-in-MB] [out-loops].\n";

	printf("%s", message);
	exit(1);
}
