#pragma once
#pragma once
//���ݿ���ڴ����ʵ�ֿ�Ļ��뻻����ҳ�����в�������Ҫͨ����������ռ䣬
//��������ڴ���ÿ��ҳ�Ĺ������PfPageHandle(�����ļ����࣬���ÿ��࣬�洢��ҳ���ļ����������ڴ�Ĵ洢��ʼ��ַ�ȵ���Ҫ���ڴ��໥���Ĳ���)

#include "header.h"
#include "pf_hashtable.h"

using namespace std;

struct BufferBlock {
	bool dirty;//����
	uint count;//���ü���
	int fd;//���ļ�������
	Page num;//ҳ��
	char buffer[PF_PAGE_SIZE];//�ļ�����,��СΪ2K
};

class PfBuffer
{
private:
	PfBuffer() {};
public:
	//ȫ��ֻ��һ��PfBuffer
	static PfBuffer* instance()
	{
		if (instance_ == nullptr) {
			instance_ = new PfBuffer();
		}
		return instance_;
	}
	~PfBuffer() {};
private:
	static PfBuffer* instance_;
	BufferBlock nodes_[PF_MEM_BLOCKS];
	PfHashTable table_;
public:
	RC allocatePage(int fd, Page num, PageBuffer& addr);//��node_�з���һ������
	PageBuffer getPage(int fd, Page num);//��node_�ҵ�ĳ��ҳ���������ҳ�Ĵ洢ָ��
	RC pin(int fd, Page num);//���鶤���ڴ���
	RC unpin(int fd, Page num);//ȡ���̶����ɱ�д�����
	int closeFile(int fd);
	void writeBackPage(int,Page num);
private:
	RC readPage(int fd, Page num, PageBuffer dst);//������������ڴ�
	RC writeBack(int fd, Page num, PageBuffer src);//����д�����
	int searchAvaliableNode();
};
