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

//选择算法结构体
struct Selection {
	selectAlgorithm alg;	  //存储选择的代价最小的算法
	float cost;               //线性搜索代价
	Selection(selectAlgorithm a, float c) :alg(a), cost(c){}
	bool operator<(const Selection& tmp)const {
		return cost > tmp.cost;
	}
};

//多表连接算法结构体
struct Join
{
	nestedAlgorithm alg;
	float cost;             //代价
	bool convert;           //左右子节点是否需要翻转
	Join(nestedAlgorithm a,float c)
	bool operator<(const Join& tmp)const {
		return cost > tmp.cost;
	}
};

//计划抽象结点
class ExecNode {
public:

	RelationData reldata_;                       //代价计算信息
	vector<Condition> conds_;                    //存储谓词
	vector<Attributes> attrs_;                   //存储简单关系中的所有属性
	vector<IndexData> indexattrs_;               //简单关系中可索引属性
	map<const char*, int, CharCmp> indicator_;   //通过属性名称找到属性vector列表
public:
	ExecNode();
	int getCost();
	void calculateCost();
};

//关系结点
class SelectNode:public ExecNode {

	priority_queue<Selection> alg_;              //存储算法信息

public:
	SelectNode();
	int getCost();
	void calculateCost();
};

//连接结点
class CombNode :public ExecNode{
	ExecNode* lnode, *rnode;                //左右结点
	priority_queue<Join> alg_;              //存储算法信息
public:
	CombNode(ExecNode* l, ExecNode* r) :lnode(l), rnode(r) {}
	int getCost();
	void calculateCost();
};

//根节点
class QueryNode :public ExecNode {
	CombNode* root;
	Order order;                   //排序函数
	vector<AggRelAttr> agg;        //聚类函数
	vector<vector<int>> froup;     //分组信息 
public:
	QueryNode(CombNode* n) :root(n) {}

};