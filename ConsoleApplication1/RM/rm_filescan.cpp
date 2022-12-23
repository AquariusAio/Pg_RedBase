#include "rm_filescan.h"

int RmFileScan::getNextRcd(RmRid& rid,PageBuffer ptr) {

	Page pages = file_->pffile_->fileHeadInfo->pageNum;
	Page num = curr_.getPage();

	
	for (;curr_.getPage()<pages;curr_.setPage(curr_.getPage()+1)) {
		bool contro = true;
		if (curr_.getSlot() < 0)curr_.setSlot(0);
		while (contro) {
			switch (file_->getRcd(curr_,rid))
			{
			case -1:contro = false; curr_.setSlot(-1); break;	//��һҳ��ͷɨ��					//��Ҫ��ҳ
			case 1:rid.rid_.setPage(curr_.getPage()); rid.rid_.setSlot(curr_.getSlot()); rid.buffer_ += offset_; curr_.setSlot(curr_.getSlot() + 1); return 1;		//��¼ִ�������ڲ�ƫ��
			case 0:curr_.setSlot(curr_.getSlot() + 1); continue; break;                     //��ǰslot��δʹ��
			}
			curr_.setSlot(curr_.getSlot() + 1);
		}
		
	}
	printf("rid end");
	return SCAN_EOF;
}