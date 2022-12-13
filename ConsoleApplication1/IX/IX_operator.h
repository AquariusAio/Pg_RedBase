#pragma once
#include"../PF/headfile/pf_operator.h"
#include"../PF/headfile/data_dictionary.h"

void CreateIndex(const char* relname, const char* attrname)
namespace IX {
	void CreateIndex(const char* relname, Attributes attr);
	void OpenIndex();
}
