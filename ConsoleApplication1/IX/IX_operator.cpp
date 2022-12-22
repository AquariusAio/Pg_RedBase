#include"IX_operator.h"
#include<stdio.h>
void CreateIndex(const char* relname, const char* attrname) {
	
	//��������
	Attributes attr;
	memcpy(attr.attrname, attrname, 7);
	attr.attrlen = 4;
	attr.flexible = false;
	attr.offset = 0;
	memcpy(attr.rename, "account", 7);
	attr.type = INT;

	RmFileHdl indexfile, rcdfile;
	char indexname[20];
	sprintf(indexname, "%s%d", relname, attr.offset);

	//���������ļ����������ļ�
	CreateFile(indexname);
	OpenFile(indexname, indexfile);
	OpenFile(relname, rcdfile);

	//�򿪼�¼�ļ�
	RmFileScan scan(rcdfile,attr.offset);
	IX::CreateIndex(indexfile->pffile_, scan, attr);
}

namespace IX {
	void CreateIndex(PfFileHdl file ,RmFileScan &scan, Attributes attr) {
		Rid rid_(1, -1);
		RmRid rid(rid_, attr.attrlen);
		IXHandle ix(file,attr,false);

		ix.createIndex(attr);//���������ļ�
		//�ݹ�ȡ����¼����B��
		while (scan.getNextRcd(rid)) {
			ix.insertIndex(rid.buffer_, rid.rid_);
		}
		printf("IX::CreateIndex() END");
	}

	void OpenIndex(const char* relname, IXFileScan &scan) {
		//��������
		Attributes attr;
		memcpy(attr.attrname, attrname, 7);
		attr.attrlen = 4;
		attr.flexible = false;
		attr.offset = 0;
		memcpy(attr.rename, "account", 7);
		attr.type = INT;


		char indexname[20];
		sprintf(indexname, "%s%d", relname, attr.offset);

		RmFileHdl indexfile;
		OpenFile(indexname, indexfile);
		scan.openScan(indexfile->pffile_, attr);
	}
}