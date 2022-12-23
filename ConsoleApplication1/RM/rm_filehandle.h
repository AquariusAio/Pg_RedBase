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
	int insertRcord( char*);                        //插入一个记录
	int closeFile();                          //关闭文件
	int catFile(Page num);
private:
	Page freePage();                                     //一个可用的page的序号
	void freePagehandle(PfPageHandle&);                  //获得一个可用PageHandle
	unsigned nextSlot(PfPageHandle&);                    //获取下一个槽号
	void insertEndProcess(PfPageHandle& page, unsigned rcdlen);   //插入后处理
public:
	PfFileHdl pffile_;      
	RmFileDataPtr fileDataPtr;
	RmPageHdl pagehandle;

};

typedef RmFileHandle* RmFileHdl;