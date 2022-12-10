#pragma once
#include "../PF/headfile/pf_filehandle.h"
#include "../PF/headfile/pf_pagehandle.h"
#include"rm_pagehandle.h"
struct RmFileDatainfo {
	int next;//������һҳ�����
};
typedef RmFileDatainfo* RmFileDataPtr;


class RmFileHandle {

public:
	RmFileHandle(PfFileHdl file);
	~RmFileHandle();

public:
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