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
	int setUsed(PfPageHandle&, unsigned);                   //��ĳlinp״̬��Ϊʹ��  
	void printPage(PfPageHandle& page);                      
	int isValid(PfPageHandle& page,PageSlot slot);           //�ж�λ�õ�״̬
	unsigned getOffset(PfPageHandle& page, PageSlot slot);   //��ü�¼�ļ�¼ƫ��
};
typedef RmPageHandle* RmPageHdl;