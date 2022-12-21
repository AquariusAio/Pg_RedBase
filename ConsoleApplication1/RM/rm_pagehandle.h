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
	int setUsed(PfPageHandle&, unsigned);                   //将某linp状态设为使用  
	void printPage(PfPageHandle& page);                      
	int isValid(PfPageHandle& page,PageSlot slot);           //判断位置的状态
	unsigned getOffset(PfPageHandle& page, PageSlot slot);   //获得记录的记录偏移
};
typedef RmPageHandle* RmPageHdl;