#pragma once
//����ҳ�Ŀ�ţ�����ҳ���ڴ�洢λ��ָ�룬ʵ��ҳ�Ĺ̶���ȡ���Ȳ���
#include"header.h"
#include"pf_pageheader.h"


class PfFileHandle;
class PfPageHandle
{
public:
	PfPageHandle() :num(-1), addr_(nullptr), dirty_(false) {}
	~PfPageHandle();
public:
	PfPageHdr head_;
private:
	bool dirty_;
	Page num;
	PageBuffer addr_;
	PfFileHandle* pffile_;
public:
	void pin();
	void unpin() ;
	void setDirty() { dirty_ = true; };
	bool full() { return head_.pd_lower == head_.pd_upper ? 1 : 0; }
public:
	void setPageBuffer(PageBuffer ptr) { addr_ = ptr; }
	void setPage(Page num) { this->num = num; }
	void setFile(PfFileHandle* file) { pffile_ = file; };
	Page getPage() { return num; }
	PageBuffer getPageBuffer() { return this->addr_; }
	PageBuffer getInsertBuffer() { return getLinpBuffer()+ PF_LINP_LEN; }
	PageBuffer getLinpBuffer() { return this->addr_ + sizeof(PfPageHdr); }

};

typedef PfPageHandle* PfPageHdl;