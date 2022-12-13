#include"IX_operator.h"

void CreateIndex(const char* relname, const char* attrname) {
	
	Attributes attr;
	attr.attrname = account;
	attr.attrlen = 4;
	attr.flexible = false;
	attr.offset = 10;
	memcpy(attr.rename, "account", 7);
	attr.type = INT;

	CreateFile(relname);
	RmFileHandle indexfile;
	OpenFile(relname, indexfile);
	RmFileScan scan(indexfile,attr,offset);

	IX::CreateIndex(indexfile.pffile_, scan, attr);
}

namespace IX {
	void CreateIndex(PfFileHandle& file ,RmFileScan &scan, Attributes attr) {
		
		RmRid rid(new Rid(1, -1), attr.offset);
		IXHandle ix;
		ix.createIndex(file);

		while (scan.getNextRcd(rid)) {
			ix.insertIndex(rid.buffer_, rid.rid_);
		}
	}
}