#include "pf_hashtable.h"
#include<list>
PfHashTable::PfHashTable(uint capacity)
	: capacity_(capacity)
{
}

//
// search - �ڱ�����Ѱ��������Ԫ��,�ҵ���,���ؼ���,����Ļ�,����-1
// ��Ԫ������� �ļ�������-ҳ��-��
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
// insert - ��hash���в���һ��Ԫ��
// 
bool PfHashTable::insert(int fd, Page num, int slot)
{
	int key = calcHash(fd, num);
	if (key < 0) return false;
	vector<Triple>::const_iterator it;
	// table�в����Ѿ�����������entry
	for (it = table_.begin(); it != table_.end(); it++) {
		if ((it->fd == fd) && (it->num == num))
			return false;
	}
	table_.push_back(Triple(fd, num, slot));
	return true;
}

//
// remove - ��hash�����Ƴ���һ��Ԫ��
// 
bool PfHashTable::remove(int fd, Page num)
{	
	int key = calcHash(fd, num);	
	if (key < 0) return false;
	vector<Triple>::iterator it;
	// table�в����Ѿ�����������entry
	for (it = table_.begin(); it != table_.end(); it++) {
		if ((it->fd == fd) && (it->num == num)) {
			table_.erase(it);
			return true;
		}
	}
}