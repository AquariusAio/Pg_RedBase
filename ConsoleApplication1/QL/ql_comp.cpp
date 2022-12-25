#include"ql_head.h"
#include"ql_comp.h"
#include<string.h>
bool Condition::operate() {

	return comp->operate(Lvalue.data, op, Rvalue.data);
}

bool QLIntComp::operate(void* lvalue, Operator op, void* rvalue) {

	if (op == NO_OP) return true;
	int* lv = (int*)lvalue, * rv = (int*)rvalue;
	switch (op)
	{
	case GE_OP: return lv >= rv;
	case EQ_OP: return lv == rv;
	case GT_OP: return lv > rv;
	case LE_OP: return lv <= rv;
	case LT_OP: return lv < rv;
	case NE_OP: return lv != rv;
	default: return true;
	}
}

bool QLVarcharComp::operate(void* lvalue, Operator op, void* rvalue) {
	if (op == NO_OP) return true;
	char* lv = (char*)lvalue, * rv = (char*)rvalue;
	int len = strlen(lv) <= strlen(rv) ? strlen(lv) : strlen(rv);
	switch (op)
	{
	case EQ_OP: return strncmp(lv, rv, len) == 0;
	case NE_OP: return strncmp(lv, rv, len) != 0;
	case LT_OP: return strncmp(lv, rv, len) < 0;
	case GT_OP: return strncmp(lv, rv, len) > 0;
	case LE_OP: return strncmp(lv, rv, len) <= 0;
	case GE_OP: return strncmp(lv, rv, len) >= 0;
	default: return true;
	}
}