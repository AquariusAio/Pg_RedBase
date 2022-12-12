#include "rm_filescan.h"

int RmFileScan::getNextRcd(RmRid& rid) {

	Page pages = file_->pffile_->fileHeadInfo->pageNum;
	Page num = curr_.getPage();
	RmPageHandle pageHdl;

	
	for (;curr_.getPage()<num;curr_.setPage(curr_.getPage()+1)) {

		while (true) {
			switch (file_->(getRcd(curr_,rid)))
			{
			case -1:break; break;						//需要换页
			case 1:rid.buffer_+=offset_; return 1;		//记录执政加上内部偏移
			case 0:continue; break;                     //当前slot并未使用
			}
		}
		
	}

	return SCAN_EOF;
}