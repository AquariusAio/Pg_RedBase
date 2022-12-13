#pragma once

#include "../PF/headfile/pf_filehandle.h"
#include "../PF/headfile/pf_pagehandle.h"
#include "../PF/headfile/data_dictionary.h"
#include "RcdRid.h"
#include "IX_Header.h"
#include"IX_Comp.h"
#include <string.h>

class TreeNode{
private:
	PfPageHdl page;         //B树存储块操作指针
	IXComp comp;	        //B树的比较算子
	
	Page left, right;       //B树结点的左右结点
	bool isRoot;            //判断是否为B树根节点
	bool isLeaf;            //判断是否为B树的叶结点
	AttrType keyType;       //结点值的类型
public:
	int capacity_;           //键的容量
	int keyused_;            //键的数量
	int keylen_;             //键的长度
	char* keys_;             //B树结点的key值
	Rid* rids_;              //B树一个结点内的所有键值
public:
	TrreNode(PfPageHdl ,AttrType, int);
	Rid getRid(int);
	keyPtr getKey(int);
	int search(void* key, Rid& rid);
	int nodeSearch(void* key);
	bool insertLeaf(void* key, Rid rid);
	bool insertInternal(void* key, Rid rid, int pos);
	bool keyUpdate(void* key, int pos);
	bool ridUpdate(Rid rid, int pos);
private:
	int comp(void *,void*)
}
typedef  TreeNodePtr TreeNode*;
typedef keyPtr void*;
