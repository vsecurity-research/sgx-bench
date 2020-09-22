/*
 * |Aisha Hasan|
 *
 * The |Enclave/Enclave_timing.h| file.
 *
 *
 *
 */

#ifndef _TIMING_H
#define _TIMING_H

void start(struct mytimeval *tv);
uint64 stop(struct mytimeval *begin, struct mytimeval *end);

uint64 tvdelta(struct mytimeval *start, struct mytimeval *stop);
void tvsub(struct mytimeval *tdiff, struct mytimeval *t1, struct mytimeval *t0);

void use_int(int result);

uint64 gettime(void);

#endif /* _TIMING_H */
