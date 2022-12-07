#include "rm_filehandle.h"
extern PfBuffer pfBuffer;
RmFileHandle::RmFileHandle(PfFileHdl file) {
	this->pffile_ = file;
	/*代码缺失*/
	this->fileDataPtr = nullptr;
	this->pagehandle = new RmPageHandle();
}

int RmFileHandle::insertRcord( char* record) {

	PfPageHandle page;
	Page num;
	PageSlot offset;
	
	freePagehandle(page);//分配一个页面
	offset = nextSlot(page);//获取插入位置偏移量，也即slot
	PageBuffer pageptr = page.getInsertBuffer()+offset;//获得插入位置指针
	memcpy(pageptr, record, sizeof(record));//插入记录

	insertEndProcess(page, sizeof(record));//后处理
	return 0;
}


Page RmFileHandle::freePage() {
	return pffile_->fileHeadInfo->freePage;
}


void RmFileHandle::freePagehandle(PfPageHandle& page) {

	Page num = freePage();
	pffile_->getPage(num, page);
	
}


unsigned RmFileHandle::nextSlot(PfPageHandle& page) {
	
	return pagehandle->nextSlot(page);
}


void RmFileHandle::insertEndProcess(PfPageHandle& page,unsigned rcdlen) {

	pagehandle->setUsed(page, rcdlen);
	page.setDirty();
}


int RmFileHandle::closeFile() {

	pffile_->fileWriteBack();
	return 0;
}

int RmFileHandle::catFile(Page num) {

	PfPageHandle page;
	pffile_->getPage(num,page);
	pagehandle->printPage(page);
}