#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <omp.h>

#define ROTL(a,b) (((a) << (b)) | ((a) >> (32 - (b))))
#define QR(a, b, c, d) (                    \
        a += b,  d ^= a,  d = ROTL(d,16),   \
        c += d,  b ^= c,  b = ROTL(b,12),   \
        a += b,  d ^= a,  d = ROTL(d, 8),   \
        c += d,  b ^= c,  b = ROTL(b, 7))


typedef struct {
	uint64_t s[2];
} xorshift128plus_state;

static inline uint64_t xorshift128plus(xorshift128plus_state *state) {  
	uint64_t x = state->s[0];
	uint64_t const y = state->s[1];
	state->s[0] = y;
	x ^= x << 23;
	state->s[1] = x ^ y ^ (x >> 17) ^ (y >> 26);
	return state->s[1] + y;
}

void init_xorshift128plus(xorshift128plus_state *state, uint64_t seed) {
	state->s[0] = seed ^ 0x6A09E667F3BCC909;
	state->s[1] = seed ^ 0xBB67AE8584CAA73B;
	for (int i = 0; i < 10; i++) {
		xorshift128plus(state);
	}
}

int main() {
	printf("For the practical DLN distinguisher of 6-round ChaCha256, we randomly generated 2^{46} samples\n");
	const uint64_t NIV = ((uint64_t)1 << 46);
	double res = 0.0;
	uint64_t global_ctr = 0;
	double start_time = omp_get_wtime();

	#pragma omp parallel reduction(+:global_ctr)
	{
		xorshift128plus_state rng;
		init_xorshift128plus(&rng, time(NULL) ^ (omp_get_thread_num() * 0x9E3779B97F4A7C15ULL));
		uint32_t X[16], Y[16], OD[16];

		X[0] = 0x61707865;
		X[1] = 0x3320646e;
		X[2] = 0x79622d32;
		X[3] = 0x6b206574;

		#pragma omp for nowait
		for(uint64_t iIV = 0; iIV < NIV; iIV++) {

			for(int i = 4; i < 12; i++) {
				X[i] = (uint32_t)xorshift128plus(&rng);
			}
			for(int i = 12; i < 16; i++) {
				X[i] = (uint32_t)xorshift128plus(&rng);
			}

			for(int i = 0; i < 16; i++) {
				Y[i] = X[i];
			}
			Y[14] = Y[14] ^ 0x00000040;

			QR(X[0], X[4], X[ 8], X[12]);
			QR(X[1], X[5], X[ 9], X[13]);
			QR(X[2], X[6], X[10], X[14]);
			QR(X[3], X[7], X[11], X[15]);

			QR(X[0], X[5], X[10], X[15]);
			QR(X[1], X[6], X[11], X[12]);
			QR(X[2], X[7], X[ 8], X[13]);
			QR(X[3], X[4], X[ 9], X[14]);

			QR(X[0], X[4], X[ 8], X[12]);
			QR(X[1], X[5], X[ 9], X[13]);
			QR(X[2], X[6], X[10], X[14]);
			QR(X[3], X[7], X[11], X[15]);

			QR(X[0], X[5], X[10], X[15]);
			QR(X[1], X[6], X[11], X[12]);
			QR(X[2], X[7], X[ 8], X[13]);
			QR(X[3], X[4], X[ 9], X[14]);

			QR(X[0], X[4], X[ 8], X[12]);
			QR(X[1], X[5], X[ 9], X[13]);
			QR(X[2], X[6], X[10], X[14]);
			QR(X[3], X[7], X[11], X[15]);

			QR(X[0], X[5], X[10], X[15]);
			QR(X[1], X[6], X[11], X[12]);
			QR(X[2], X[7], X[ 8], X[13]);
			QR(X[3], X[4], X[ 9], X[14]);

			QR(Y[0], Y[4], Y[ 8], Y[12]);
			QR(Y[1], Y[5], Y[ 9], Y[13]);
			QR(Y[2], Y[6], Y[10], Y[14]);
			QR(Y[3], Y[7], Y[11], Y[15]);

			QR(Y[0], Y[5], Y[10], Y[15]);
			QR(Y[1], Y[6], Y[11], Y[12]);
			QR(Y[2], Y[7], Y[ 8], Y[13]);
			QR(Y[3], Y[4], Y[ 9], Y[14]);

			QR(Y[0], Y[4], Y[ 8], Y[12]);
			QR(Y[1], Y[5], Y[ 9], Y[13]);
			QR(Y[2], Y[6], Y[10], Y[14]);
			QR(Y[3], Y[7], Y[11], Y[15]);

			QR(Y[0], Y[5], Y[10], Y[15]);
			QR(Y[1], Y[6], Y[11], Y[12]);
			QR(Y[2], Y[7], Y[ 8], Y[13]);
			QR(Y[3], Y[4], Y[ 9], Y[14]);

			QR(Y[0], Y[4], Y[ 8], Y[12]);
			QR(Y[1], Y[5], Y[ 9], Y[13]);
			QR(Y[2], Y[6], Y[10], Y[14]);
			QR(Y[3], Y[7], Y[11], Y[15]);

			QR(Y[0], Y[5], Y[10], Y[15]);
			QR(Y[1], Y[6], Y[11], Y[12]);
			QR(Y[2], Y[7], Y[ 8], Y[13]);
			QR(Y[3], Y[4], Y[ 9], Y[14]);

			for(int i = 0; i < 16; i++) {
				OD[i] = X[i] ^ Y[i];
			}

			int output = 0;
	

			output = (OD[0]>>16)&0x1 ^ (OD[0]>>0)&0x1 ^
			         (OD[1]>>23)&0x1 ^ (OD[1]>>12)&0x1 ^ (OD[1]>>11)&0x1 ^ (OD[1]>>7)&0x1 ^ (OD[1]>>0)&0x1 ^
			         (OD[2]>>24)&0x1 ^ (OD[2]>>19)&0x1 ^ (OD[2]>>18)&0x1 ^ (OD[2]>>16)&0x1 ^ (OD[2]>>8)&0x1 ^ (OD[2]>>7)&0x1 ^ (OD[2]>>0)&0x1 ^
			         (OD[4]>>19)&0x1 ^ (OD[4]>>13)&0x1 ^ (OD[4]>>7)&0x1 ^
			         (OD[5]>>7)&0x1 ^
			         (OD[6]>>19)&0x1 ^ (OD[6]>>14)&0x1 ^ (OD[6]>>7)&0x1 ^
			         (OD[7]>>26)&0x1 ^ (OD[7]>>15)&0x1 ^ (OD[7]>>7)&0x1 ^ (OD[7]>>6)&0x1 ^
			         (OD[8]>>31)&0x1 ^ (OD[8]>>19)&0x1 ^ (OD[8]>>8)&0x1 ^ (OD[8]>>0)&0x1 ^
			         (OD[9]>>26)&0x1 ^ (OD[9]>>12)&0x1 ^ (OD[9]>>6)&0x1 ^ (OD[9]>>0)&0x1 ^
			         (OD[10]>>0)&0x1 ^
			         (OD[11]>>7)&0x1 ^
			         (OD[12]>>31)&0x1 ^ (OD[12]>>20)&0x1 ^ (OD[12]>>19)&0x1 ^ (OD[12]>>12)&0x1 ^ (OD[12]>>0)&0x1 ^
			         (OD[13]>>27)&0x1 ^ (OD[13]>>26)&0x1 ^ (OD[13]>>24)&0x1 ^ (OD[13]>>15)&0x1 ^ (OD[13]>>0)&0x1 ^
			         (OD[14]>>26)&0x1 ^ (OD[14]>>8)&0x1 ^
			         (OD[15]>>24)&0x1 ^
			         (((OD[1]>>6)&0x1) ^ 1) & (((OD[6]>>13)&0x1) ^ ((OD[11]>>6)&0x1)) ^
			         (((OD[1]>>6)&0x1) ^ ((OD[6]>>13)&0x1) ^ ((OD[11]>>6)&0x1) ^ 1) & (((OD[1]>>5)&0x1) ^ 1) & (((OD[6]>>12)&0x1) ^ ((OD[11]>>5)&0x1)) ^
			         (((OD[11]>>11)&0x1) ^ 1) & ((OD[12]>>11)&0x1) ^
			         (((OD[11]>>11)&0x1) ^ ((OD[12]>>11)&0x1) ^ 1) & (((OD[11]>>10)&0x1) ^ 1) & ((OD[12]>>10)&0x1) ^
			         (((OD[2]>>7)&0x1) ^ 1) & (((OD[7]>>14)&0x1) ^ ((OD[8]>>7)&0x1)) ^
			         (((OD[2]>>7)&0x1) ^ ((OD[7]>>14)&0x1) ^ ((OD[8]>>7)&0x1) ^ 1) & (((OD[2]>>6)&0x1) ^ 1) & (((OD[7]>>13)&0x1) ^ ((OD[8]>>6)&0x1)) ^
			         (((OD[9]>>25)&0x1) ^ 1) & ((OD[14]>>25)&0x1) ^
			         (((OD[9]>>25)&0x1) ^ ((OD[14]>>25)&0x1) ^ 1) & (((OD[9]>>24)&0x1) ^ 1) & ((OD[14]>>24)&0x1);
			if(output == 0) global_ctr++;
		}

	}
	double end_time = omp_get_wtime();
	printf("Total execution time: %.4f seconds\n", end_time - start_time);

	res = 2.0 * ((double)global_ctr / (double)NIV) - 1.0;
	printf("Experimental result is p = %f(2^%f)\n", res, log2(fabs(res)));
	system("pause");

	return 0;
}


