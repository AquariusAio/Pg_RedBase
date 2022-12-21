#include "pf_hashtable.h"
#include<list>
PfHashTable::PfHashTable(uint capacity)
	: capacity_(capacity)
{
}

//
// search - 在表中搜寻这样的三元组,找到了,返回即可,否则的话,返回-1
// 三元组包含有 文件描述符-页数-槽
int PfHashTable::search(int fd, Page num)
{
	//if (table_.empty()) return -1;
	int key = calcHash(fd, num);
	if (key < 0) return -1;
	vector<Triple>::iterator it;
	for (it = table_.begin(); it != table_.end(); it++) {
		if ((it->fd == fd) && (it->num == num))
			return it->slot;
	}
	return -1;
}


//
// insert - 往hash表中插入一个元素
// 
bool PfHashTable::insert(int fd, Page num, int slot)
{
	int key = calcHash(fd, num);
	if (key < 0) return false;
	vector<Triple>::const_iterator it;
	// table中不能已经存在这样的entry
	for (it = table_.begin(); it != table_.end(); it++) {
		if ((it->fd == fd) && (it->num == num))
			return false;
	}
	table_.push_back(Triple(fd, num, slot));
	return true;
}

//
// remove - 从hash表中移除掉一个元素
// 
bool PfHashTable::remove(int fd, Page num)
{	
	int key = calcHash(fd, num);	
	if (key < 0) return false;
	vector<Triple>::iterator it;
	// table中不能已经存在这样的entry
	for (it = table_.begin(); it != table_.end(); it++) {
		if ((it->fd == fd) && (it->num == num)) {
			table_.erase(it);
			return true;
		}
	}
}