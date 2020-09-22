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

#include <stdio.h>
#include <sys/time.h>

#include "LMBenchError.h"
#include "sgx_eid.h"   /* sgx_enclave_id_t */
#include "sgx_error.h" /* sgx_status_t */
#include "sgx_urts.h"  /* sgx_launch_token_t */

#define NOT_LAUNCHED 0
#define LAUNCHED 1
#define ENCLAVE_FILENAME "enclave.signed.so"

static sgx_enclave_id_t enclaveID = 4;
static int launched = NOT_LAUNCHED;
static int updated = 0;
static sgx_launch_token_t token = {0};
static sgx_status_t sgx_status = SGX_SUCCESS;

void send_params(int bufsz, int outbufsz, int numloops);
void init(int outbufsz, int *mybuf, int *lastone);
void sum_all_loops(int bufsz, int outbufsz);
int sum_buffer();
void ocall_sum_buffer();

int ecall_initialise(void);
int get_enclave(sgx_enclave_id_t *eid);
int create_enclave(sgx_enclave_id_t *eid);

#endif /* !_ENCLAVE_BRIDGE_H_ */
