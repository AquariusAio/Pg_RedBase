#pragma once
#include "../PF/headfile/headfile/pf_filehandle.h"
#include "../PF/headfile/headfile/pf_pagehandle.h"
#include"rm_rcdrid.h"
#include "../IX/RcdRid.h"
#include"rm_pagehandle.h"



class RmFileHandle {

public:
	RmFileHandle(PfFileHdl file);
	~RmFileHandle();

public:
	int getRcd(Rid &rid,RmRid&rmRid);
	int insertRcord( char*);                        //����һ����¼
	int closeFile();                          //�ر��ļ�
	int catFile(Page num);
private:
	Page freePage();                                     //һ�����õ�page�����
	void freePagehandle(PfPageHandle&);                  //���һ������PageHandle
	unsigned nextSlot(PfPageHandle&);                    //��ȡ��һ���ۺ�
	void insertEndProcess(PfPageHandle& page, unsigned rcdlen);   //�������
public:
	PfFileHdl pffile_;      
	RmFileDataPtr fileDataPtr;
	RmPageHdl pagehandle;

};

typedef RmFileHandle* RmFileHdl;