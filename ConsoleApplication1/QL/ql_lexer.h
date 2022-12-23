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
	int offset_;					// ���ڼ�¼�Ѿ��������˵�λ��
	int line_;					// ��¼�ı����ڵ���,��Ҳ�ǳ���Ҫ
	bool eof_;
public:
	void setStream(const string& stream) {
		offset_ = 0;
		line_ = 1;				// �ӵ�1�п�ʼ
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
