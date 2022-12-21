#include "rm_filehandle.h"
#include<iostream>
extern PfBuffer pfBuffer;
RmFileHandle::RmFileHandle(PfFileHdl file) {
	this->pffile_ = file;
	/*����ȱʧ*/
	this->fileDataPtr = nullptr;
	this->pagehandle = new RmPageHandle();
}

int RmFileHandle::insertRcord( char* record) {

	PfPageHandle page;
	Page num;
	PageSlot offset;
	
	freePagehandle(page);//����һ��ҳ��
	offset = nextSlot(page);//��ȡ����λ��ƫ������Ҳ��slot
	PageBuffer pageptr = page.getInsertBuffer()+offset;//��ò���λ��ָ��
	memcpy(pageptr, record, strlen(record));//�����¼

	insertEndProcess(page, strlen(record));//����
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

	int full = pagehandle->setUsed(page, rcdlen);
	page.setDirty();
	//ҳ��
	if (full) {
		pffile_->setfull(pffile_->fileHeadInfo->freePage);
		std::cout<<"ALLOCTE NEW FREE PAGE "<<pffile_->fileHeadInfo->freePage<<endl;
	}
	
}



int RmFileHandle::getRcd(Rid & rid, RmRid & rmRid) {

	Page num = rid.getPage();
	PageSlot slot = rid.getSlot();
	PfPageHandle page;
	RmPageHandle pageHdl;

	pffile_->getPage(num, page);
	if (!pageHdl.isValid(page, slot)) return 0;//slot��������ʹ��״̬

	PageBuffer dataptr = page.getInsertBuffer()+pageHdl.getOffset(page,slot);
	rmRid.setBuffer(dataptr);

	return 1;
}


int RmFileHandle::catFile(Page num) {

	PfPageHandle page;
	pffile_->getPage(num,page);
	pagehandle->printPage(page);
}


int RmFileHandle::closeFile() {

	pffile_->fileWriteBack();
	return 0;
}