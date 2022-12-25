#include"ql_optimizer.h"
#define MAXATTRS 20;
#define MAXCONDS 40;

void Optimizer::executionPlanGenerate(NODE*& node) {

	switch (node->kind) {
	case N_QUERY:
		int nselattrs = 0;
		AggRelAttr relAttrs[MAXATTRS];
		int nrelations = 0; /* �����Ŀ */
		char* relations[MAXATTRS]; /* ������� */
		int nconditions = 0; /* ��������Ŀ */
		Condition conditions[MAXCONDS];
		int order = 0; /* ����or���� */
		RelAttr orderAttr; /* ����orderAttr������ */
		bool group = false;
		RelAttr groupAttr; /* ����groupAttr������ */

		/* ��ʼ������ѡ������� */
		nselattrs = mk_agg_rel_attrs(n->u.QUERY.relattrlist, MAXATTRS, relAttrs);

		/* ��ʼ����table�������� */
		nrelations = mk_relations(n->u.QUERY.rellist, MAXATTRS, relations);

		/* ��ʼ�������� */
		nconditions = mk_conditions(n->u.QUERY.conditionlist, MAXCONDS, conditions);

		/* ��ʼ������������ */
		mk_order_relattr(n->u.QUERY.orderrelattr, order, orderAttr);

		mk_rel_attr(n->u.QUERY.grouprelattr, groupAttr);

		SelectNode* node = nullptr;
		for (int i = 0; i < nrelations; i++) {
			node = new SelectNode();
			selnode.push_back(node);
		}
		dpPlanGenerate();
	}

}

void Optimizer::dpPlanGenerate() {

	
	CombNode *comb = findBestPlan(0, selnode.size());
	root = new QueryNode(romb);

}

CombNode* Optimizer::findBestPlan(int stat, int end) {

	if (end - stat == 1) {
		return new CombNode(&selnode[stat], &selnode[end]);
	}
	
	CombNode* best=nullptr;
	for (int i = stat; i <= end; i++) {
		CombNode* lnode = findBestPlan(stat, i);
		CombNode* rnode = findBestPlan(i + 1, end);
		CombNode* curr = new CombNode(lnode,rnode);
		if (best) {
			if (best->getCost() > curr->getCost()) {
				delete(best);
				best = curr;
			}
		}
		else best = curr;
	}
	return best;
}