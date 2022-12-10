#include "IX_Comp.h"
#include<string.h>

int IntIXComp::operate(int* l, int* r) {
	if (&l == &r) return 0;
	else if (&l > &r) return 1;
	else if (&l < &r)return -1;
}


int FloatIXComp::operate(float* l, float* r) {
	if (&l == &r) return 0;
	else if (&l > &r) return 1;
	else if (&l < &r)return -1;
}


int VarcharIXComp::operate(char* l, char* r) {
	int end = strcmp(l, r);
	if (end == 0)return 0;
	else if (end > 0)return 1;
	else if (end < 0)return -1;
}