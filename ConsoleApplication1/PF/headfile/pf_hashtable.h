#pragma once
using namespace std;
#include"header.h"
#include<vector>
#include<list>
class PfHashTable
{
	struct Triple {
		int fd;
		Page num;
		int slot;
		Triple(int fd, Page num, int slot) : fd(fd), num(num), slot(slot) {}
	};
public:
	PfHashTable(uint capacity=PF_MEM_BLOCKS);
	~PfHashTable();
public:
	int search(int fd, Page num);
	bool insert(int fd, Page num, int slot);
	bool remove(int fd, Page num);
private:
	int calcHash(int fd, Page num)
	{
		return (fd + num) % PF_MEM_BLOCKS;
	}
private:
	uint capacity_;
	vector<Triple> table_;
};

