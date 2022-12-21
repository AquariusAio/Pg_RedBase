#pragma once
#include"../PF/headfile/pf_operator.h"
#include"../PF/headfile/data_dictionary.h"
#include"../RM/rm_filehandle.h"
#include"IX_Handle.h"
#include"../RM/rm_filescan.h"

namespace IX {
	void CreateIndex(PfFileHdl file, RmFileScan& scan, Attributes attr);
	void OpenIndex();
};
void CreateIndex(const char* relname, const char* attrname);

