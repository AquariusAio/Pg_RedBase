#pragma once
#include "../IX/RcdRid.h"
#include "../PF/headfile/header.h"
class RmRid {
public:
	RmRid(Rid& rid, int rcdlen) :rid_(rid), buffer_(nullptr), rcdlen_(rcdlen) {};
	~RmRid() {}
public:
	void setBuffer(PageBuffer ptr) { buffer_ = ptr; }
	void setRid(Rid& rid) { rid_ = rid; }
	void setRcdlen(int rcdlen) { rcdlen_ = rcdlen; }
public:
	Rid rid_;
	PageBuffer buffer_;
	int rcdlen_;
};