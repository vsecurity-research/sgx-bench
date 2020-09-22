/*
 * |Aisha Hasan|
 *
 * The |Enclave/Enclave_sum_buffer.c| file.
 *
 *
 *
 */

#include "Enclave_sum_buffer.h"

void print_time(uint64 time);

int *buffer = NULL;
int *last = NULL;

void ecall_send_params(int inbufsz, int inloops)
{

	char buf[BUFSIZE] = {'\0'};

	//malloc inner buffer
	if (buffer == NULL) {
		buffer = (int *)malloc(inbufsz);
		last = (int *)((char *)buffer + inbufsz - sizeof(int));

		if (!buffer) {
			snprintf(buf, BUFSIZE, "%s LINE:%d\n"
								   "\t malloc error.\n",
					 __FILE__, __LINE__);
			ocall_print_string(buf);
			memset(buf, '\0', BUFSIZE);
		}
		init(inbufsz, buffer, last);
	}

	snprintf(buf, BUFSIZE, "%s LINE:%d\tinbufsz:[%d] inloops=%d.",
			 __FILE__, __LINE__, inbufsz, inloops);
	ocall_print_string(buf);
	memset(buf, '\0', BUFSIZE);

	start(0);

	int sum = 0;

	for (int i = 0; i < inloops; i++) {
		sum = sum_buffer();
	}

	ocall_sum_buffer();
	stop(0, 0);

	snprintf(buf, BUFSIZE, "%s LINE:%d\tsum[%d].",
			 __FILE__, __LINE__, sum);
	ocall_print_string(buf);
	memset(buf, '\0', BUFSIZE);

	print_time(gettime());
}

void init(int inbufsz, int *mybuf, int *lastone)
{

	char buf[BUFSIZE] = {'\0'};

	snprintf(buf, BUFSIZE, "%s LINE:%d\tinbufsz[%d], num_ints[%lu].",
			 __FILE__, __LINE__, inbufsz, inbufsz / sizeof(int));
	ocall_print_string(buf);
	memset(buf, '\0', BUFSIZE);

	// initializing array elements
	for (int i = 0; i < inbufsz / sizeof(int); i++) {
		mybuf[i] = 1; //i+1;
	}

	snprintf(buf, BUFSIZE, "%s LINE:%d\tafter init: buf[0]=[%d] last=[%d].",
			 __FILE__, __LINE__, mybuf[0], *lastone);
	ocall_print_string(buf);
	memset(buf, '\0', BUFSIZE);
}

int sum_buffer()
{
	int sum = 0;
	int *tempptr = buffer;

	while (tempptr <= last) {
		sum += *tempptr;
		tempptr++;
	}

	// This ensure the compiler doesn't optimized away the loop above.
	use_int(sum);
	return sum;
}

void print_time(uint64 time)
{
	char buf[BUFSIZE] = {'\0'};
	double secs = (double)time / 1000000.0;

	if (secs <= 0.)
		return;

	if (secs < 1.) {
		snprintf(buf, BUFSIZE, "\tFILE: %s:%d \t %.6f secs\n",
				 __FILE__, __LINE__, secs);
	} else {
		snprintf(buf, BUFSIZE, "\tFILE: %s:%d \t %.2f secs\n",
				 __FILE__, __LINE__, secs);
		ocall_print_string(buf);
		memset(buf, '\0', BUFSIZE);
	}
}
