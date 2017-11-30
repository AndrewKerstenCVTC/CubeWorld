#ifndef __SIMPLEX_H__
#define __SIMPLEX_H__

/*
* OpenSimplex (Simplectic) Noise in C.
* Ported to C from Kurt Spencer's java implementation by Stephen M. Cameron
*
* v1.1 (October 6, 2014)
* - Ported to C
*
* v1.1 (October 5, 2014)
* - Added 2D and 4D implementations.
* - Proper gradient sets for all dimensions, from a
*   dimensionally-generalizable scheme with an actual
*   rhyme and reason behind it.
* - Removed default permutation array in favor of
*   default seed.
* - Changed seed-based constructor to be independent
*   of any particular randomization library, so results
*   will be the same when ported to other languages.
*/
#include <stdint.h>

struct osn_context;

int simplex(int64_t seed, struct osn_context **ctx);
void simplex_free(struct osn_context *ctx);
int simplex_init_perm(struct osn_context *ctx, int16_t p[], int nelements);
double simplex2(struct osn_context *ctx, double x, double y);
double simplex3(struct osn_context *ctx, double x, double y, double z);
double simplex4(struct osn_context *ctx, double x, double y, double z, double w);

#endif
