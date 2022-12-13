#pragma once
#include"../PF/headfile/pf_operator.h"
#include"../PF/headfile/data_dictionary.h"
#include"../RM/rm_filehandle.h"
#include"IX_Handle.h"
#include"../RM/rm_filescan.h"

void CreateIndex(const char* relname, const char* attrname)
namespace IX {
	void CreateIndex(PfFileHandle& file, RmFileScan& scan, Attributes attr);
	void OpenIndex();
}
