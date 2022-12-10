#pragma once
#pragma once
//����ʵ�ֶ����ļ���ĳ��ҳ�ķ��䣬Ѱ�ң�ʵ��pagehandle��pfbuffer�Ĺ̶�������ͨ��
#include "pf_buffer.h"
#include "noncopyable.h"
#include "pf_pagehandle.h"
#include "pf_pageheader.h"

using namespace std;

//----------------------�ļ�ͷ����Ϣ�ṹ---------------------------
struct PfFileHeader {
	Page freePage;
	Page foolPage;
	int pageNum;
};
typedef PfFileHeader* PfFileHdr;

#define PF_FILEHEAD_LEN sizeof(PfFileHeader);

//----------------------�ļ����ƾ��-------------------------------

class PfFileHandle :public noncopyable
{
public:
	PfFileHandle() { fileHeadInfo = new PfFileHeader(); };
	~PfFileHandle();

public:
	void getPage(Page num, PfPageHandle& page);                //���տ鵽�ҵ���Ӧ�Ŀ�
	void nextPage();               //�ҵ���������һ�����п�
	void allocatePage(PfPageHandle&);           //���ļ��ڷ���һ��ҳ
	//PfPageHnadle��PfBufferͨ��
	void pin();
	void unpin();
	void makeDirty();
	void fileWriteBack();
public:
	PfFileHdr fileHeadInfo;        //�ļ�ͷ����Ϣ
	int fd_;                       //�ļ�������
};
typedef PfFileHandle* PfFileHdl;
