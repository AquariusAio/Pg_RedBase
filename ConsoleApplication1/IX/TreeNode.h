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
	PfPageHdl page;         //B���洢�����ָ��
	IXComp comp;	        //B���ıȽ�����
	
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
