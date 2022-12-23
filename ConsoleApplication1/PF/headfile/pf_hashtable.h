#pragma once
using namespace std;
#include"header.h"
#include<vector>
#include<list>

enum BlockType {
	DATA,
	LOG,
	DIC
};

struct BufferBlock {
	bool dirty;//块标记
	uint count;//引用计数
	int fd;//打开文件描述符
	Page num;//页号
	BlockType type;//块功能类型
	char buffer[PF_PAGE_SIZE];//文件内容
};

//——————————置换管理————————
class BufferStrategy
{
	struct Triple {
		int fd;
		Page num;
		int slot;
		Triple(int fd, Page num, int slot) : fd(fd), num(num), slot(slot) {}
	};
public:
	BufferStrategy(uint capacity = PF_MEM_BLOCKS) : capacity_(capacity) {}
	~BufferStrategy() {};
public:
	 int search(int fd, Page num);
	bool insert(int fd, Page num, int slot);
	bool remove(int fd, Page num);
	int replace() {};
private:
	int calcHash(int fd, Page num)
	{
		return (fd + num) % PF_MEM_BLOCKS;
	}
protected:
	uint capacity_;
	vector<Triple> table_;
};

//——————————LRU置换——————————
class LRUStrategy :public BufferStrategy {
	struct Triple {
		int fd;
		Page num;
		int slot;
		Triple(int fd, Page num, int slot) : fd(fd), num(num), slot(slot) {}
	};
protected:
	uint capacity_;
	vector<Triple> table_;
public:
	int replace();
};

//————————CLOCK置换————————————
class ClockStrategy :public BufferStrategy {
	int point;
	struct Triple {
		int fd;
		Page num;
		int slot;
		int count;
		Triple(int fd, Page num, int slot) : fd(fd), num(num), slot(slot),count(1) {}
	};
protected:
	uint capacity_;
	vector<Triple> table_;
public:
	ClockStrategy(uint capacity = PF_MEM_BLOCKS) :capacity_(capacity), point(0) {}
	int replace();
};
