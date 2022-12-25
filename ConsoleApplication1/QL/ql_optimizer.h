#pragma once
#include<vector>
#include"ql_syntax.h"
#include"ql_execNode.h"

class Optimizer {
	ExecNode* root;
	vector<SelectNode> selnode;
public:
	Optimizer();
public:
	void executionPlanGenerate(NODE *&);
	void parseSyntaxNode(NODE*&);
	void dpPlanGenerate();
private:
	CombNode* findBestPlan(int, int);
}