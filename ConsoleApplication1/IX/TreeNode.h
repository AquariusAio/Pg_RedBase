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
	PfPageHdl page;       //B���洢�����ָ��
	
	Page left, right;      //B���������ҽ��
	bool isRoot;           //�ж��Ƿ�ΪB�����ڵ�
	bool isLeaf;           //�ж��Ƿ�ΪB����Ҷ���
	AttrType keyType;      //���ֵ������
	int keyused;            //��������

	char* keys_;             //B������keyֵ
	Rid* rids_;            //B��һ������ڵ����м�ֵ
public:
	TrreNode(PfPageHdl ,AttrType, int);
	~TreeNode();
	Rid getRid(int);
	int nodeSearch(void* key);
	bool nodeInsert(void* key, int pos);

}

class Comp {

};