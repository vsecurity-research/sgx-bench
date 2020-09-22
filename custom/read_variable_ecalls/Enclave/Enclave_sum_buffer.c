/*
 * |Aisha Hasan|
 *
 * The |Enclave/Enclave_sum_buffer.c| file.
 *
 *
 *
 */

#include "Enclave_sum_buffer.h"

int *buffer = NULL;
int *last = NULL;

void ecall_send_params(int inbufsz)
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

	snprintf(buf, BUFSIZE, "%s LINE:%d\tinbufsz:[%d].",
			 __FILE__, __LINE__, inbufsz);
	ocall_print_string(buf);
	memset(buf, '\0', BUFSIZE);
}

void init(int inbufsz, int *mybuf, int *lastone)
{

	char buf[BUFSIZE] = {'\0'};

	snprintf(buf, BUFSIZE, "%s LINE:%d\tinbufsz[%d], ints_in_buffer[%lu].",
			 __FILE__, __LINE__, inbufsz, inbufsz / sizeof(int));
	ocall_print_string(buf);
	memset(buf, '\0', BUFSIZE);

	// initializing array elements
	for (int i = 0; i < inbufsz / sizeof(int); i++) {
		mybuf[i] = 1; //i+1;
	}

	snprintf(buf, BUFSIZE,
			 "%s LINE:%d\tafter init: buf[0]=[%d] last=[%d].", __FILE__,
			 __LINE__, mybuf[0], *lastone);
	ocall_print_string(buf);
	memset(buf, '\0', BUFSIZE);
}

int ecall_get_sum(int index, int numints)
{
	int sum = 0;
	for (int i = 0; i < numints; i++) {
		sum += buffer[index + i];
	}

	return sum;
}
