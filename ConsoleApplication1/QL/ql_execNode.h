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
	selectAlgorithm alg;	//�洢ѡ��Ĵ�����С���㷨
	int cost;               //�洢����
};

class SelectNode {
	vector<Condition> conds_;    //�洢ν��
	vector<Attributes> attrs_;   //�洢�򵥹�ϵ�е�����
};