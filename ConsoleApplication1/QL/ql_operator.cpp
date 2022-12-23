#include "ql_operator.h"
#include<iostream>

NODE* parse_tree;
namespace QL {
	void Parser() {
		string buffer;
		LexerPtr lexer = Lexer::instance();
		SyntaxTree tree(lexer);

		//��������������
		std::cout.flush();
		getline(cin, buffer);

		//����
		tree.resetParser(buffer);
		parse_tree = tree.buildSyntaxTree();
	}
}