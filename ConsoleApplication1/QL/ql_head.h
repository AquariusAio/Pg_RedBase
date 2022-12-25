#pragma once
#include"../PF/headfile/data_dictionary.h"
#include<iostream>
//运算符
enum Operator {
	NO_OP,										// 单目等值
	EQ_OP, NE_OP, LT_OP, GT_OP, LE_OP, GE_OP	// 双目比较
};

// 聚集函数
enum AggFun {
	NO_F,
	MIN_F, MAX_F, COUNT_F,
	SUM_F, AVG_F
};

//谓词
class QLComp;
struct RelAttr {
	char* relname;
	char* attrname;
	//friend std::ostream& operator<<(std::ostream& s, const RelAttr& ra);
};

struct Value {
	AttrType type;
	void* data;
	//friend ostream& operator<<(ostream& os, const Value& v);
};

class Condition {
public:
	RelAttr Lattr;
	RelAttr Rattr;
	Value Lvalue;
	Value Rvalue;
	Operator op;
	QLComp* comp;
public:
	bool operate();
};
