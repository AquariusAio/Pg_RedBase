#include"IX_operator.h"
#include<stdio.h>
void CreateIndex(const char* relname, const char* attrname) {
	
	//测试属性
	Attributes attr;
	memcpy(attr.attrname, attrname, 7);
	attr.attrlen = 6;
	attr.flexible = false;
	attr.offset = 4;
	memcpy(attr.rename, "account", 7);
	attr.type = VARCAHR;

	RmFileHdl indexfile, rcdfile;
	char indexname[20];
	sprintf(indexname, "%s%d", relname, attr.offset);

	//创造索引文件并打开索引文件
	CreateFile(indexname);
	OpenFile(indexname, indexfile);
	OpenFile(relname, rcdfile);

	//打开记录文件
	RmFileScan scan(rcdfile,attr.offset);
	IX::CreateIndex(indexfile->pffile_, scan, attr);
}

namespace IX {
	void CreateIndex(PfFileHdl file ,RmFileScan &scan, Attributes attr) {
		Rid rid_(1, -1);
		RmRid rid(rid_, attr.attrlen);
		IXHandle ix(file,attr,false);

		ix.createIndex(attr);//创造索引文件
		//递归取出记录插入B树
		while (scan.getNextRcd(rid,(PageBuffer)ix.root_->rids_)) {
			ix.insertIndex(rid.buffer_, rid.rid_);
			
		}
		printf("IX::CreateIndex() END");
	}

	void OpenIndex(const char* relname, IXFileScan &scan) {
		//测试属性
		Attributes attr;
		memcpy(attr.attrname, "name", 4);
		attr.attrlen = 6;
		attr.flexible = false;
		attr.offset = 4;
		memcpy(attr.rename, "account", 7);
		attr.type = VARCAHR;


		char indexname[20];
		sprintf(indexname, "%s%d", relname, attr.offset);

		RmFileHdl indexfile;
		OpenFile(indexname, indexfile);
		scan.openScan(indexfile->pffile_, attr);
	}
}