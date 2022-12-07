#pragma once
//����ҳ�Ŀ�ţ�����ҳ���ڴ�洢λ��ָ�룬ʵ��ҳ�Ĺ̶���ȡ���Ȳ���
#include"header.h"
#include"pf_pageheader.h"
class PfPageHandle
{
public:
	PfPageHandle() :num(-1), addr_(nullptr), dirty_(false) {}
	~PfPageHandle() {};
public:
	PfPageHdr head_;
private:
	bool dirty_;
	Page num;
	PageBuffer addr_;
	//weak_ptr<PfFileHandle*> pffile_;
public:
	void pin();
	void unpin() ;
	void setDirty() {};
public:
	//void setFile(shared_ptr<PfFileHandle*> file) { pffile_ = file; }
	void setPageBuffer(PageBuffer ptr) { addr_ = ptr; }
	void setPage(Page num) { this->num = num; }
	PageBuffer getPageBuffer() { return this->addr_; }
	PageBuffer getInsertBuffer() { return this->addr_ + PF_LINP_LEN; }
	PageBuffer getLinpBuffer() { return this->addr_ + sizeof(PfPageHdr); }

};

typedef PfPageHandle* PfPageHdl;