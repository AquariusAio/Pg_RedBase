#include "ql_operator.h"
#include<iostream>

NODE* parse_tree;
namespace QL {
	void Parse() {
		string buffer;
		LexerPtr lexer = Lexer::instance();
		SyntaxTree tree(lexer);

		//��������������
		std::cout.flush();
		getline(cin, buffer);

		//����
		tree.resetParser(buffer);
		parse_tree = tree.buildSyntaxTree();
		std::cout << "Syntax Tree" << endl;
	}
	
}