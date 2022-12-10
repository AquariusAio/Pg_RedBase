#pragma once
#include"../PF/headfile/header.h"
class IXComp {
public:
	virtual int opreate(void* l, void* r)=0;
};

class IntIXComp :public IXComp
{
public:
	int operate(int* l, int* r);
};

class FloatIXComp :public IXComp {
public:
	int operate(float* l, float* r);
};

class VarcharIXComp :public IXComp {
public:
	int operate(char* l, char* r);
};

/*-------------¹¤³¡----------------*/

class IXCompFactory {
public:
	static IXComp generateComp(AttrType type) {
		switch (type)
		{
		case INT:return new IntIXComp();break;
		case FLOAT:return new FloatIXComp();break;
		case CHAR:return new VarcharIXComp(); break;
		case VARCAHR:return new VarcharIXComp(); break;
		default:printf("COMP ERR:type out of range!")break;
		}
	}
};