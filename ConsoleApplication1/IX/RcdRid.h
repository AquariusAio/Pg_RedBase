#pragma once
#include"../PF/headfile/header.h"


class Rid {
	Page num;
	PageSlot slot;
public:
	Rid(Page page, PageSlot pageslot) :num(page), slot(pageslot) {};
	~Rid() {};
	Page getPage() { return num; }
	PageSlot getSlot() { return slot; }
};

#define RID_LEN sizeof(Rid)