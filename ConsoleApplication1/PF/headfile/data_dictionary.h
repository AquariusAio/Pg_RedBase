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
	char relname[REL_NAME_LEM];//��ϵ����
	Page num;      //ռ�õ�ҳ�ı��
	int attrnum;   //���Ե���Ŀ
	int rcdlen;    //��¼����
};
#define REL_LEN sizeof(Relation);

struct RelationData {
	char relname[REL_NAME_LEM];
	unsigned blocks;   //��ϵ��ռ�Ŀ���
	unsigned rcds;     //��ϵ�ļ�¼����
};

struct Attributes {
	char attrname[ATTR_NAME_LEN];
	char rename[REL_NAME_LEM];
	AttrType type;            //��������
	bool flexible;            //�Ƿ�ɱ߳�
	bool indexable;			  //�ж��Ƿ�������
	int offset;               //��¼��ƫ��
	int attrlen;              //��¼����
};


struct IndexData {
	char attrname[ATTR_NAME_LEN];
	char rename[REL_NAME_LEM];
	unsigned height;			//���ĸ߶�
};