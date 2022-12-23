#include"ql_syntax.h"

#include<iostream>



//
// buildSyntaxTree - ����һ���﷨��
//
NODE* SyntaxTree::buildSyntaxTree()
{
	NODE* node = nullptr;
	bool succ = parseCommand(node);
	if (!succ) succ = parseUtility(node);
	if (succ) {
		TokenPtr ta = next();
		if (ta->type == RW_SEMICOLON)
		{
			return node;
			std::cout << "SQL���Ӧ��;��β" << endl;
		}
	}
	std::cout << "DML������ʧ��" << endl;
	return node;
}

//
// discard - ����ָ��������token
// 
void SyntaxTree::discard(int num)
{
	while (tokens_.size() != 0 && num != 0) {
		tokens_.pop_front();
		num--;
	}
	while (num > 0) {
		lexer_->next();
		num--;
	}
}


bool SyntaxTree::parseDML(NODE*& node)
{
	bool succ = parseQuery(node);
	if (!succ) succ = parseInsert(node);
	return succ;
}

bool SyntaxTree::parseQuery(NODE*& node)
{
	TokenPtr ta = peek(1);
	if (ta && ta->type == RW_SELECT) {
		discard(1);
		NODE* clause;
		parseNonmtSelectClause(clause);
		ta = next();
		if (!ta || ta->type != RW_FROM) {
			std::cout << "Select �������﷨����" << endl;
			node == nullptr;
			return false;
		}
		NODE* relattrlist, * condlist, * groupby, * orderby;
		parseNonmtRelationList(relattrlist);
		parseOptWhereClause(condlist);
		/* order by��group by���Խ���λ�� */
		parseOptOrderByClause(orderby);
		parseOptGroupByClause(groupby);
		if (orderby == nullptr && groupby != nullptr) {
			parseOptOrderByClause(orderby);
		}
		node = query_node(clause, relattrlist, condlist, orderby, groupby);
		return true;
	}
	node == nullptr;
	return false;

}

void SyntaxTree::parseNonmtSelectClause(NODE*& node)
{
	TokenPtr ta = peek(1);
	if (ta) {
		if (ta->type == RW_STAR) {
			discard(1);
			node = list_node(aggrelattr_node(NO_F, NULL, (char*)"*"));
		}
		else
			parseNonmtAggrelattrList(node);
		return;
	}
	std::cout << "���ຯ������ʧ��" << endl;
}

void SyntaxTree::parseNonmtRelationList(NODE*& node)
{
	TokenPtr ta = next(), tb;
	NODE* rel;
	if (!ta || ta->type != RW_STRING) {
		std::cout << "��������" << endl;
		return;
	}
	rel = relation_node(ta->content);
	tb = peek(1);
	if (!tb) {
		std::cout << "��������" << endl;
		return;
	}
	if (tb->type != RW_COMMA) node = list_node(rel);
	else {
		discard(1);
		NODE* rellist;
		parseNonmtRelationList(rellist);
		node = prepend(rel, rellist);
	}
	return;

}

void SyntaxTree::parseOptWhereClause(NODE*& node)
{
	TokenPtr t = peek(1);
	if (t && t->type == RW_WHERE) {
		discard(1);
		parseNonmtCondList(node);
		return;
	}
	node == nullptr;
}

//
// parseOptOrderByClause -����order by���
//  opt_order_by_clause -> RW_ORDER RW_BY ordering_spec
//						-> nothing
//  ordering_spec -> relattr opt_asc_desc
//	opt_asc_desc -> RW_DESC | RW_ASC
//
void SyntaxTree::parseOptOrderByClause(NODE*& node)
{
	TokenPtr ta = peek(1), tb;
	if (ta && ta->type == RW_ORDER) {
		discard(1);
		ta = next();
		if (!ta && ta->type != RW_BY) {
			std::cout << "����ѡ�����" << endl;
			return;
		}
		NODE* relattr;
		if (!parseRelAttr(relattr))		if (!ta && ta->type != RW_BY) {
			std::cout << "����ѡ�����" << endl;
			return;
		}
		ta = peek(1);
		if (!ta)  goto {
			std::cout << "����ѡ�����" << endl;
			return;
		}
		int order;
		switch (ta->type)
		{
		case RW_DESC: discard(1); order = -1; break;
		case RW_ASC: discard(1);  order = 1; break;
		default: order = 1; break;
		}
		node = orderattr_node(order, relattr);
		return;
	}
	node = orderattr_node(0, 0);
	return;

}

//
// parseOptGroupByClause - ����group by���
//  opt_group_by_caluse -> nothing | RW_GROUP RW_BY relattr 
//
void SyntaxTree::parseOptGroupByClause(NODE*& node)
{
	TokenPtr ta = peek(1);
	if (ta && ta->type == RW_GROUP) {
		discard(1);
		ta = next();
		if (!ta || ta->type != RW_BY) {
			std::cout << "����ѡ�����" << endl;
			return;
		}
		bool succ = parseRelAttr(node);
		if (!succ){
			std::cout << "����ѡ�����" << endl;
			return;
		}
		return;
	}
	node = relattr_node(nullptr, nullptr);
	return;
}

NODE* query_node(NODE* relattrlist, NODE* rellist, NODE* conditionlist,
	NODE* order_relattr, NODE* group_relattr)
{
	NODE* n = newnode(N_QUERY);

	n->u.QUERY.relattrlist = relattrlist;
	n->u.QUERY.rellist = rellist;
	n->u.QUERY.conditionlist = conditionlist;
	n->u.QUERY.orderrelattr = order_relattr;
	n->u.QUERY.grouprelattr = group_relattr;
	return n;
}

NODE* aggrelattr_node(AggFun a, char* relname, char* attrname) {

	NODE* n = newnode(N_AGGRELATTR);

	n->u.AGGRELATTR.func = a;
	n->u.AGGRELATTR.relname = relname;
	n->u.AGGRELATTR.attrname = attrname;
	return n;
}


NODE* relation_node(char* relname)
{
	NODE* n = newnode(N_RELATION);

	n->u.RELATION.relname = relname;
	return n;
}

NODE* attrtype_node(char* attrname, char* type)
{
	NODE* n = newnode(N_ATTRTYPE);

	n->u.ATTRTYPE.attrname = attrname;
	n->u.ATTRTYPE.type = type;
	return n;
}

void reset_parser(void)
{
	reset_scanner();
	nodeptr = 0;
}