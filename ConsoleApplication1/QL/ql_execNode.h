#pragma once
#include <vector>
#include <map>
#include"ql_head.h"
#include"../PF/headfile/data_dictionary.h"
using namespace std;

enum selectAlgorithm {
	A1,A2,A3,A4,A5,A6
};

struct Selection {
	selectAlgorithm alg;	//存储选择的代价最小的算法
	int cost;               //存储代价
};

class SelectNode {
	vector<Condition> conds_;    //存储谓词
	vector<Attributes> attrs_;   //存储简单关系中的所有
};