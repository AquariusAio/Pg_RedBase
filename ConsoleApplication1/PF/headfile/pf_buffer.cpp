#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include"pf_buffer.h"
#include"pf_filehandle.h"
#include"util.h"
using namespace std;
using namespace RedBase;
PfBuffer* PfBuffer::instance_ = nullptr;

RC PfBuffer::allocatePage(int fd, Page num, PageBuffer& addr)
{
	int idx = table_.search(fd, num);
	if (idx >= 0) {
		// 页面已经在内存中了，现在只需要增加引用即可
		nodes_[idx].count++;
	}
	else {
		idx = searchAvaliableNode();
		if (idx < 0)return PF_NOBUF;
		readPage(fd, num, nodes_[idx].buffer);
		nodes_[idx].fd = fd;
		nodes_[idx].num = num;
		nodes_[idx].count = 1;
		nodes_[idx].dirty = false;
		//table_.insert(fd, num, idx);
	}
	addr = nodes_[idx].buffer;
	return 0;
}


int PfBuffer::searchAvaliableNode()
{
	//从存储buffer所有页面的三元组内找到一个引用计数为0的位置，将其的删除并重新分配

	for (int i = 0; i <PF_MEM_BLOCKS; i++) {
		if (nodes_[i].count == 0) {
			table_.remove(nodes_[i].fd, nodes_[i].num);
			return i;
		}
	}
	return 1;
}

RC PfBuffer::pin(int fd, Page num)
{
	int idx = table_.search(fd, num);
	if (idx >= 0) {
		// 页面已经在内存中了，现在只需要增加引用即可
		nodes_[idx].count++;
	}
	else {
		// 分配一个空的页面,页面的在buff中的位置由idx记录
		idx = searchAvaliableNode();
		if (idx < 0) return PF_NOBUF;
		readPage(fd, num, nodes_[idx].buffer);
		nodes_[idx].fd = fd;
		nodes_[idx].num = num;
		nodes_[idx].count = 1;
		nodes_[idx].dirty = false;
		table_.insert(fd, num, idx);
	}
	return 0;
}

RC PfBuffer::unpin(int fd, Page num)
{
	int idx = table_.search(fd, num);
	if (idx < 0) return PF_PAGENOTINBUF;
	nodes_[idx].count -= 1;
	if (nodes_[idx].count == 0) {
		if (nodes_[idx].dirty) {
			writeBack(nodes_[idx].fd, nodes_[idx].num, nodes_[idx].buffer);
			nodes_[idx].dirty = false;
		}
	}
	return 0;
}

RC PfBuffer::readPage(int fd, Page num, PageBuffer dst)
{
	long offset = num * (long)PF_PAGE_SIZE + (long)PF_FILEHEAD_LEN;
	Lseek(fd, offset, L_SET);
	// 读取数据
	int n = Read(fd, dst, PF_PAGE_SIZE);
	if (n != PF_PAGE_SIZE) return PF_INCOMPLETEREAD;
	return 0; // 一切正常
}


//
// writeBack - 将src处的内容写回到磁盘
// 
RC PfBuffer::writeBack(int fd, Page num, PageBuffer src)
{
	// 这里的页面计数,从文件头部之后开始

	long offset = num * PF_PAGE_SIZE + (long)PF_FILEHEAD_LEN;
	Lseek(fd, offset, L_SET);

	int n = Write(fd, src, PF_PAGE_SIZE);
	if (n != PF_PAGE_SIZE) return PF_INCOMPLETEWRITE;
	return 0;
}

PageBuffer PfBuffer::getPage(int fd, Page num) {

	int idx = table_.search(fd, num);
	if (idx >= 0) {
		// 页面已经在内存中了，现在只需要增加引用即可
		nodes_[idx].count++;
	}
	else {
		// 分配一个空的页面,页面的在buff中的位置由idx记录
		idx = searchAvaliableNode();
		if (idx < 0) return nullptr;
		readPage(fd, num, nodes_[idx].buffer);
		nodes_[idx].fd = fd;
		nodes_[idx].num = num;
		nodes_[idx].count = 1;
		nodes_[idx].dirty = false;
		table_.insert(fd, num, idx);
	}
	// dst指针直接指向页的首部
	return nodes_[idx].buffer;
}

int PfBuffer::closeFile(int fd) {

	for (int i = 0; i < PF_MEM_BLOCKS; i++) {
		if (nodes_[i].fd == fd) {
			writeBack(nodes_[i].fd, nodes_[i].num, nodes_[i].buffer);
			table_.remove(fd,nodes_[i].num);
		}
	}
	return 0;
}