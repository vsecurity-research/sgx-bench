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

	int counter;
	int megabytes = 0;
	int loops = 0;

	if (ac < 3) {
		printf("Too few args. Use ./sumbuffer [outbufsize-in-MB] [loops].\n");
		exit(1);
	}

	megabytes = atoi(av[1]);
	if ((megabytes <= 0) || (megabytes > 100)) {
		printf("Megabytes should be greater than 0 and smaller than 100\n");
		printf("Use ./sumbuffer [outbufsize-in-MB] [loops].\n");
		exit(1);
	}

	loops = atoi(av[2]);
	if ((loops <= 0) || (loops > 100000)) {
		printf("Loops should be greater than 0 and smaller than 100,000\n");
		printf("Use ./sumbuffer [outbufsize-in-MB] [loops].\n");
		exit(1);
	}

	int mod;
	int bufsz = megabytes * 2 * 1024 * 1024; //make sure this is mulitple of 4
	int outbufsz = bufsz / 2;

	if (mod = (outbufsz % sizeof(int)))
		outbufsz += (sizeof(int) - mod);
	if (outbufsz > bufsz)
		outbufsz = bufsz;

	printf("%s LINE:%d buf[%d] inbuf[%d] outbuf[%d].\n",
		   __FILE__, __LINE__, bufsz, bufsz - outbufsz, outbufsz);

	send_params(bufsz, outbufsz, loops);

	printf("Info: SampleEnclave successfully returned.\n");
	return (0);
}
