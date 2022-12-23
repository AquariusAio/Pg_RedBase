#include "pf_hashtable.h"
#include<list>

int LRUStrategy::replace() {

	return table_.begin()->slot;
}

int ClockStrategy::replace() {

	int cont = 0;
	vector<Triple>::iterator it;
	for (it=table_.begin()+point; cont<table_.size(); cont++,it++,point=(point+1)%table_.size()) {
		if (it->count==0)	return it->slot;
		else  it->count = 0;
	}
}

int BufferStrategy::search(int fd, Page num)
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


bool BufferStrategy::insert(int fd, Page num, int slot)
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


bool BufferStrategy::remove(int fd, Page num)
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