#pragma once
#include<string>
#include"ql_token.h"

class Lexer;
typedef Lexer* LexerPtr;
typedef Token* TokenPtr;

class Lexer
{
public:
	Lexer();
public:
	~Lexer();
private:
	string stream_;
	int offset_;					// 用于记录已经解析到了的位置
	int line_;					// 记录文本所在的行,列也非常重要
	bool eof_;
public:
	void setStream(const string& stream) {
		offset_ = 0;
		line_ = 1;				// 从第1行开始
		stream_ = stream;
		eof_ = stream.size() == 0;
	}
	TokenPtr next();
private:
	int countNL(const string&);
private:
	static LexerPtr instance_;
public:
	static LexerPtr instance();
};
