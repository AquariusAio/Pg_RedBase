#pragma once

#include "../PF/headfile/pf_filehandle.h"
#include "../PF/headfile/pf_pagehandle.h"
#include "../PF/headfile/data_dictionary.h"
#include "RcdRid.h"
#include "IXheader.h"
#include <string.h>
class Comp;
class TreeNode{
private:
	PfPageHdl page;       //B树存储块操作指针
	
	Page left, right;      //B树结点的左右结点
	bool isRoot;           //判断是否为B树根节点
	bool isLeaf;           //判断是否为B树的叶结点
	AttrType keyType;      //结点值的类型
	int keyused;            //键的数量

	char* keys_;             //B树结点的key值
	Rid* rids_;            //B树一个结点内的所有键值
public:
	TrreNode(PfPageHdl ,AttrType, int);
	~TreeNode();
	Rid getRid(int);
	int nodeSearch(void* key);
	bool nodeInsert(void* key, int pos);

}

class Comp {

};