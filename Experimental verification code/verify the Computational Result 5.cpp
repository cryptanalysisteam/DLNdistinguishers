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

double GetAverageABS(double *array, uint64_t NS) {
	double sum = 0.0;
	#pragma omp parallel for reduction(+:sum)
	for (uint64_t i = 0; i < NS; i++) {
		sum += fabs(array[i]);
	}
	return sum / NS;
}

int main() {
	printf("For the Computational Result 5, we randomly generated 2^{2} keys and 2^{46} IVs\n");
	const uint64_t NK = ((uint64_t)1 << 2);
	const uint64_t NIV = ((uint64_t)1 << 46);
	double res = 0.0;
	double cor[NK] = {0};
	double start_time = omp_get_wtime();

	for (uint64_t iKey = 0; iKey < NK; iKey++) {
		uint64_t global_ctr = 0;

		#pragma omp parallel reduction(+:global_ctr)
		{
			xorshift128plus_state rng;
			init_xorshift128plus(&rng, time(NULL) ^ (omp_get_thread_num() * 0x9E3779B97F4A7C15ULL) ^ iKey);
			uint32_t X[16];

			X[0] = 0x61707865;
			X[1] = 0x3320646e;
			X[2] = 0x79622d32;
			X[3] = 0x6b206574;

			for (int i = 4; i < 12; i++) {
				X[i] = (uint32_t)xorshift128plus(&rng);
			}


			#pragma omp for nowait
			for (uint64_t iIV = 0; iIV < NIV; iIV++) {

				for (int i = 12; i < 16; i++) {
					X[i] = (uint32_t)xorshift128plus(&rng);
				}

				int input = 0;
				int output = 0;

				QR(X[0], X[4], X[8], X[12]);
				QR(X[1], X[5], X[9], X[13]);
				QR(X[2], X[6], X[10], X[14]);
				QR(X[3], X[7], X[11], X[15]);

				QR(X[0], X[5], X[10], X[15]);
				QR(X[1], X[6], X[11], X[12]);
				QR(X[2], X[7], X[8], X[13]);
				QR(X[3], X[4], X[9], X[14]);

				QR(X[0], X[4], X[8], X[12]);
				QR(X[1], X[5], X[9], X[13]);
				QR(X[2], X[6], X[10], X[14]);
				QR(X[3], X[7], X[11], X[15]);

				QR(X[0], X[5], X[10], X[15]);
				QR(X[1], X[6], X[11], X[12]);
				QR(X[2], X[7], X[8], X[13]);
				QR(X[3], X[4], X[9], X[14]);

				QR(X[0], X[4], X[8], X[12]);
				QR(X[1], X[5], X[9], X[13]);
				QR(X[2], X[6], X[10], X[14]);
				QR(X[3], X[7], X[11], X[15]);


				input =
				    (X[2]>>0)&0x1 ^ (X[6]>>19)&0x1 ^ (X[6]>>7)&0x1 ^ (X[10]>>12)&0x1 ^ (X[14]>>0)&0x1;

				QR(X[0], X[5], X[10], X[15]);
				QR(X[1], X[6], X[11], X[12]);
				QR(X[2], X[7], X[8], X[13]);
				QR(X[3], X[4], X[9], X[14]);

				QR(X[0], X[4], X[8], X[12]);
				QR(X[1], X[5], X[9], X[13]);
				QR(X[2], X[6], X[10], X[14]);
				QR(X[3], X[7], X[11], X[15]);

				output =
				    (X[0]>>23)&0x1 ^ (X[0]>>19)&0x1 ^ (X[0]>>7)&0x1 ^ (X[0]>>3)&0x1 ^ (X[0]>>2)&0x1 ^
				    (X[1]>>16)&0x1 ^
				    (X[2]>>24)&0x1 ^ (X[2]>>23)&0x1 ^ (X[2]>>12)&0x1 ^ (X[2]>>8)&0x1 ^ (X[2]>>0)&0x1 ^
				    (X[3]>>31)&0x1 ^ (X[3]>>28)&0x1 ^ (X[3]>>20)&0x1 ^ (X[3]>>16)&0x1 ^ (X[3]>>12)&0x1 ^ (X[3]>>11)&0x1 ^ (X[3]>>7)&0x1 ^ (X[3]>>4)&0x1 ^ (X[3]>>3)&0x1 ^ (X[3]>>0)&0x1 ^
				    (X[4]>>31)&0x1 ^ (X[4]>>19)&0x1 ^ (X[4]>>14)&0x1 ^ (X[4]>>13)&0x1 ^
				    (X[5]>>7)&0x1 ^
				    (X[6]>>31)&0x1 ^ (X[6]>>25)&0x1 ^ (X[6]>>19)&0x1 ^ (X[6]>>13)&0x1 ^ (X[6]>>7)&0x1 ^
				    (X[7]>>27)&0x1 ^ (X[7]>>26)&0x1 ^ (X[7]>>23)&0x1 ^ (X[7]>>7)&0x1 ^ (X[7]>>6)&0x1 ^ (X[7]>>3)&0x1 ^
				    (X[8]>>24)&0x1 ^ (X[8]>>12)&0x1 ^
				    (X[10]>>26)&0x1 ^ (X[10]>>24)&0x1 ^ (X[10]>>18)&0x1 ^
				    (X[11]>>28)&0x1 ^ (X[11]>>20)&0x1 ^
				    (X[12]>>31)&0x1 ^ (X[12]>>20)&0x1 ^ (X[12]>>19)&0x1 ^ (X[12]>>11)&0x1 ^ (X[12]>>10)&0x1 ^ (X[12]>>7)&0x1 ^
				    (X[13]>>24)&0x1 ^ (X[13]>>8)&0x1 ^ (X[13]>>0)&0x1 ^
				    (X[14]>>26)&0x1 ^ (X[14]>>25)&0x1 ^ (X[14]>>20)&0x1 ^ (X[14]>>16)&0x1 ^ (X[14]>>12)&0x1 ^ (X[14]>>11)&0x1 ^ (X[14]>>6)&0x1 ^ (X[14]>>5)&0x1 ^ (X[14]>>0)&0x1 ^
				    (X[15]>>31)&0x1 ^ (X[15]>>19)&0x1 ^ (X[15]>>18)&0x1 ^ (X[15]>>16)&0x1 ^ (X[15]>>12)&0x1 ^ (X[15]>>11)&0x1 ^ (X[15]>>7)&0x1 ^ (X[15]>>4)&0x1 ^ (X[15]>>0)&0x1;

				    if (input == output) global_ctr++;
			}
		}

		cor[iKey] = 2.0 * ((double)global_ctr / (double)NIV) - 1.0;
	}

	res = GetAverageABS(cor, NK);
//	for (int i = 0; i < NK; i++) {
//		printf("%d res = %f 2^%f\n", i, cor[i], log2(fabs(cor[i])));
//	}
	printf("Computational Result 5 is %f(2^%f)\n",res,log2(abs(res)));
	double end_time = omp_get_wtime();
	printf("Total execution time: %.4f seconds", end_time - start_time);
	system("pause");
	return 0;
}
