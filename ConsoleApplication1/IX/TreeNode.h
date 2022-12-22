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
	PfPageHdl page_;         //B���洢�����ָ��
	IXComp *comp;	        //B���ıȽ�����

	Page left, right;       //B���������ҽ��
	bool isRoot;            //�ж��Ƿ�ΪB�����ڵ�
	bool isLeaf;            //�ж��Ƿ�ΪB����Ҷ���
	AttrType keyType;       //���ֵ������
public:
	int capacity_;           //��������
	int keyused_;            //��������
	int keylen_;             //���ĳ���
	char* keys_;             //B������keyֵ
	Rid* rids_;              //B��һ������ڵ����м�ֵ
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

