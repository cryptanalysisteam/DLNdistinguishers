#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <random>
#include <algorithm>
using namespace std;

#define ROTL(a,b) (((a) << (b)) | ((a) >> (32 - (b))))
#define QR(a, b, c, d) (					\
        a += b,  d ^= a,  d = ROTL(d,16),	\
        c += d,  b ^= c,  b = ROTL(b,12),	\
        a += b,  d ^= a,  d = ROTL(d, 8),	\
        c += d,  b ^= c,  b = ROTL(b, 7))
#define HALF_QR1(a, b, c, d) (			\
                                        a += b,  d ^= a,  d = ROTL(d,16),	\
                                        c += d,  b ^= c,  b = ROTL(b,12))
#define HALF_QR2(a, b, c, d) (			\
                                        a += b,  d ^= a,  d = ROTL(d,8),	\
                                        c += d,  b ^= c,  b = ROTL(b,7))


#define GET_BIT(a,p) (((a)>>(p))&0x1)
mt19937 gen((unsigned int) time(NULL));

double GetAverageABS(double *array, uint64_t NS) {
	double sum = 0.0;
	for(uint64_t i = 0; i < NS; i++) {
		sum += fabs(array[i]);
	}
	return sum / NS;
}

int calculate_single(uint32_t x, uint32_t y, int i) {
	return GET_BIT(x, i) ^ GET_BIT(y, i) ^
	       GET_BIT(x, i-1) * GET_BIT(y, i-1) ^
	       GET_BIT(x, i-1) * GET_BIT(x, i-2) * GET_BIT(y, i-2) ^
	       GET_BIT(y, i-1) * GET_BIT(x, i-2) * GET_BIT(y, i-2);
}

int calculate_two(uint32_t x, uint32_t y, int i) {
	return GET_BIT(x, i) ^ GET_BIT(y, i) ^
	       GET_BIT(x, i-1) ^ GET_BIT(y, i-1) ^
	       GET_BIT(x, i-1) * GET_BIT(y, i-1) ^
	       GET_BIT(x, i-2) * GET_BIT(y, i-2) ^
	       GET_BIT(x, i-1) * GET_BIT(x, i-2) * GET_BIT(y, i-2) ^
	       GET_BIT(y, i-1) * GET_BIT(x, i-2) * GET_BIT(y, i-2);
}

int calculate_three(uint32_t x, uint32_t y, int i) {
	return GET_BIT(x, i) ^ GET_BIT(y, i) ^
	       GET_BIT(x, i-1) ^ GET_BIT(y, i-1) ^
	       GET_BIT(x, i-2) ^ GET_BIT(y, i-2) ^
	       GET_BIT(x, i-1) * GET_BIT(y, i-1) ^
	       GET_BIT(x, i-2) * GET_BIT(y, i-2) ^
	       GET_BIT(x, i-1) * GET_BIT(x, i-2) * GET_BIT(y, i-2) ^
	       GET_BIT(y, i-1) * GET_BIT(x, i-2) * GET_BIT(y, i-2);
}



double epsilon(int i) {
	if (i == 0 || i == 1) {
		return 1.0;
	} else if (i > 1) {
		return 0.75 + pow(2.0, -i);
	} else {
		return 0.0;
	}
}
double epsilon3(int i) {
	if (i == 0 || i == 1) {
		return 1.0;
	} else if (i > 1) {
		return 0.25 + 3*pow(2.0, -i);
	} else {
		return 0.0;
	}
}


int main() {
	printf("For the Computational Result 6, we randomly generated 2^{6} keys and 2^{32} IVs\n");
	uint64_t NK;
	uint64_t NIV;
	NK=((uint64_t)1<<6);
	NIV=((uint64_t)1<<32);

	double res = 0.0;
	double cor[NK] = { 0 };


	for(uint64_t iKey = 0; iKey < NK; iKey++) {
		uint64_t ctr[NK] = { 0 };


		uint32_t X[16] = { 0 };
		X[0] = 0x61707865;
		X[1] = 0x3320646e;
		X[2] = 0x79622d32;
		X[3] = 0x6b206574;

		for(int i = 4; i < 12; i++) {
			X[i] = gen();
		}

		for(uint64_t iIV = 0; iIV < NIV; iIV++) {
			for(int i = 12; i < 16; i++) {
				X[i] = gen();
			}

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

			QR(X[0], X[4], X[ 8], X[12]);
			QR(X[1], X[5], X[ 9], X[13]);
			QR(X[2], X[6], X[10], X[14]);
			QR(X[3], X[7], X[11], X[15]);

			int input = 0;

			input = input ^ GET_BIT(X[0], 23) ^ GET_BIT(X[0], 19) ^ GET_BIT(X[0], 7) ^ GET_BIT(X[0], 3) ^ GET_BIT(X[0], 2) ^
			        GET_BIT(X[10], 26) ^ GET_BIT(X[10], 24) ^ GET_BIT(X[10], 18) ^ GET_BIT(X[1], 16) ^ GET_BIT(X[11], 28) ^ GET_BIT(X[11], 20) ^
			        GET_BIT(X[2], 12) ^ GET_BIT(X[2], 8) ^ GET_BIT(X[2], 24) ^ GET_BIT(X[2], 23) ^ GET_BIT(X[8], 24) ^ GET_BIT(X[8], 12) ^  
			        GET_BIT(X[3], 31) ^ GET_BIT(X[3], 28) ^ GET_BIT(X[3], 20) ^
			        GET_BIT(X[3], 16) ^ GET_BIT(X[3], 7) ^ GET_BIT(X[3], 12) ^ GET_BIT(X[3], 11) ^ GET_BIT(X[3], 4) ^ GET_BIT(X[3], 3);


			int Xb1 = X[5];
			int Xb2 = X[6];
			int Xb3 = X[7];
			int Xb4 = X[4];

			HALF_QR1(X[0], X[5], X[10], X[15]);
			HALF_QR1(X[1], X[6], X[11], X[12]);
			HALF_QR1(X[2], X[7], X[ 8], X[13]);
			HALF_QR1(X[3], X[4], X[ 9], X[14]);

			int output = 0;

			output = output ^ calculate_single(X[0], -Xb1, 23) ^ calculate_single(X[0], -Xb1, 19) ^ calculate_single(X[0], -Xb1, 7) ^ calculate_two(X[0], -Xb1, 3) ^
			         calculate_single(X[10], -X[15], 26)  ^ calculate_single(X[10], -X[15], 24) ^ calculate_single(X[10], -X[15], 18) ^  calculate_single(X[1], -Xb2, 16) ^ 
					 calculate_single(X[11], -X[12], 28) ^ calculate_single(X[11], -X[12], 20) ^
			         calculate_single(X[2], -Xb3, 12) ^ calculate_single(X[2], -Xb3, 8) ^ calculate_two(X[2], -Xb3, 24) ^ calculate_single(X[8], -X[13], 24) ^ calculate_single(X[8], -X[13], 12) ^
			         calculate_single(X[3], -Xb4, 31) ^ calculate_single(X[3], -Xb4, 28) ^ calculate_single(X[3], -Xb4, 20) ^ calculate_single(X[3], -Xb4, 16) ^
			         calculate_single(X[3], -Xb4, 7) ^ calculate_two(X[3], -Xb4, 12) ^ calculate_two(X[3], -Xb4, 4);



			if(input == output) ctr[iKey]++;
		}

		cor[iKey] = 2.0*((double)ctr[iKey]/(double)NIV)-1.0;
	}
	res = GetAverageABS(cor,NK);
//	for(int i = 0; i < NK; i++) {
//		printf("%d res = %f 2^%f\n",i,cor[i],log2(abs(cor[i])));
//	}

	printf("Experimental results is %f(2^%f)\n",res,log2(abs(res)));

	double pp = epsilon(23) * epsilon(19) * epsilon(7) * epsilon(3) * epsilon(26) * epsilon(24)  * epsilon(18) * epsilon(16) * epsilon(28) *
	            epsilon(20) * epsilon(12) * epsilon(8) * epsilon(24) * epsilon(24) * epsilon(12) * epsilon(31) * epsilon(28) *
	            epsilon(20) * epsilon(16) * epsilon(7) * epsilon(12) * epsilon(4);

	printf("Theoretical results is %f(2^%f)\n",pp,log2(pp));

	system("pause");

}
