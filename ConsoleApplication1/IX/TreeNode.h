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
	int capacity_;          //��������
	int keyused_;            //��������

	char* keys_;             //B������keyֵ
	Rid* rids_;              //B��һ������ڵ����м�ֵ
public:
	TrreNode(PfPageHdl ,AttrType, int);
	Rid getRid(int);
	int nodeSearch(void* key);
	bool nodeInsert(void* key, int pos);
}
typedef  TreeNodePtr TreeNode*;

