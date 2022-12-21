#include "pf_filehandle.h"
#include "util.h"
#include <string.h>
#include<iostream>
extern PfBuffer pfBuffer;

void PfFileHandle::getPage(Page num,PfPageHandle &page) {

	PageBuffer ptr;
	//获取页面指针
	ptr=pfBuffer.getPage(this->fd_, num);
	memcpy( &page.head_, ptr, sizeof(page.head_));
	page.setPageBuffer(ptr);
	page.setPage(num);
	page.setFile(this);

}
void PfFileHandle::setfull(Page num){

	PageBuffer ptr;
	ptr=pfBuffer.getPage(this->fd_, 0);
	
	RmFileDataPtr dataptr = (RmFileDataPtr)ptr;
	RmFileDatainfo filedata;
	long offset = 0;

	//free link change
	fileHeadInfo->freePage=dataptr[num].next;
	dataptr[num].next = 0;
	//full link change
	if (fileHeadInfo->foolPage == -1) {
		fileHeadInfo->foolPage = num;
	}
	else{
		int pos = fileHeadInfo->foolPage;
		while (dataptr[pos] .next!= 0) {
			
			pos = dataptr[pos].next;
		}
		dataptr[pos].next = num;
	}
    //写回文件头部
	pfBuffer.writeBackPage(fd_, 0);
	RedBase::Lseek(fd_,0, L_SET);
	RedBase::Write(fd_, &fileHeadInfo, sizeof(PfFileHdr));
}

void PfFileHandle::fileWriteBack() {

	pfBuffer.closeFile(this->fd_);
}
