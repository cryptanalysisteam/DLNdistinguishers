#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <random>
#include <algorithm>
using namespace std;

#define ROTL(a,b) (((a) << (b)) | ((a) >> (32 - (b))))
#define QR(a, b, c, d) ( \
                         b ^= ROTL(a + d, 7), \
                         c ^= ROTL(b + a, 9), \
                         d ^= ROTL(c + b, 13), \
                         a ^= ROTL(d + c, 18))

#define HALF_QR1(a, b, c, d) ( \
                               b ^= ROTL(a + d, 7), \
                               c ^= ROTL(b + a, 9))

#define HALF_QR2(a, b, c, d) ( \
                               d ^= ROTL(c + b, 13), \
                               a ^= ROTL(d + c, 18))


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
	printf("For the first part of Computational Result 11, we randomly generated 2^{6} keys and 2^{38} IVs\n");
	uint64_t NK;
	uint64_t NIV;
	NK=((uint64_t)1<<6);
	NIV=((uint64_t)1<<38);

	double res = 0.0;
	double cor[NK] = { 0 };


	for(uint64_t iKey = 0; iKey < NK; iKey++) {
		uint64_t ctr[NK] = { 0 };


		uint32_t X[16] = { 0 };
		X[0] = 0x61707865;
		X[5] = 0x3320646e;
		X[10] = 0x79622d32;
		X[15] = 0x6b206574;

		for(int i = 1; i < 5; i++) {
			X[i] = gen();
		}
		for(int i = 11; i < 15; i++) {
			X[i] = gen();
		}

		for(uint64_t iIV = 0; iIV < NIV; iIV++) {
			for(int i = 6; i < 10; i++) {
				X[i] = gen();
			}
			int input = 0;
			int output = 0;

			QR(X[0], X[4], X[8], X[12]);
			QR(X[5], X[9], X[13], X[1]);
			QR(X[10], X[14], X[2], X[6]);
			QR(X[15], X[3], X[7], X[11]);

			QR(X[0], X[1], X[2], X[3]);
			QR(X[5], X[6], X[7], X[4]);
			QR(X[10], X[11], X[8], X[9]);
			QR(X[15], X[12], X[13], X[14]);

			QR(X[0], X[4], X[8], X[12]);
			QR(X[5], X[9], X[13], X[1]);
			QR(X[10], X[14], X[2], X[6]);
			QR(X[15], X[3], X[7], X[11]);

			QR(X[0], X[1], X[2], X[3]);
			QR(X[5], X[6], X[7], X[4]);
			QR(X[10], X[11], X[8], X[9]);
			QR(X[15], X[12], X[13], X[14]);

			QR(X[0], X[4], X[8], X[12]);
			QR(X[5], X[9], X[13], X[1]);
			QR(X[10], X[14], X[2], X[6]);
			QR(X[15], X[3], X[7], X[11]);

			input = (X[4]>>7)&0x1 ;

			QR(X[0], X[1], X[2], X[3]);
			QR(X[5], X[6], X[7], X[4]);
			QR(X[10], X[11], X[8], X[9]);
			QR(X[15], X[12], X[13], X[14]);

			QR(X[0], X[4], X[8], X[12]);
			QR(X[5], X[9], X[13], X[1]);
			QR(X[10], X[14], X[2], X[6]);
			QR(X[15], X[3], X[7], X[11]);

			HALF_QR1(X[0], X[1], X[2], X[3]);
			HALF_QR1(X[5], X[6], X[7], X[4]);
			HALF_QR1(X[10], X[11], X[8], X[9]);
			HALF_QR1(X[15], X[12], X[13], X[14]);

			output =
			    (X[0]>>4)&0x1 ^ (X[0]>>0)&0x1 ^
			    (X[1]>>4)&0x1 ^ (X[1]>>3)&0x1 ^
			    (X[2]>>13)&0x1 ^
			    (X[3]>>17)&0x1 ^
			    (X[4]>>7)&0x1 ^
			    (X[5]>>22)&0x1 ^ (X[5]>>21)&0x1 ^ (X[5]>>19)&0x1 ^
			    (X[6]>>26)&0x1 ^ (X[6]>>25)&0x1 ^ (X[6]>>22)&0x1 ^ (X[6]>>17)&0x1 ^ (X[6]>>16)&0x1 ^
			    (X[7]>>31)&0x1 ^ (X[7]>>26)&0x1 ^
			    (X[8]>>19)&0x1 ^ (X[8]>>14)&0x1 ^
			    (X[9]>>24)&0x1 ^
			    (X[10]>>24)&0x1 ^ (X[10]>>23)&0x1 ^ (X[10]>>10)&0x1 ^ (X[10]>>9)&0x1 ^ (X[10]>>5)&0x1 ^ (X[10]>>4)&0x1 ^
			    (X[11]>>31)&0x1 ^ (X[11]>>10)&0x1 ^ (X[11]>>5)&0x1 ^
			    (X[12]>>14)&0x1 ^ (X[12]>>13)&0x1 ^ (X[12]>>0)&0x1 ^
			    (X[14]>>25)&0x1 ^ (X[14]>>13)&0x1 ^ (X[14]>>7)&0x1 ^
			    (X[15]>>25)&0x1 ^ (X[15]>>24)&0x1 ^ (X[15]>>17)&0x1 ^ (X[15]>>16)&0x1 ^ (X[15]>>7)&0x1;
			if(input == output) ctr[iKey]++;
		}

		cor[iKey] = 2.0*((double)ctr[iKey]/(double)NIV)-1.0;
	}
	res = GetAverageABS(cor,NK);
//	for(int i = 0; i < NK; i++) {
//		printf("%d res = %f 2^%f\n",i,cor[i],log2(abs(cor[i])));
//	}

	printf("The first part of Computational Result 11 is %f(2^%f)\n",res,log2(abs(res)));


	system("pause");

}
