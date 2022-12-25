#pragma once
#include"ql_head.h"

class QLComp
{
public:
	QLComp() {};
	virtual bool operate(void* lvalue, Operator op, void* rvalue) = 0;
};

class QLIntComp :public QLComp
{
public:
	QLIntComp() {};
	bool operate(void* lvalue, Operator op, void* rvalue);
};

class QLVarcharComp :public QLComp
{
public:
	QLVarcharComp() {};
	bool operate(void* lvalue, Operator op, void* rvalue);
};