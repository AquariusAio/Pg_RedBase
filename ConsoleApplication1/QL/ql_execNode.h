#pragma once
#include <vector>
#include<queue>
#include <map>
#include<cmath>
#include"ql_head.h"
#include"../PF/headfile/data_dictionary.h"
using namespace std;
#define Ts 4
#define Tt 0.1
#define COST_MAX 100000

enum selectAlgorithm {
	A1,A2,A3,A40,A41,A5,A6
};

enum nestedAlgorithm {
	NestedLoopJoin,BlockJoin,IndexJoin,MergeJoin,HashJoin
};

//ѡ���㷨�ṹ��
struct Selection {
	selectAlgorithm alg;	  //�洢ѡ��Ĵ�����С���㷨
	float cost;               //������������
	Selection(selectAlgorithm a, float c) :alg(a), cost(c){}
	bool operator<(const Selection& tmp)const {
		return cost > tmp.cost;
	}
};

//��������㷨�ṹ��
struct Join
{
	nestedAlgorithm alg;
	float cost;             //����
	bool convert;           //�����ӽڵ��Ƿ���Ҫ��ת
	Join(nestedAlgorithm a,float c)
	bool operator<(const Join& tmp)const {
		return cost > tmp.cost;
	}
};

//�ƻ�������
class ExecNode {
public:

	RelationData reldata_;                       //���ۼ�����Ϣ
	vector<Condition> conds_;                    //�洢ν��
	vector<Attributes> attrs_;                   //�洢�򵥹�ϵ�е���������
	vector<IndexData> indexattrs_;               //�򵥹�ϵ�п���������
	map<const char*, int, CharCmp> indicator_;   //ͨ�����������ҵ�����vector�б�
public:
	ExecNode();
	int getCost();
	void calculateCost();
};

//��ϵ���
class SelectNode:public ExecNode {

	priority_queue<Selection> alg_;              //�洢�㷨��Ϣ

public:
	SelectNode();
	int getCost();
	void calculateCost();
};

//���ӽ��
class CombNode :public ExecNode{
	ExecNode* lnode, *rnode;                //���ҽ��
	priority_queue<Join> alg_;              //�洢�㷨��Ϣ
public:
	CombNode(ExecNode* l, ExecNode* r) :lnode(l), rnode(r) {}
	int getCost();
	void calculateCost();
};

//���ڵ�
class QueryNode :public ExecNode {
	CombNode* root;
	Order order;                   //������
	vector<AggRelAttr> agg;        //���ຯ��
	vector<vector<int>> froup;     //������Ϣ 
public:
	QueryNode(CombNode* n) :root(n) {}

};