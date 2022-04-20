#include<stdio.h>
#include<string.h>
#include<algorithm>
#include<queue>
#include<vector>
#include <iostream>


int N, D, K, A, B, C, M, L1, L2, L3;

int* generate_parameters(int x0, int x1, int A, int B, int C, int M, size_t size) {
	int* ret = new int[size];
	ret[0] = x0;
	ret[1] = x1;
	for (int i = 2; i < size; ++i)
		ret[i] = ((long long)A * ret[i - 1] + (long long)B * ret[i - 2] + C) % M;
	return ret;
}

void forward(int* X, int D, int* P, int L, int* out) {
	for (int j = 0; j < L; ++j)
		out[j] = 0;
	for (int i = 0; i < D; ++i) {
		for (int j = 0; j < L; ++j) {
			out[j] = (out[j] + X[j] * P[j * D + i]) % M;
		}
	}
}

void activation(int* X, int D, int cutoff) {
	for (int i = 0; i < D; ++i)
		if (X[i] < cutoff)
			X[i] = 0;
}

int argmax(int* X, int D) {
	int ret = -1;
	int maxn = -1;
	for (int i = 0; i < D; ++i) {
		if (X[i] > maxn) {
			maxn = X[i];
			ret = i;
		}
	}
	return ret;
}

int main(int argc, char** argv) {
	FILE* fin = fopen(argv[1], "r");
	FILE* fout = fopen(argv[2], "w");
	fscanf(fin, "%d%d%d%d%d%d%d%d%d%d\n", &N, &D, &K, &A, &B, &C, &M, &L1, &L2, &L3);
	int* X = new int[N * D];
	for (int i = 0; i < K; ++i)
		fscanf(fin, "%d", &X[i]);
	

	fclose(fin);
	for (int i = K; i < N * D; ++i)
		X[i] = ((long long)A * X[i - 1] + (long long)B * X[i - 2] + C) % M;

	//for (int i = 0; i < N*D; ++i)
	//	std::cout << X[i] << std::endl;
	//std::cout << std::endl;

	int* P1 = generate_parameters(X[N * D - 2], X[N * D - 1], A, B, C, M, D * L1);
	int* P2 = generate_parameters(P1[D * L1 - 2], P1[D * L1 - 1], A, B, C, M, L1 * L2);
	int* P3 = generate_parameters(P2[L1 * L2 - 2], P2[L1 * L2 - 1], A, B, C, M, L2 * L3);


	//for (int i = 0; i < D * L1; ++i)
	//	std::cout << P1[i] << std::endl;
	//std::cout << std::endl;


	int* X1 = new int[L1];
	int* X2 = new int[L2];
	int* X3 = new int[L3];



	for (int i = 0; i < 1; ++i) {
		forward(X + i * D, D, P1, L1, X1);




		activation(X1, L1, M / 2);

		for (int i = 0; i < L1; ++i)
			std::cout << X1[Fi] << std::endl;


		forward(X1, L1, P2, L2, X2);
		activation(X2, L2, M / 2);
		forward(X2, L2, P3, L3, X3);
		int res = argmax(X3, L3);
		fprintf(fout, "%d\n", res);

		std::cout << std::endl;
	}

	fclose(fout);

	return 0;
}