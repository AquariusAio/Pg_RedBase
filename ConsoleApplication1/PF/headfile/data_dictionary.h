#pragma once
#include"header.h"

#define REL_NAME_LEM 20
#define ATTR_NAME_LEN 20

enum AttrType
{
	INT,
	FLOAT,
	CHAR,
	VARCAHR

};

struct Relation {
	char relname[REL_NAME_LEM];//关系名称
	Page num;      //占用的页的编号
	int attrnum;   //属性的数目
	int rcdlen;    //记录长度
};
#define REL_LEN sizeof(Relation);

struct RelationData {
	char relname[REL_NAME_LEM];
	unsigned blocks;   //关系所占的块数
	unsigned rcds;     //关系的记录总数
};

struct Attributes {
	char attrname[ATTR_NAME_LEN];
	char rename[REL_NAME_LEM];
	AttrType type;            //属性类型
	bool flexible;            //是否可边长
	bool indexable;			  //判断是否有索引
	int offset;               //记录内偏移
	int attrlen;              //记录长度
};


struct IndexData {
	char attrname[ATTR_NAME_LEN];
	char rename[REL_NAME_LEM];
	unsigned height;			//树的高度
};