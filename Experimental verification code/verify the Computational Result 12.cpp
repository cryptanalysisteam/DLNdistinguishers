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

double epsilon(int i) {
	if (i == 0 || i == 1) {
		return 1.0;
	} else if (i > 1) {
		return 0.75 + pow(2.0, -i);
	} else {
		return 0.0;
	}
}


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

int main() {
	printf("For the Computational Result 12, we randomly generated 2^{6} keys and 2^{32} IVs\n");
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

			int input = 0;

			input = input ^ GET_BIT(X[4],4) ^ GET_BIT(X[4],1) ^ GET_BIT(X[8],24) ^ GET_BIT(X[8],14) ^ GET_BIT(X[8],11) ^ GET_BIT(X[8],6) ^ 
			         GET_BIT(X[9],19) ^ GET_BIT(X[9],6) ^ GET_BIT(X[13],31) ^ GET_BIT(X[13],26) ^ GET_BIT(X[13],21) ^  GET_BIT(X[13],12) ^ GET_BIT(X[13],7) ^  GET_BIT(X[13],0) ^
			         GET_BIT(X[14],31) ^ GET_BIT(X[14],25) ^  GET_BIT(X[14],21) ^  GET_BIT(X[14],20) ^ GET_BIT(X[14],13) ^ GET_BIT(X[2],18) ^ GET_BIT(X[2],14) ^  GET_BIT(X[2],4) ^
			         GET_BIT(X[3],18) ^ GET_BIT(X[3],14) ^ GET_BIT(X[7],31) ^ GET_BIT(X[7],4) ^ GET_BIT(X[7],1) ;

			int Xa1 = X[0];
			int Xa2 = X[5];
			int Xa3 = X[10];
			int Xa4 = X[15];

			int Xd1 = X[12];
			int Xd2 = X[1];
			int Xd3 = X[6];
			int Xd4 = X[11];


			HALF_QR1(X[0], X[4], X[8], X[12]);
			HALF_QR1(X[5], X[9], X[13], X[1]);
			HALF_QR1(X[10], X[14], X[2], X[6]);
			HALF_QR1(X[15], X[3], X[7], X[11]);

			int output = 0;

			output = output ^ GET_BIT(X[4],4) ^ GET_BIT(X[4],1) ^ GET_BIT(X[8],24) ^ GET_BIT(X[8],14) ^ GET_BIT(X[8],11) ^ GET_BIT(X[8],6) ^ 
			         GET_BIT(X[9],19) ^ GET_BIT(X[9],6) ^ GET_BIT(X[13],31) ^ GET_BIT(X[13],26) ^ GET_BIT(X[13],21) ^  GET_BIT(X[13],12) ^ GET_BIT(X[13],7) ^  GET_BIT(X[13],0) ^
			         GET_BIT(X[14],31) ^ GET_BIT(X[14],25) ^  GET_BIT(X[14],21) ^  GET_BIT(X[14],20) ^ GET_BIT(X[14],13) ^ GET_BIT(X[2],18) ^ GET_BIT(X[2],14) ^  GET_BIT(X[2],4) ^
			         GET_BIT(X[3],18) ^ GET_BIT(X[3],14) ^ GET_BIT(X[7],31) ^ GET_BIT(X[7],4) ^ GET_BIT(X[7],1) ;

			output  = output ^ calculate_single(Xa1, Xd1, 29) ^ calculate_single(Xa1, Xd1, 26) ^ calculate_single(Xa1, X[4], 29) ^ calculate_single(Xa1, X[4], 15) ^ calculate_single(Xa1, X[4], 5) ^ calculate_single(Xa1, X[4], 2) ^
			          calculate_single(Xa2, Xd2, 31) ^ calculate_single(Xa2, Xd2, 12) ^ calculate_single(Xa2, X[9], 30) ^ calculate_two(Xa2, X[9], 23) ^ calculate_single(Xa2, X[9], 17) ^ calculate_single(Xa2, X[9], 12) ^ calculate_single(Xa2, X[9], 3) ^
			          calculate_single(Xa3, Xd3, 24) ^ calculate_single(Xa3, Xd3, 18) ^ calculate_two(Xa3, Xd3, 14) ^ calculate_single(Xa3, Xd3, 6) ^ calculate_single(Xa3, X[14], 27) ^ calculate_single(Xa3, X[14], 9) ^ calculate_single(Xa3, X[14], 5) ^
			          calculate_single(Xa4, Xd4, 11) ^ calculate_single(Xa4, Xd4, 7) ^ calculate_single(Xa4, X[3], 27) ^ calculate_single(Xa4, X[3], 24) ^ calculate_single(Xa4, X[3], 22);


			if(input == output) ctr[iKey]++;
		}

		cor[iKey] = 2.0*((double)ctr[iKey]/(double)NIV)-1.0;
	}
	res = GetAverageABS(cor,NK);
//	for(int i = 0; i < NK; i++) {
//		printf("%d res = %f 2^%f\n",i,cor[i],log2(abs(cor[i])));
//	}

	printf("Experimental results is %f(2^%f)\n",res,log2(abs(res)));

	double pp = 
				epsilon(29) * epsilon(26) * epsilon(29) * epsilon(15) * epsilon(5) * epsilon(2) *
	            epsilon(31) * epsilon(12) * epsilon(30) * epsilon(23) * epsilon(17) * epsilon(12) * epsilon(3) *
				epsilon(24) * epsilon(18) * epsilon(14) * epsilon(6) * epsilon(27) * epsilon(9) * epsilon(5) * 
				epsilon(11) * epsilon(7) * epsilon(27)  * epsilon(24)  * epsilon(22)
				;
	printf("Theoretical results is %f(2^%f)\n",pp,log2(pp));


	system("pause");

}
