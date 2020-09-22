/* 
 * |Aisha Hasan|
 * 
 * The |App/EnclaveBridge.c| file.
 * 
 */

#include "EnclaveBridge.h"

#include <sys/stat.h>

#define DEBUG_PRINT 1

#define debug_print(fmt, ...)                                   \
	do {                                                        \
		if (DEBUG_PRINT)                                        \
			fprintf(stderr, "%s %s():%d: \t" fmt,               \
					__FILE__, __func__, __LINE__, __VA_ARGS__); \
	} while (0)

static struct mytimeval start_tv, stop_tv;

static volatile unsigned long long use_result_dummy;
void use_int(int result)
{
	use_result_dummy += result;
}

/*###############################  OCALL functions  #########################*/
void ocall_print_string(const char *str)
{
	printf("%s\n", str);
	fflush(stdin);
}

/*##############################  Global variables  #########################*/
int *mybuf;
int *lastone;

/*
 * 1. Initialise enclave.
 * 2. Calculate buffer size (in + out = total).
 * 3. Allocate space for the outside buffer.
 * 4. Initialise outer buffer. Done once, as summing doesn't change
 *    the values in the buffer.
 * 5.
 * 
 **/
void send_params(int bufsz, int loops, int numints)
{

	int rv = NL_STATUS_OK;
	rv = ecall_initialise();

	printf("%s LINE:%d new enclaveID: [%lu].\n", __FILE__, __LINE__,
		   enclaveID);

	//malloc outer buffer
	debug_print("tempbufsize:[%d ints]\n", numints);

	mybuf = (int *)malloc(numints * sizeof(int));
	lastone = (int *)((char *)mybuf + numints * sizeof(int) - sizeof(int));

	if (!mybuf) {
		printf("%s LINE:%d\n\tmalloc error.\n", __FILE__, __LINE__);
	}

	//initialise outer buffer
	init(numints * sizeof(int), mybuf, lastone);

	ecall_send_params(enclaveID, bufsz);

	uint64 numecalls = 0;
	int index = 0;
	int sum = 0;
	int retvalsum = 0;

	start(0);
	for (int k = 0; k < loops; k++) {
		sum = 0;

		for (int i = 0; i < bufsz / sizeof(int); i += numints) {
			numecalls++;
			ecall_get_sum(enclaveID, &retvalsum, i, numints);
			sum += retvalsum;
		}
	}

	print_time(stop(0, 0));
	debug_print("sum=[%d].\n", sum);
	debug_print("numecalls=[%llu].\n", numecalls);

	printf("%s LINE:%d destroy enclaveID: [%lu].\n",
		   __FILE__, __LINE__, enclaveID);

	printf("%s LINE:%d %s destroyed enclaveID: [%lu].\n",
		   __FILE__, __LINE__,
		   (sgx_destroy_enclave(enclaveID) != SGX_SUCCESS) ? "not" : "",
		   enclaveID);
}

/*###########################################################################*/
/*##########################  SUMMING functions  ##########################*#*/
/*###########################################################################*/

/*
 * Fill buffer
 */
void init(int outbufsz, int *mybuf, int *lastone)
{
	debug_print("inside init: outbufsz[%d], num_ints[%lu].\n",
				outbufsz, outbufsz / sizeof(int));

	for (int i = 0; i < outbufsz / sizeof(int); i++) {
		mybuf[i] = 0; //i+1;
	}
}

/*###########################################################################*/
/*##########################  TIMING functions  ##########################*#*/
/*###########################################################################*/

void ocall_gettimeofday(struct mytimeval *tv)
{

	struct timeval temptv;

	(void)gettimeofday(&temptv, (struct timezone *)0);
	tv->tv_sec = temptv.tv_sec;
	tv->tv_usec = temptv.tv_usec;
}

/*
 * Start timing now.
 */
void start(struct mytimeval *tv)
{
	if (tv == NULL) {
		tv = &start_tv;
	}

	ocall_gettimeofday(tv);
	//(void) gettimeofday( &tv, (struct timezone *) 0);
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

		assert(tdiff->tv_usec >= 0);
	}

	/* time shouldn't go backwards!!! */
	if (tdiff->tv_usec < 0 || t1->tv_sec < t0->tv_sec) {
		tdiff->tv_sec = 0;
		tdiff->tv_usec = 0;
	}
}

void print_time(uint64 time)
{
	double secs = (double)time / 1000000.0;

	if (secs <= 0.)
		return;

	if (secs < 1.) {
		printf("\tFILE: %s:%d \t %.6f secs\n", __FILE__, __LINE__, secs);
	} else {
		printf("\tFILE: %s:%d \t %.2f secs\n", __FILE__, __LINE__, secs);
	}
}

/*###########################################################################*/
/*##############################  ENCLAVE functions  ########################*/
/*###########################################################################*/

int ecall_initialise()
{
	int rv = NL_STATUS_OK;

	if (!get_enclave(NULL))
		rv = NL_STATUS_SGXERROR;

	return rv;
}

int get_enclave(sgx_enclave_id_t *eid)
{
	if (launched)
		return launched;
	else
		return create_enclave(NULL);
}

int create_enclave(sgx_enclave_id_t *eid)
{

	sgx_status = sgx_create_enclave(ENCLAVE_FILENAME, SGX_DEBUG_FLAG,
									&token, &updated, &enclaveID, NULL);

	if (sgx_status == SGX_SUCCESS) {
		if (eid != NULL)
			*eid = enclaveID;
		launched = LAUNCHED;
		return 1;
	}

	printf("%s LINE: %d ERROR %d, failed to create enclave.\n",
		   __FILE__, __LINE__, sgx_status);
	return 0;
}
