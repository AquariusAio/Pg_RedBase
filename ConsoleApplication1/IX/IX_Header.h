#pragma once
#include"../PF/headfile/data_dictionary.h"


/*
	B+树的文件的头部信息
*/
struct BtreeFileHeader {

	int used;            //已经使用的页的数量
	int height;          //B树的高度
	AttrType attrtype;   //属性的类型

	Page root, leaf;     //B+树的根和首个叶子结点
	int capacity;        //每一页内记录的容量
	
};


/*
	B+树文件内的页头部信息
*/
struct BtreeNodeheader
{
	unsigned type;      //结点页的类型
	Page left, right;   //左右子页的指针
	int keyused;        //记录的数量
	int keylen;          //每一个键的长度
};


/*
	type的取值类型
*/
#define IS_ROOT 1
#define IS_LEAF 2