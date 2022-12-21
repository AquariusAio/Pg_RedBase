#include "pf_operator.h"
using namespace RedBase;

int OpenFile(const char* pathname, RmFileHdl& rmfile) {

	PfFileHdl file=new PfFileHandle();
	file->fd_ = Open(pathname,_O_RDWR);
	int n = Read(file->fd_,file->fileHeadInfo, sizeof(PfFileHeader));
	if (n != sizeof(PfFileHeader)) {
		Close(file->fd_);
		return PF_HDRREAD;
	}
	rmfile = new RmFileHandle(file);
	return 0;
};

int CreateFile(const char *filename ) {
	/*���ļ�д���ʼ���ļ�ͷ����Ϣ*/
	int fd;
	fd = Open(filename, O_CREAT |_O_RDWR);

	char fileHdr[PF_PAGE_SIZE];
	memset(fileHdr, -1, PF_PAGE_SIZE);

	PfFileHdr fileHead=(PfFileHdr)fileHdr;
	fileHead->freePage = 1;
	fileHead->foolPage =-1;
	fileHead->pageNum = PF_MEM_BLOCKS;

	int n = Write(fd, fileHead, sizeof(PfFileHeader));
	if (n != PF_PAGE_SIZE) {
		printf("�ļ�����ʧ��");
	}
	/*��ʼ������ҳ��ͷ����Ϣ*/
	PageIni(filename,fd);
	return 0;
}

void PageIni(const char* filename,int fd) {

/*	PfFileHdl file = new PfFileHandle();
	file->fd_ = Open(filename, _O_RDWR);
	int n = Read(file->fd_,file->fileHeadInfo, sizeof(PfFileHeader));
*/
	/*��0ҳ��ʼ��*/
	char headPage[PF_PAGE_SIZE];
	memset(headPage, -1, PF_PAGE_SIZE);
	RmFileDataPtr headData = (RmFileDataPtr)headPage;

	for (int i = 0; i < PF_MEM_BLOCKS; i++) {
		headData[i].next = i+1;
		printf("PageIni %d\n", headData[i].next);
	}
	Write(fd,headData, PF_PAGE_SIZE);

	/*��Ϣҳ��ʼ��*/
	PfPageHdrPtr ptr = new PfPageHdr();
	ItemIdData itemData[PF_LINP_NUM];
	char blankFill[PF_PAGE_SIZE - sizeof(PfPageHdr) - PF_LINP_LEN];
	memset(blankFill, -1, PF_PAGE_SIZE - sizeof(PfPageHdr) - PF_LINP_LEN);

	for (int i = 1; i < PF_MEM_BLOCKS; i++) {
		Write(fd, ptr, sizeof(PfPageHdr));
		Write(fd, itemData, sizeof(itemData));
		Write(fd, blankFill, sizeof(blankFill));
	}
	Close(fd);//�ر��ļ�
	return;
}

void CloseFile(RmFileHdl& rmfile) {
	rmfile->closeFile();
}