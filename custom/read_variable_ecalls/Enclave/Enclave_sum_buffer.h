/*
 * Aisha Hasan
 *
 * Header file for |Enclave_sum_buffer.c|
 *
 */

#ifndef _ENCLAVE_H_
#define _ENCLAVE_H_

#include "bench.h"
#include "sgx_trts.h"

void ecall_send_params(int inbufsz);
int ecall_get_sum(int index, int numints);

void init(int inbufsz, int *mybuf, int *lastone);

#endif /* !_ENCLAVE_H_ */
