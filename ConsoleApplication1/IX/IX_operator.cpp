#include"IX_operator.h"
#include<stdio.h>
void CreateIndex(const char* relname, const char* attrname) {
	
	Attributes attr;
	memcpy(attr.attrname, attrname, 7);
	attr.attrlen = 4;
	attr.flexible = false;
	attr.offset = 0;
	memcpy(attr.rename, "account", 7);
	attr.type = INT;

	char indexname[20];
	sprintf(indexname, "%s%d", relname, attr.offset);
	CreateFile(indexname);
	RmFileHdl indexfile, rcdfile;
	OpenFile(indexname, indexfile);
	OpenFile(relname, rcdfile);

	RmFileScan scan(rcdfile,attr.offset);
	IX::CreateIndex(indexfile->pffile_, scan, attr);
}

namespace IX {
	void CreateIndex(PfFileHdl file ,RmFileScan &scan, Attributes attr) {
		Rid rid_(1, -1);
		RmRid rid(rid_, attr.attrlen);
		IXHandle ix(file,attr);
		ix.createIndex(attr);

		while (scan.getNextRcd(rid)) {
			ix.insertIndex(rid.buffer_, rid.rid_);
		}
	}
}