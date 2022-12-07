#pragma once
#include "../PF//headfile/pf_pageheader.h"
#include "../PF/headfile/pf_buffer.h"
#include "../PF/headfile/pf_pagehandle.h"
#include<string.h>
class RmPageHandle {

public:
	RmPageHandle() {};
	~RmPageHandle() {};
public:
	unsigned nextSlot(PfPageHandle& page);
	void setUsed(PfPageHandle&, unsigned);
	void printPage(PfPageHandle& page);
};
typedef RmPageHandle* RmPageHdl;