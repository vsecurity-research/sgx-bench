/* 
 * |Aisha Hasan|
 * 
 * The |App/EnclaveBridge.c| file.
 */

#include "EnclaveBridge.h"

#include <sys/stat.h>

#define DEBUG_PRINT 0

#define debug_print(fmt, ...)                                   \
	do {                                                        \
		if (DEBUG_PRINT)                                        \
			fprintf(stderr, "%s %s():%d: \t" fmt,               \
					__FILE__, __func__, __LINE__, __VA_ARGS__); \
	} while (0)

/*
#ifdef DEBUG
#define debug_print(fmt, ...) do { fprintf(stderr, fmt, __VA_ARGS__); } while (0)
#else
#define debug_print(fmt, ...) do {} while (0)
#endif
*/

static volatile unsigned long long use_result_dummy;
void use_int(int result) { use_result_dummy += result; }

/*###############################  OCALL functions  #########################*/
void ocall_print_string(const char *str)
{
	printf("%s\n", str);
	fflush(stdin);
}

void ocall_gettimeofday(struct mytimeval *tv)
{

	struct timeval temptv;

	(void)gettimeofday(&temptv, (struct timezone *)0);
	tv->tv_sec = temptv.tv_sec;
	tv->tv_usec = temptv.tv_usec;
}

/*##############################  Global variables  #########################*/
int *mybuf;
int *lastone;
int loops = 500, outloops, inloops;
double percent = 0.5;
int roundto = 10;

/*
 * 1. Initialise enclave.
 * 2. Calculate buffer size (in + out = total).
 * 3. Allocate space for the outside buffer.
 * 4. Initialise outer buffer. Done once, as summing doesn't change
 *    the values in the buffer.
 * 5.
 * 
 **/
void send_params(int bufsz, int outbufsz, int numloops)
{

	int rv = NL_STATUS_OK;
	rv = ecall_initialise();

	printf("%s LINE:%d new enclaveID: [%lu].\n", __FILE__, __LINE__, enclaveID);
	loops = numloops;

	//malloc outer buffer
	debug_print("outbufsize:[%d]\n", outbufsz);

	mybuf = (int *)malloc(outbufsz);
	lastone = (int *)((char *)mybuf + outbufsz - sizeof(int));

	if (!mybuf) {
		printf("%s LINE:%d\n\tmalloc error.\n", __FILE__, __LINE__);
	}

	//initialise outer buffer
	init(outbufsz, mybuf, lastone);
	debug_print("after init: mybuf[0]=[%d] lastone=[%d].\n",
				mybuf[0], *lastone);

	sum_all_loops(bufsz, outbufsz);

	printf("%s LINE:%d destroy enclaveID: [%lu].\n",
		   __FILE__, __LINE__, enclaveID);

	printf("%s LINE:%d %s destroyed enclaveID: [%lu].\n",
		   __FILE__, __LINE__,
		   (sgx_destroy_enclave(enclaveID) != SGX_SUCCESS) ? "not" : "", enclaveID);
}

/*###########################################################################*/
/*##########################  SUMMING functions  ##########################*#*/
/*###########################################################################*/

/*
 * Fill buffer with 1s.
 */
void init(int outbufsz, int *mybuf, int *lastone)
{
	debug_print("inside init: outbufsz[%d], num_ints[%lu].\n",
				outbufsz, outbufsz / sizeof(int));

	for (int i = 0; i < outbufsz / sizeof(int); i++) {
		mybuf[i] = 1; //i+1;
	}
}

/*
 * Calls ecall_send_params(), which will call summing functions.
 *
 * - The proportion of iterations run inside vs. outside changes with each
 *	 iteration of the for loop.
 * - The iterations are rounded to the nearest 'chunk'. This accounts for
 *	 +1/-1 due to multiplication by (double)percent.
 * - Iterations inside/outisde + total are printed, as well as the proportion
 *	 of iterations in:out.
 * - (Those values should be carefully checked at runtime!)
 */
void sum_all_loops(int bufsz, int outbufsz)
{

	for (percent = 0.0; percent <= 1.0; percent += 0.1) {

		debug_print("percent=%.2f\n", percent);

		outloops = (int)(percent * loops);
		outloops = (outloops % roundto >= roundto / 2) ? (outloops + roundto - outloops % roundto) : (outloops - outloops % roundto);

		if (outloops > loops)
			outloops = loops;
		inloops = loops - outloops;

		//start sum loops
		printf("\t in(%d) + out(%d) = (%d) in(%.2f)-out(%.2f)\n",
			   inloops, outloops, loops, 1 - percent, percent);
		ecall_send_params(enclaveID, bufsz - outbufsz, inloops);
	}
}

/*
 * This function mirrors sum_buffer() inside the Enclave.
 */
int sum_buffer()
{

	int sum = 0;
	int *tempptr = mybuf;

	while (tempptr <= lastone) {
		sum += *tempptr;
		tempptr++;
	}
	use_int(sum);
	return sum;
}

/*
 * Mirrors loop right after start(0) inside the enclave in ecall_send_params()
 */
void ocall_sum_buffer()
{
	int sum = 0;

	for (int i = 0; i < outloops; i++) {
		sum = sum_buffer();
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
