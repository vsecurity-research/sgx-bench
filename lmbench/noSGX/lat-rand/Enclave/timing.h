/*
 * |Aisha Hasan|
 *
 * The |Enclave/Enclave_timing.h| file.
 *
 *
 *
 */

#ifndef _TIMING_H_
#define _TIMING_H_

// #include "bench.h"
#include "../Include/common_types.h"
// #include "../App/EnclaveBridge.h"
// #include "Enclave_t.h"




// // char	*p64(uint64 big);
// // char	*p64sz(uint64 big);
// // double	Delta(void);
// // double	Now(void);
// // void	adjust(int usec);
// // void	bandwidth(uint64 bytes, uint64 times, int verbose);
// // uint64	bytes(char *s);
// // void	context(uint64 xfers);
// // uint64	delta(void);
int	get_enough(int);
uint64	get_n(void);
// // void	kb(uint64 bytes);
double	l_overhead(void);
// // char	last(char *s);
// // void	latency(uint64 xfers, uint64 size);
// // void	mb(uint64 bytes);
// // void	micro(char *s, uint64 n);
// // void	micromb(uint64 mb, uint64 n);
// // void	milli(char *s, uint64 n);
// // void	morefds(void);
void	nano(char *s, uint64 n);
// // uint64	now(void);
// // void	ptime(uint64 n);
// // void	rusage(void);
void	save_n(uint64);
void	settime(uint64 usecs);
void	start(struct mytimeval *tv);
uint64	stop(struct mytimeval *begin, struct mytimeval *end);
uint64	t_overhead(void);
double	timespent(void);
// // void	timing(FILE *out);
uint64	tvdelta(struct mytimeval *start, struct mytimeval *stop);
void	tvsub(struct mytimeval *tdiff, struct mytimeval *t1, struct mytimeval *t0);
void	use_int(int result);
void	use_pointer(void *result);
uint64	usecs_spent(void);
uint64	gettime(void);
// // void	touch(char *buf, int size);
// // size_t*	permutation(int max, int scale);
// // int	cp(char* src, char* dst, mode_t mode);
// // long	bread(void* src, long count);

// // #if defined(hpux) || defined(__hpux)
// // int	getpagesize();
// // #endif

#endif /* _TIMING_H_ */
