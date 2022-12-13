#pragma once
#include"../PF/headfile/data_dictionary.h"


/*
	B+�����ļ���ͷ����Ϣ
*/
struct BtreeFileHeader {

	int used;            //�Ѿ�ʹ�õ�ҳ������
	int height;          //B���ĸ߶�
	AttrType attrtype;   //���Ե�����

	Page root, leaf;     //B+���ĸ����׸�Ҷ�ӽ��
	int capacity;        //ÿһҳ�ڼ�¼������
	
};


/*
	B+���ļ��ڵ�ҳͷ����Ϣ
*/
struct BtreeNodeheader
{
	unsigned type;      //���ҳ������
	Page left, right;   //������ҳ��ָ��
	int keyused;        //��¼������
	int keylen;          //ÿһ�����ĳ���
};


/*
	type��ȡֵ����
*/
#define IS_ROOT 1
#define IS_LEAF 2