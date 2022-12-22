#pragma once

#include "../PF/headfile/pf_filehandle.h"
#include "../PF/headfile/pf_pagehandle.h"
#include "../PF/headfile/data_dictionary.h"
#include "RcdRid.h"
#include "IX_Header.h"
#include"IX_Comp.h"
#include <string.h>

typedef void* keyPtr;

class TreeNode {
private:
	PfPageHdl page_;         //B树存储块操作指针
	IXComp *comp;	        //B树的比较算子

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
	Rid* getRid(int);
	keyPtr getKey(int);
	Page getPage() { return page_->getPage(); }
	Page getLeftPage() { return left; }
	Page getRightPage() { return right; }
	void setRightPage(Page num) { right = num; }
	void setLeftPage(Page num) { left = num; }
public:
	TreeNode(PfPageHdl, AttrType, int);
	~TreeNode();
	int search(void* key, Rid& rid);
	int nodeSearch(void* key);
	bool insertLeaf(void* key, Rid rid);
	bool insertInternal(void* key, Rid rid, int pos);
	bool keyUpdate(void* key, int pos);
	bool ridUpdate(Rid rid, int pos);
	bool isLeaf() { return isLeaf; }
	int Comp(keyPtr lkey, keyPtr rkey) { return comp->operate(lkey,rkey); }
};
typedef TreeNode* TreeNodePtr;

