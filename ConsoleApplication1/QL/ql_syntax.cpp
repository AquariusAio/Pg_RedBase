#include"ql_syntax.h"

#include<iostream>



//
// buildSyntaxTree - 构建一棵语法树
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
			std::cout << "SQL语句应以;结尾" << endl;
		}
	}
	std::cout << "DML语句解析失败" << endl;
	return node;
}

//
// discard - 丢弃指定个数的token
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
			std::cout << "Select 语句出现语法错误！" << endl;
			node == nullptr;
			return false;
		}
		NODE* relattrlist, * condlist, * groupby, * orderby;
		parseNonmtRelationList(relattrlist);
		parseOptWhereClause(condlist);
		/* order by和group by可以交换位置 */
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
	std::cout << "聚类函数解析失败" << endl;
}

void SyntaxTree::parseNonmtRelationList(NODE*& node)
{
	TokenPtr ta = next(), tb;
	NODE* rel;
	if (!ta || ta->type != RW_STRING) {
		std::cout << "表名错误" << endl;
		return;
	}
	rel = relation_node(ta->content);
	tb = peek(1);
	if (!tb) {
		std::cout << "表名错误" << endl;
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
// parseOptOrderByClause -解析order by语句
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
			std::cout << "排序选项错误" << endl;
			return;
		}
		NODE* relattr;
		if (!parseRelAttr(relattr))		if (!ta && ta->type != RW_BY) {
			std::cout << "排序选项错误" << endl;
			return;
		}
		ta = peek(1);
		if (!ta)  goto {
			std::cout << "排序选项错误" << endl;
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
// parseOptGroupByClause - 解析group by语句
//  opt_group_by_caluse -> nothing | RW_GROUP RW_BY relattr 
//
void SyntaxTree::parseOptGroupByClause(NODE*& node)
{
	TokenPtr ta = peek(1);
	if (ta && ta->type == RW_GROUP) {
		discard(1);
		ta = next();
		if (!ta || ta->type != RW_BY) {
			std::cout << "分组选项错误" << endl;
			return;
		}
		bool succ = parseRelAttr(node);
		if (!succ){
			std::cout << "分组选项错误" << endl;
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