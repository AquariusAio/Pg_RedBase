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
		// ҳ���Ѿ����ڴ����ˣ�����ֻ��Ҫ�������ü���
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
	//�Ӵ洢buffer����ҳ�����Ԫ�����ҵ�һ�����ü���Ϊ0��λ�ã������ɾ�������·���

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
		// ҳ���Ѿ����ڴ����ˣ�����ֻ��Ҫ�������ü���
		nodes_[idx].count++;
	}
	else {
		// ����һ���յ�ҳ��,ҳ�����buff�е�λ����idx��¼
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
	// ��ȡ����
	int n = Read(fd, dst, PF_PAGE_SIZE);
	if (n != PF_PAGE_SIZE) return PF_INCOMPLETEREAD;
	return 0; // һ������
}


//
// writeBack - ��src��������д�ص�����
// 
RC PfBuffer::writeBack(int fd, Page num, PageBuffer src)
{
	// �����ҳ�����,���ļ�ͷ��֮��ʼ

	long offset = num * PF_PAGE_SIZE + (long)PF_FILEHEAD_LEN;
	Lseek(fd, offset, L_SET);

	int n = Write(fd, src, PF_PAGE_SIZE);
	if (n != PF_PAGE_SIZE) return PF_INCOMPLETEWRITE;
	return 0;
}

PageBuffer PfBuffer::getPage(int fd, Page num) {

	int idx = table_.search(fd, num);
	if (idx >= 0) {
		// ҳ���Ѿ����ڴ����ˣ�����ֻ��Ҫ�������ü���
		nodes_[idx].count++;
	}
	else {
		// ����һ���յ�ҳ��,ҳ�����buff�е�λ����idx��¼
		idx = searchAvaliableNode();
		if (idx < 0) return nullptr;
		readPage(fd, num, nodes_[idx].buffer);
		nodes_[idx].fd = fd;
		nodes_[idx].num = num;
		nodes_[idx].count = 1;
		nodes_[idx].dirty = false;
		table_.insert(fd, num, idx);
	}
	// dstָ��ֱ��ָ��ҳ���ײ�
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