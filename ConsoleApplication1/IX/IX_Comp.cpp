#include "IX_Comp.h"
#include<string.h>

int IntIXComp::operate(void* ll, void* rr) {
	if (ll == nullptr) {
		printf("nullptr ");
		return 0;
	}
	int* l = (int*)ll;
	int* r = (int*)rr;
	if (*l == *r) return 0;
	else if (*l > *r) return 1;
	else if (*l < *r)return -1;
}


int FloatIXComp::operate(void* ll, void* rr) {
	float* l = (float*)ll;
	float* r = (float*)rr;
	if (*l == *r) return 0;
	else if (*l > *r) return 1;
	else if (*l < *r)return -1;
}


int VarcharIXComp::operate(void* ll, void* rr) {
	int end = strcmp((char*)ll,(char*) rr);
	if (end == 0)return 0;
	else if (end > 0)return 1;
	else if (end < 0)return -1;
}