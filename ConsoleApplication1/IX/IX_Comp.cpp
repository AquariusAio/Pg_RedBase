#include "IX_Comp.h"
#include<string.h>
#include<iostream>

int IntIXComp::operate(void* ll, void* rr) {

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
	
	char* l = (char*)ll;
	char* r = (char*)rr;
	int end = strcmp((char*)ll,(char*) rr);

	if (end == 0)return 0;
	else if (end > 0)return 1;
	else if (end < 0)return -1;
}