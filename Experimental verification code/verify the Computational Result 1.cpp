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


int main() {
	printf("For the Computational Result 1, we randomly generated 2^{6} keys and 2^{32} IVs\n");
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
			int input = 0;
			int output = 0;

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


			input = (X[3]>>0)&0x1 ^
			        (X[4]>>0)&0x1 ;

			QR(X[0], X[5], X[10], X[15]);
			QR(X[1], X[6], X[11], X[12]);
			QR(X[2], X[7], X[ 8], X[13]);
			QR(X[3], X[4], X[ 9], X[14]);

			QR(X[0], X[4], X[ 8], X[12]);
			QR(X[1], X[5], X[ 9], X[13]);
			QR(X[2], X[6], X[10], X[14]);
			QR(X[3], X[7], X[11], X[15]);



			HALF_QR1(X[0], X[5], X[10], X[15]);
			HALF_QR1(X[1], X[6], X[11], X[12]);
			HALF_QR1(X[2], X[7], X[ 8], X[13]);
			HALF_QR1(X[3], X[4], X[ 9], X[14]);

	


			output = (X[0]>>0)&0x1 ^ (X[1]>>7)&0x1 ^ (X[1]>>0)&0x1 ^(X[2]>>8)&0x1 ^ (X[2]>>0)&0x1 ^
			         (X[3]>>0)&0x1 ^ (X[4]>>12)&0x1 ^ (X[4]>>6)&0x1 ^
			         (X[6]>>12)&0x1 ^ (X[7]>>31)&0x1 ^ (X[7]>>19)&0x1 ^
			         (X[9]>>26)&0x1 ^ (X[11]>>12)&0x1 ^ (X[11]>>0)&0x1 ^ (X[12]>>23)&0x1 ^ (X[12]>>12)&0x1 ^
			         (X[12]>>11)&0x1 ^ (X[13]>>24)&0x1 ^ (X[13]>>19)&0x1 ^ (X[13]>>18)&0x1 ^ (X[13]>>16)&0x1 ^
			         (X[13]>>7)&0x1 ^ (X[14]>>0)&0x1 ^
			         (X[15]>>16)&0x1 ;




			if(input == output) ctr[iKey]++;
		}

		cor[iKey] = 2.0*((double)ctr[iKey]/(double)NIV)-1.0;
	}
	res = GetAverageABS(cor,NK);
//	for(int i = 0; i < NK; i++) {
//		printf("%d res = %f 2^%f\n",i,cor[i],log2(abs(cor[i])));
//	}
	printf("Computational Result 1 is %f(2^%f)\n",res,log2(abs(res)));


	system("pause");

}
