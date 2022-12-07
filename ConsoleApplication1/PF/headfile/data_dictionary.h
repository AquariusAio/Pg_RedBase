#pragma once
#include"header.h"

#define REL_NAME_LEM 20
#define ATTR_NAME_LEN 20


struct Relation {
	char relname[REL_NAME_LEM];//关系名称
	Page num;      //占用的页的编号
	int attrnum;   //属性的数目
	int rcdlen;    //记录长度
};
#define REL_LEN sizeof(Relation);

enum AttrType
{
	INT,
	FLOAT,
	CHAR,
	VARCAHR

};

struct Attributes {
	char attrname[ATTR_NAME_LEN];
	char rename[REL_NAME_LEM];
	AttrType type;            //属性类型
	bool flexible;            //是否可边长
	int offset;               //记录内偏移
	int attrlen;              //记录长度
};

struct RelCreation {
	void* data;
	AttrType type;
	int len;
};