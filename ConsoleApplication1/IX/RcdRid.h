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
	void setPage(Page page) { num = page; }
	void setSlot(PageSlot pageslot) { slot = pageslot; }
	bool ridEqual(Rid& rid) { if ((rid.getPage() == num) && (rid.getSlot() == slot)) return true;return false; }
};

#define RID_LEN sizeof(Rid)