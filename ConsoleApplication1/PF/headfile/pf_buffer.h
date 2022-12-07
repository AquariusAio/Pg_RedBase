#pragma once
#pragma once
//数据库的内存管理，实现块的换入换出，页的所有操作首先要通过这个类分配空间，
//在这个类内创建每个页的管理的类PfPageHandle(设置文件父类，设置块脏，存储打开页的文件描述符，内存的存储起始地址等等需要与内存类互动的操作)

#include "header.h"
#include "pf_hashtable.h"

using namespace std;

struct BufferBlock {
	bool dirty;//块标记
	uint count;//引用计数
	int fd;//打开文件描述符
	Page num;//页号
	char buffer[PF_PAGE_SIZE];//文件内容,大小为2K
};

class PfBuffer
{
private:
	PfBuffer() {};
public:
	//全局只有一个PfBuffer
	static PfBuffer* instance()
	{
		if (instance_ == nullptr) {
			instance_ = new PfBuffer();
		}
		return instance_;
	}
	~PfBuffer() {};
private:
	static PfBuffer* instance_;
	BufferBlock nodes_[PF_MEM_BLOCKS];
	PfHashTable table_;
public:
	RC allocatePage(int fd, Page num, PageBuffer& addr);//从node_中分配一块区域
	PageBuffer getPage(int fd, Page num);//从node_找到某个页，返回这个页的存储指针
	RC pin(int fd, Page num);//将块钉在内存中
	RC unpin(int fd, Page num);//取消固定，可被写回外存
	int closeFile(int fd);
private:
	RC readPage(int fd, Page num, PageBuffer dst);//将块从外存读入内存
	RC writeBack(int fd, Page num, PageBuffer src);//将块写回外存
	int searchAvaliableNode();
};
