#include "pf_filehandle.h"
#include <string.h>
extern PfBuffer pfBuffer;

void PfFileHandle::getPage(Page num,PfPageHandle &page) {

	PageBuffer ptr;
	//��ȡҳ��ָ��
	ptr=pfBuffer.getPage(this->fd_, num);
	memcpy( &page.head_, ptr, sizeof(page.head_));
	page.setPageBuffer(ptr);
	page.setPage(num);

}

void PfFileHandle::fileWriteBack() {

	pfBuffer.closeFile(this->fd_);
}
