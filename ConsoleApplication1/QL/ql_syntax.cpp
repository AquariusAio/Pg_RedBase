#include"ql_syntax.h"

#include<iostream>

#define MAXNODE 100

static NODE nodepool[MAXNODE];
static int nodeptr = 0;

//
// next - 获取下一个token
// 
TokenPtr SyntaxTree::next()
{
	TokenPtr t;
	if (tokens_.size() != 0) t = tokens_.front();
	else return lexer_->next();
	tokens_.pop_front();
	return t;
}

//
// peek - 向前偷看,因为是在内部使用的函数,所以不会对参数进行检查
//
TokenPtr SyntaxTree::peek(int pos)
{
	while (tokens_.size() < pos) {
		TokenPtr t = lexer_->next();
		if (t == nullptr) return nullptr;
		tokens_.push_back(t);
	}
	return tokens_[pos - 1];
}
//
// buildSyntaxTree - 构建一棵语法树
//
NODE* SyntaxTree::buildSyntaxTree()
{
	NODE* node = nullptr;
	bool succ = parseDML(node);
	if (succ) {
		TokenPtr ta = next();
		if (ta->type == RW_SEMICOLON)
		{
			return node;
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
//
// parseCondition - 解析where后面的条件语句
//	condition -> relattr op relattr_or_value
//
void SyntaxTree::parseCondition(NODE*& node)
{
	bool succ = parseRelAttr(node);
	NODE* rel_or_val;
	if (succ) {
		TokenPtr tk = peek(1);
		Operator op;
		switch (tk->type) {
		case T_LT: discard(1); op = LT_OP; break;
		case T_LE: discard(1); op = LE_OP; break;
		case T_GT: discard(1); op = GT_OP; break;
		case T_EQ: discard(1); op = EQ_OP; break;
		case T_NE: discard(1); op = NE_OP; break;
		default: op = NO_OP; break;
		}
		parseRelAttrOrValue(rel_or_val);
		node = condition_node(node, op, rel_or_val);
	}
	std::cout << "Something wrong with the condition sentences." << endl;
}
//
// parseValue - 解析值
//	value -> T_STRING | T_INT | T_FLOAT
//
bool SyntaxTree::parseValue(NODE*& node)
{
	TokenPtr val = next();
	switch (val->type) {
	case T_STRING:
		node = value_node(VARCAHR, val->content);
		break;
	case T_INT:
	{
		int v;
		sscanf(val->content, "%d", &v);
		node = value_node(INT, &v);
		break;
	}
	case T_FLOAT:
	{
		float v;
		sscanf(val->content, "%f", &v);
		node = value_node(FLOAT, &v);
		break;
	}
	default:
		node = nullptr;
		return false;
	}
	return true;
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
//
// parseRelAttrOrValue - 解析属性或者值
//
void SyntaxTree::parseRelAttrOrValue(NODE*& node)
{
	if (parseRelAttr(node)) {
		node = relattr_or_value_node(node, nullptr);
	}
	else if (parseValue(node)) {
		node = relattr_or_value_node(nullptr, node);
	}
	else {
		std::cout << "Something wrong with the relation attr or value." << endl;
	}
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



void SyntaxTree::parseNonmtCondList(NODE*& node)
{
	NODE* condlist, * cond;
	parseCondition(cond);
	TokenPtr ta = peek(1);
	if (ta) {
		if (ta->type == RW_AND) {
			discard(1);
			parseNonmtCondList(condlist);
			node = prepend(cond, condlist);
		}
		else  node = list_node(cond);
		return;
	}
	std::cout << "Something wrong with the condlist." << endl;
}

//
// parseOptOrderByClause -解析order by语句	
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
		if (!ta){
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

NODE* newnode(NODEKIND kind)
{
	NODE* n;

	/* if we've used up all of the nodes then error */
	if (nodeptr == MAXNODE) {
		fprintf(stderr, "out of memory\n");
		exit(1);
	}

	/* get the next node */
	n = nodepool + nodeptr;
	++nodeptr;

	/* initialize the `kind' field */
	n->kind = kind;
	return n;
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

//
// parseRelAttr - 解析表的属性
//  relattr -> RW_STRING RW_DOT RW_STRING
//			-> RW_STRING
//
bool SyntaxTree::parseRelAttr(NODE*& node)
{
	TokenPtr ta = next(), tb;
	if (ta && ta->type == RW_STRING) {
		tb = peek(1);
		if (!tb) {
			std::cout << "Something wrong with the relation attributions." << endl;
			node = nullptr;
			return false;
		}
		if (tb->type == RW_DOT) {
			discard(1);
			tb = next();
			if (!tb || tb->type != RW_STRING) {
				std::cout << "Something wrong with the relation attributions." << endl;
				node = nullptr;
				return false;
			}
			node = relattr_node(ta->content, tb->content);
		}
		else
			node = relattr_node(nullptr, ta->content);
		return true;
	}

}

//
// parseNonmtAggrelattrList - 解析属性列表
//
void SyntaxTree::parseNonmtAggrelattrList(NODE*& node)
{
	bool succ = parseAggrelattr(node);
	TokenPtr t = peek(1);
	if (t) {
		if (t->type == RW_COMMA) {
			discard(1);
			NODE* attrlist;
			parseNonmtAggrelattrList(attrlist);
			node = prepend(node, attrlist);
		}
		else {
			node = list_node(node);
		}
		return;
	}
	std::cout << "属性表存在问题" << endl;
}

// parseAggrelattr - 解析属性列表
//	aggrelattr -> ammsc RW_LPAREN RW_STRING RW_DOT RW_STRING RW_RPAREN
//				-> ammsc RW_LPAREN RW_STRING RW_RPAREN
//				-> ammsc RW_LPAREN RW_STAR RW_RPAREN
//				-> RW_STRING RW_DOT RW_STRING
//				-> RW_STRING
//  ammsc -> RW_AVG | RW_MAX | RW_MIN | RW_SUM | RW_COUNT;
//
bool SyntaxTree::parseAggrelattr(NODE*& node)
{
	TokenPtr ta = peek(1), tb, tc;
	if (ta) {
		if (ta->type != RW_STRING) {
			discard(1);
			AggFun fun;
			switch (ta->type)
			{
			case RW_AVG: fun = AVG_F; break;
			case RW_MAX: fun = MAX_F; break;
			case RW_MIN: fun = MIN_F; break;
			case RW_SUM: fun = SUM_F; break;
			case RW_COUNT: fun = COUNT_F; break;
			default:std::cout << "Something wrong with the function" << endl;return false;
			}
			ta = next();
			if (!ta || ta->type != RW_LPAREN) {
				std::cout << "Something wrong with the function" << endl; return false;
			}
			ta = next();
			if (!ta){
				std::cout << "Something wrong with the function" << endl; return false;
			}
			if (ta->type == RW_STAR) {
				ta = next();
				if (!ta || ta->type != RW_RPAREN) {
					std::cout << "Something wrong with the function" << endl; return false;
				}
				node = aggrelattr_node(fun, NULL, (char*)"*");
				return true;
			}
			if (ta->type != RW_STRING){
				std::cout << "Something wrong with the function" << endl; return false;
			}
			tb = next();
			if (!tb) {
				std::cout << "Something wrong with the function" << endl; return false;
			}
			if (tb->type == RW_RPAREN) {
				node = aggrelattr_node(fun, NULL, ta->content);
				return true;
			}
			if (tb->type != RW_DOT) {
				std::cout << "Something wrong with the function" << endl; return false;
			}
			tb = next();
			if (!tb || tb->type != RW_STRING) {
				std::cout << "Something wrong with the function" << endl; return false;
			}
			tc = next();
			if (!tc || tc->type != RW_RPAREN) {
				std::cout << "Something wrong with the function" << endl; return false;
			}
			node = aggrelattr_node(fun, ta->content, tb->content);
		}
		else {
			discard(1);
			tb = peek(1);
			if (!tb) {
				std::cout << "Something wrong with the function" << endl;
				return false;
			}
			if (tb->type != RW_DOT) {
				node = aggrelattr_node(NO_F, nullptr, ta->content);
				return true;
			}
			else {
				discard(1); /* 消耗掉RW_DOT */
				tb = next();
				if (!tb || tb->type != RW_STRING) {
					std::cout << "Something wrong with the function" << endl;
					return false;
				}
				node = aggrelattr_node(NO_F, ta->content, tb->content);
				return true;
			}
		}
	}
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

NODE* condition_node(NODE* lhsRelattr, Operator op, NODE* rhsRelattrOrValue)
{
	NODE* n = newnode(N_CONDITION);

	n->u.CONDITION.lhsRelattr = lhsRelattr;
	n->u.CONDITION.op = op;
	n->u.CONDITION.rhsRelattr =
		rhsRelattrOrValue->u.RELATTR_OR_VALUE.relattr;
	n->u.CONDITION.rhsValue =
		rhsRelattrOrValue->u.RELATTR_OR_VALUE.value;
	return n;
}

/*
* relattr_or_valuenode: allocates, initializes, and returns a pointer to
* a new relattr_or_value node having the indicated values.
*/
NODE* relattr_or_value_node(NODE* relattr, NODE* value)
{
	NODE* n = newnode(N_RELATTR_OR_VALUE);

	n->u.RELATTR_OR_VALUE.relattr = relattr;
	n->u.RELATTR_OR_VALUE.value = value;
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

NODE* orderattr_node(int order, NODE* relattr)
{
	NODE* n = newnode(N_ORDERATTR);

	n->u.ORDERATTR.order = order;
	n->u.ORDERATTR.relattr = relattr;
	return n;
}

/*
* list_node: allocates, initializes, and returns a pointer to a new
* list node having the indicated values.
*/
NODE* list_node(NODE* n)
{
	NODE* list = newnode(N_LIST);

	list->u.LIST.curr = n;
	list->u.LIST.next = NULL;
	return list;
}
/*
* value_node: allocates, initializes, and returns a pointer to a new
* value node having the indicated values.
*/
NODE* value_node(AttrType type, void* value)
{
	NODE* n = newnode(N_VALUE);

	n->u.VALUE.type = type;
	switch (type) {
	case INT:
		n->u.VALUE.ival = *(int*)value;
		break;
	case FLOAT:
		n->u.VALUE.rval = *(float*)value;
		break;
	case VARCAHR:
		n->u.VALUE.sval = (char*)value;
		break;
	}
	return n;
}

/*
* prepends node n onto the front of list.
*
* Returns the resulting list.
*/
NODE* prepend(NODE* n, NODE* list)
{
	NODE* newlist = newnode(N_LIST);

	newlist->u.LIST.curr = n;
	newlist->u.LIST.next = list;
	return newlist;
}

/*
* relattr_node: allocates, initializes, and returns a pointer to a new
* relattr node having the indicated values.
*/
NODE* relattr_node(char* relname, char* attrname)
{
	NODE* n = newnode(N_RELATTR);

	n->u.RELATTR.relname = relname;
	n->u.RELATTR.attrname = attrname;
	return n;
}

void reset_parser(void)
{
	reset_scanner();
	nodeptr = 0;
}



