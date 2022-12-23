#include "ql_operator.h"
#include<iostream>

NODE* parse_tree;
namespace QL {
	void Parser() {
		string buffer;
		LexerPtr lexer = Lexer::instance();
		SyntaxTree tree(lexer);

		//接受输入解析语句
		std::cout.flush();
		getline(cin, buffer);

		//建树
		tree.resetParser(buffer);
		parse_tree = tree.buildSyntaxTree();
	}
}