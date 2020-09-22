/*
 * $Id$
 */
#ifndef _BENCH_H
#define _BENCH_H

#include <stdio.h>
#include <string.h>

#include <stdlib.h>

#include <sys/types.h>

#ifndef HAVE_uint
typedef unsigned int uint;
#endif

#ifndef HAVE_uint64
#ifdef HAVE_uint64_t
typedef uint64_t uint64;
#else  /* HAVE_uint64_t */
typedef unsigned long long uint64;
#endif /* HAVE_uint64_t */
#endif /* HAVE_uint64 */

#ifndef HAVE_int64
#ifdef HAVE_int64_t
typedef int64_t int64;
#else  /* HAVE_int64_t */
typedef long long int64;
#endif /* HAVE_int64_t */
#endif /* HAVE_int64 */

// #define NO_PORTMAPPER

#include "Enclave_t.h"
#include "timing.h"

#ifdef DEBUG
#define debug(x) fprintf x
#else
#define debug(x)
#endif

#define BUFSIZE 4096

#define streq !strcmp
#define ulong unsigned long

#define REAL_SHORT 50000
#define SHORT 1000000
#define MEDIUM 2000000
#define LONGER 7500000 /* for networking data transfers */
#define ENOUGH REAL_SHORT

#define TRIES 11

typedef struct {
	uint64 u;
	uint64 n;
} value_t;

typedef struct {
	int N;
	value_t v[TRIES];
} result_t;

int sizeof_result(int N);
void insertinit(result_t *r);
void insertsort(uint64, uint64, result_t *);
void save_median();
void save_minimum();
void set_results(result_t *r);
result_t *get_results();

#endif /* _BENCH_H */
