/* 
 * |Aisha Hasan|
 * 
 * The |App/EnclaveBridge.h| file.
 * 
 * 
 * 
 * 
 */
#ifndef _ENCLAVE_BRIDGE_H_
#define _ENCLAVE_BRIDGE_H_

#include "Enclave_u.h"

#include <assert.h>
#include <stdio.h>
#include <sys/time.h>

#include "LMBenchError.h"
#include "sgx_eid.h"   /* sgx_enclave_id_t */
#include "sgx_error.h" /* sgx_status_t */
#include "sgx_urts.h"  /* sgx_launch_token_t */

#define NOT_LAUNCHED 0
#define LAUNCHED 1
#define ENCLAVE_FILENAME "enclave.signed.so"

#ifndef HAVE_uint64
#ifdef HAVE_uint64_t
typedef uint64_t uint64;
#else  /* HAVE_uint64_t */
typedef unsigned long long uint64;
#endif /* HAVE_uint64_t */
#endif /* HAVE_uint64 */

static sgx_enclave_id_t enclaveID = 4;
static int launched = NOT_LAUNCHED;
static int updated = 0;
static sgx_launch_token_t token = {0};
static sgx_status_t sgx_status = SGX_SUCCESS;

void send_params(int inbufsize, int outbufsize,
				 int inloops, int outloops, int numintsread);

void init(int outbufsz, int *mybuf, int *lastone);

void ocall_gettimeofday(struct mytimeval *tv);
void start(struct mytimeval *tv);
uint64 stop(struct mytimeval *begin, struct mytimeval *end);

uint64 tvdelta(struct mytimeval *start, struct mytimeval *stop);
void tvsub(struct mytimeval *tdiff, struct mytimeval *t1, struct mytimeval *t0);
void print_time(uint64 time);

int ecall_initialise(void);
int get_enclave(sgx_enclave_id_t *eid);
int create_enclave(sgx_enclave_id_t *eid);

#endif /* !_ENCLAVE_BRIDGE_H_ */
