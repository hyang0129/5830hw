#include<stdio.h>
#include<string.h>
#include<random>
#include<string>
#include<algorithm>
#include<set>

std::mt19937 generator(22356789);
std::uniform_int_distribution<int> dist(0,std::numeric_limits<int>::max());

void dump_func(int cas,int N,int D,int K,int A,int B,int C,int M,int L1,int L2,int L3){
	printf("dumping case %d\n",cas);
	std::string file_name = "sample" + std::to_string(cas) + ".in";
	FILE* fp = fopen(file_name.c_str(),"w");
	fprintf(fp,"%d %d %d %d %d %d %d %d %d %d\n",N,D,K,A,B,C,M,L1,L2,L3);
	for(int i = 0;i < K;++i){
		fprintf(fp,"%d\n",dist(generator) % M);
	}
	fclose(fp);
}

int main(){
	dump_func(1,10,3,2,dist(generator),dist(generator),dist(generator),31,16,8,4);
	dump_func(2,100,784,2,dist(generator),dist(generator),dist(generator),31,256,128,10);
	dump_func(3,1000,784,2,dist(generator),dist(generator),dist(generator),31,512,128,10);
	dump_func(4,10000,784,2,dist(generator),dist(generator),dist(generator),31,512,128,10);
	dump_func(5,100000,784,2,dist(generator),dist(generator),dist(generator),31,512,128,10);
	dump_func(6,100000,784,2,dist(generator),dist(generator),dist(generator),63,512,128,10);
	dump_func(7,100000,784,2,dist(generator),dist(generator),dist(generator),127,512,128,10);
	dump_func(8,100000,1000,2,dist(generator),dist(generator),dist(generator),127,512,1024,10);
	dump_func(9,100000,1000,2,dist(generator),dist(generator),dist(generator),127,512,1024,100);
	dump_func(10,100000,1000,2,dist(generator),dist(generator),dist(generator),127,512,1024,1000);
	return 0;
}

