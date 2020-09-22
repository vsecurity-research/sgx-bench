/*
 * |Aisha Hasan|
 *
 * The |Enclave/Enclave_lib_timing.c| file.
 */

#include "bench.h"

static struct mytimeval start_tv, stop_tv;
static volatile uint64 use_result_dummy;

#define DONE 1
#define NOT_DONE 0
int done = NOT_DONE;

/*
 * Start timing now.
 */
void start(struct mytimeval *tv)
{
	if (tv == NULL) {
		tv = &start_tv;
	}

	ocall_gettimeofday(tv);
}

/*
 * Stop timing and return real time in microseconds.
 */
uint64
stop(struct mytimeval *begin, struct mytimeval *end)
{
	if (end == NULL) {
		end = &stop_tv;
		ocall_gettimeofday(end);
	}
	ocall_gettimeofday(end);

	if (begin == NULL) {
		begin = &start_tv;
	}
	return (tvdelta(begin, end));
}

uint64 tvdelta(struct mytimeval *start, struct mytimeval *stop)
{
	struct mytimeval td;
	uint64 usecs;

	tvsub(&td, stop, start);
	usecs = td.tv_sec;
	usecs *= 1000000;
	usecs += td.tv_usec;
	return (usecs);
}

void tvsub(struct mytimeval *tdiff, struct mytimeval *t1, struct mytimeval *t0)
{
	tdiff->tv_sec = t1->tv_sec - t0->tv_sec;
	tdiff->tv_usec = t1->tv_usec - t0->tv_usec;
	if (tdiff->tv_usec < 0 && tdiff->tv_sec > 0) {
		tdiff->tv_sec--;
		tdiff->tv_usec += 1000000;

#ifndef NDEBUG
		if (tdiff->tv_usec < 0) {
			char buf[BUFSIZE] = {'\0'};

			snprintf(buf, BUFSIZE, "\tFILE: %s LINE:%d \t negative time!",
					 __FILE__, __LINE__);
			ocall_print_string(buf);
			memset(buf, '\0', BUFSIZE);
			//exit(); - sgx doesn't support exit
		}
#endif /* end assert()*/
	}

	/* time shouldn't go backwards!!! */
	if (tdiff->tv_usec < 0 || t1->tv_sec < t0->tv_sec) {
		tdiff->tv_sec = 0;
		tdiff->tv_usec = 0;
	}
}

uint64 gettime(void)
{
	return (tvdelta(&start_tv, &stop_tv));
}

void use_int(int result) { use_result_dummy += result; }