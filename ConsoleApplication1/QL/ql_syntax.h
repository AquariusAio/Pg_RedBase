#pragma once
#include <queue>
#include "ql_lexer.h"
#include "ql_token.h"
#include"ql_head.h"
#include"../PF/headfile/data_dictionary.h"

enum NODEKIND {
	N_CREATETABLE,
	N_CREATEINDEX,
	N_DROPTABLE,
	N_DROPINDEX,
	N_LOAD,
	N_SET,
	N_HELP,
	N_PRINT,
	N_QUERY,
	N_INSERT,
	N_DELETE,
	N_UPDATE,
	N_RELATTR,
	N_ORDERATTR,
	N_AGGRELATTR,
	N_CONDITION,
	N_RELATTR_OR_VALUE,
	N_ATTRTYPE,
	N_VALUE,
	N_RELATION,
	N_STATISTICS,
	N_LIST,
	N_EXIT
};

typedef struct node {
	NODEKIND kind;

	union {
		/* SM component nodes */
		/* create table node */
		struct {
			char* relname;
			struct node* attrlist;
		} CREATETABLE;

		/* create index node */
		struct {
			char* relname;
			char* attrname;
		} CREATEINDEX;

		/* drop index node */
		struct {
			char* relname;
			char* attrname;
		} DROPINDEX;

		/* drop table node */
		struct {
			char* relname;
		} DROPTABLE;

		/* load node */
		struct {
			char* relname;
			char* filename;
		} LOAD;

		/* set node */
		struct {
			char* paramName;
			char* string;
		} SET;

		/* help node */
		struct {
			char* relname;
		} HELP;

		/* print node */
		struct {
			char* relname;
		} PRINT;

		/* QL component nodes */
		/* query node */
		struct {
			struct node* relattrlist;
			struct node* rellist;
			struct node* conditionlist;
			struct node* orderrelattr;
			struct node* grouprelattr;
		} QUERY;

		/* insert node */
		struct {
			char* relname;
			struct node* valuelist;
		} INSERT;

		/* delete node */
		struct {
			char* relname;
			struct node* conditionlist;
		} DELETE;

		/* update node */
		struct {
			char* relname;
			struct node* relattr;
			struct node* relorvalue;
			struct node* conditionlist;
		} UPDATE;

		/* command support nodes */
		/* relation attribute node */
		struct {
			char* relname;
			char* attrname;
		} RELATTR;

		/* order + relation attribute node */
		struct {
			int order;
			struct node* relattr;
		} ORDERATTR;

		/* agg func + relation attribute node */
		struct {
			AggFun func;
			char* relname;
			char* attrname;
		} AGGRELATTR;

		/* condition node */
		struct {
			struct node* lhsRelattr;
			Operator op;
			struct node* rhsRelattr;
			struct node* rhsValue;
		} CONDITION;

		/* relation-attribute or value */
		struct {
			struct node* relattr;
			struct node* value;
		} RELATTR_OR_VALUE;

		/* <attribute, type> pair */
		struct {
			char* attrname;
			char* type;
		} ATTRTYPE;

		/* <value, type> pair */
		struct {
			AttrType type;
			int  ival;
			float rval;
			char* sval;
		} VALUE;

		/* relation node */
		struct {
			char* relname;
		} RELATION;

		/* list node */
		struct {
			struct node* curr;
			struct node* next;
		} LIST;
	} u;
}NODE;

NODE* newnode(NODEKIND kind);
NODE* query_node(NODE* relattrlist, NODE* rellist, NODE* conditionlist, NODE* order_relattr, NODE* group_relattr);
NODE* aggrelattr_node(AggFun a, char* relname, char* attrname);
NODE* relation_node(char* relname);
NODE* attrtype_node(char* attrname, char* type);
NODE* orderattr_node(int order, NODE* relattr);
NODE* relattr_node(char* relname, char* attrname);
NODE* relattr_or_value_node(NODE* relattr, NODE* value);
NODE* list_node(NODE* n);
NODE* prepend(NODE* n, NODE* list);
NODE* value_node(AttrType type, void* value);
NODE* condition_node(NODE* lhsRelattr, Operator op, NODE* rhsRelattrOrValue);
void reset_parser(void);


class SyntaxTree {
public:
	SyntaxTree(LexerPtr& lexer)
		: lexer_(lexer)
	{}
public:
	void resetParser(string& stream)
	{
		reset_parser();
		lexer_->setStream(stream);
		tokens_.clear();
	}
	NODE* buildSyntaxTree();
private:
	bool parseDML(NODE*&);
	bool parseQuery(NODE*&);
	bool parseRelAttr(NODE*&);
	void parseCondition(NODE*&);
	bool parseValue(NODE*&);

	bool parseAggrelattr(NODE*&);
	void parseNonmtSelectClause(NODE*&);
	void parseNonmtRelationList(NODE*&);
	void parseNonmtCondList(NODE*&);
	
	void parseRelAttrOrValue(NODE*&);
	void parseOptWhereClause(NODE*&);
	void parseOptOrderByClause(NODE*&);
	void parseOptGroupByClause(NODE*&);
	void parseNonmtAggrelattrList(NODE*&);

private:
	TokenPtr next();
	TokenPtr peek(int pos);
	void discard(int num);
private:
	LexerPtr lexer_;
	deque<TokenPtr> tokens_; /* ÓÃÓÚ¼ÇÂ¼Token */
};

