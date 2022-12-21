#pragma once
#pragma once
//此类实现对于文件内某个页的分配，寻找，实现pagehandle和pfbuffer的固定操作的通信
#include "pf_buffer.h"
#include "noncopyable.h"
#include "pf_pagehandle.h"
#include "pf_pageheader.h"


using namespace std;
//----------------------文件第一页信息结构---------------------------
struct RmFileDatainfo {
	int next;//链表下一页的序号
};
typedef RmFileDatainfo* RmFileDataPtr;

//----------------------文件头部信息结构---------------------------
struct PfFileHeader {
	Page freePage;
	Page foolPage;
	int pageNum;
};
typedef PfFileHeader* PfFileHdr;

#define PF_FILEHEAD_LEN sizeof(PfFileHeader);

//----------------------文件控制句柄-------------------------------

class PfFileHandle 
{
public:
	PfFileHandle() { fileHeadInfo = new PfFileHeader(); };
	~PfFileHandle();

public:
	void getPage(Page num, PfPageHandle& page);                //按照块到找到对应的块
	void nextPage();               //找到输入块号下一个空闲块
	void allocatePage(PfPageHandle&);           //从文件内分配一个页
	//PfPageHnadle与PfBuffer通信
	void pin();
	void unpin();
	void makeDirty();
	void fileWriteBack();
	void setfull(Page);
public:
	PfFileHdr fileHeadInfo;        //文件头部信息
	int fd_;                       //文件描述符
};
typedef PfFileHandle* PfFileHdl;
