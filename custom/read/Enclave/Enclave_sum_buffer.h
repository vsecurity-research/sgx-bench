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

void ecall_send_params(int inbufsz, int loop);
void init(int inbufsz, int *mybuf, int *lastone);
int sum_buffer();

#endif /* !_ENCLAVE_H_ */
