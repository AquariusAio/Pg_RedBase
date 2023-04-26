#include "IX_Handle.h"
#include"../PF/headfile/util.h"
#include<iostream>
extern PfBuffer pfBuffer;
IXHandle::IXHandle(PfFileHdl file,Attributes attr,bool create=true):file_(file),attr_(attr)
{
	
	PfPageHandle page;
	file_->getPage(0, page);
	PageBuffer ptr = page.getPageBuffer();
	memcpy(&head_, ptr, sizeof(BtreeFileHeader));
	//�����ҳ
	if (create) {
		PfPageHdl rootpage = new PfPageHandle();
		file_->getPage(head_.root, *rootpage);
		root_ = new TreeNode(rootpage, attr_.type, head_.capacity);
	}
}

IXHandle::~IXHandle() {

	//�����ļ���Ϣ��д��
	PfPageHandle page;
	file_->getPage(0, page);
	memcpy(page.getPageBuffer(), &head_, sizeof(BtreeFileHeader));
	page.setDirty();

	//����ʹ��  д�������ļ���Ϣ
	pfBuffer.writeBackPage(file_->fd_, page.getPage());
	delete(root_);
}

bool IXHandle::createIndex(Attributes attr) {
	PfPageHandle page;
	file_->getPage(0, page);
	BtreeFileHeader head;
	head.attrtype = attr.type;
	head.capacity = (PF_PAGE_SIZE-sizeof(BtreeNodeheader))/(attr.attrlen+sizeof(Rid))-1;
	head.height = 1;
	head.leaf = -1;
	head.root = 1;
	head.used = 1;
	head.keylen = attr.attrlen;
	memcpy(page.getPageBuffer(),&head,sizeof(BtreeFileHeader));
	memcpy(&head_, &head, sizeof(BtreeFileHeader));
	page.setDirty();


	//root���Ĵ���
	BtreeNodeheader nodehead;
	nodehead.keylen = attr.attrlen;
	nodehead.keyused = 0;
	nodehead.left = -1;
	nodehead.right = -1;
	nodehead.type = attr.type;

	PfPageHdl rootpage = new PfPageHandle();
	file_->getPage(head_.root, *rootpage);
	memcpy(rootpage->getPageBuffer(), &nodehead, sizeof(BtreeFileHeader));
	rootpage->setDirty();

	//root���Ĵ���
	root_ = new TreeNode(rootpage, attr_.type, head_.capacity);
}

bool IXHandle::insertIndex(keyPtr rcd, Rid rid) {

	keyPtr key;
	key = malloc(head_.keylen);
	memcpy(key, rcd, head_.keylen);
	//��ֵ�ĸ���
	if (strcmp((char*)key, "ajxlog") == 0) {
		printf("gfgihq");
	}
	std::cout << "IXHandle insertIndex  " <<file_->fileHeadInfo->freePage<<" " << (char*)key << endl;
	bool fix = true,rootfix=false;
	rootfix=insertIndex(root_, 1, key, rid, fix);//�Ӹ����ݹ����

	if (!fix && rootfix) {
		//��������
		TreeNodePtr newroot = newTreeNode();
		if (root_->getRightPage() == rid.getPage()) {
			newroot->keyUpdate(root_->getKey(0), 0);
			newroot->keyUpdate(key, 1);
			newroot->ridUpdate(Rid(root_->getPage(), -1), 0);
			newroot->ridUpdate(Rid(rid.getPage(),-1), 1);
		}
		else {
			newroot->keyUpdate(key, 0);
			newroot->keyUpdate(root_->getKey(0), 1);
			newroot->ridUpdate(Rid(rid.getPage(), -1), 0);
			newroot->ridUpdate(Rid(root_->getPage(), -1), 1);
		}
		newroot->keyused_ = 2;
		delete(root_);
		root_ = newroot;
		Rid* next;
		next = root_->getRid(1);
		head_.height += 1;
		head_.root = newroot->getPage();
		free(key);
	}
	//for (int i = 0; i <root_->keyused_; i++) std::cout << root_->rids_[i].getPage() << root_->rids_[i].getSlot() << endl;
	return false;

}

bool IXHandle::insertIndex(TreeNodePtr node, int height, keyPtr key, Rid &rid,bool &fix) {
	
	std::cout << "NODE HEAD:Page " << node->getPage() << "; KEYUSED " << node->keyused_ << "; KEYLEN " << node->keylen_ << endl;
	int pos = -1;
	if (height == head_.height) {
		//����Ҷ�ӽ�㣬��ҳ�ڲ���key
		int idx = node->search(key, rid);
		if (idx) {
			//�������Ѿ�����
			fix = true;
			return false;
		}
	}
	else {
		Rid *next;
		pos = node->nodeSearch(key);
		if (pos < 0) {
			//����С�Ľ��ָ�룬�����
			//���˽ڵ��0λ�õ�key���£�ȡ���������ӽڵ�
			pos = 0;
			node->keyUpdate(key,0);
			next = node->getRid(0);
		}
		else {
			//�ҵ��м����ָ��
			next = node->getRid(pos);
		}
		TreeNode *nextNode = getNode(next->getPage());
		bool fixup = insertIndex(nextNode, height + 1, key, rid, fix);
		delete(nextNode);
		if (fix) return fixup;
	}

	if (node->keyused_ < head_.capacity) {
		fix = true;//����Ҫ���ϵ�����
		//�������������ΪҶ�ӽ���ڴ˽�����
		if (height == head_.height) node->insertLeaf(key, rid);
		else node->insertInternal(key, rid, pos + 1);
	}
	else {
		//�����Ҫ����
		fix = false;
		TreeNodePtr newNode = newTreeNode();
		NodeDevide(node, newNode);

		//����rid
		if (node->Comp(newNode->getKey(0), key) < 0) newNode->insertLeaf(key, rid);
		else node->insertLeaf(key, rid);

		//������Ҫ�����keyֵ��ridֵ
		memcpy(key, newNode->getKey(0), node->keylen_);
		rid.setPage(newNode->getPage());
		rid.setSlot(-1);

		//��Ҷ�����ĵ���
		if (head_.leaf == -1) head_.leaf = node->getPage();
		newNode->setRightPage(node->getRightPage());
		if (node->getRightPage() != -1) {
			TreeNodePtr right = getNode(node->getRightPage());
			right->setLeftPage(newNode->getPage());
		}
		newNode->setLeftPage(node->getPage());
		node->setRightPage(newNode->getPage());

		delete(newNode);
	}
	return true;
}

TreeNode* IXHandle::getNode(Page num) {

	if (num == 4) {
		printf("\n");
	}
	PfPageHdl page=new PfPageHandle();
	file_->getPage(num, *page);
	return new TreeNode(page, attr_.type, head_.capacity);
}

TreeNode* IXHandle::newTreeNode() {
	if (file_->fileHeadInfo->freePage == 1) {
		printf("\n");
	}
	std::cout << "new Node " << file_->fileHeadInfo->freePage<<endl;
	//ͷ����Ϣ
	BtreeNodeheader nodehead;
	nodehead.keylen = attr_.attrlen;
	nodehead.keyused = 0;
	nodehead.left = -1;
	nodehead.right = -1;
	nodehead.type = attr_.type;

	//�����ļ���ͷ��Ϣ���޸�
	file_->fileHeadInfo->freePage++;
	Page num = file_->fileHeadInfo->freePage;
	RedBase::Write(file_->fd_, file_->fileHeadInfo, sizeof(PfFileHdr));

	//д��ҳͷ�����
	PfPageHdl page=new PfPageHandle();
	file_->getPage(num, *page);
	memcpy(page->getPageBuffer(), &nodehead, sizeof(BtreeNodeheader));
	page->setDirty();
	return new TreeNode(page, attr_.type, head_.capacity);
}

void IXHandle::NodeDevide(TreeNodePtr src_, TreeNodePtr new_) {

	int splitpos = src_->keyused_ / 2;
	std::cout << "BTree Node Devide  " << src_->getPage() << endl;
	keyPtr key;
	for (int i = 0; i < src_->keyused_; i++) {
		key = malloc(src_->keylen_); 
		memcpy(key, src_->getKey(i), src_->keylen_); 
		std::cout << (char*)key << endl;
		if (i == splitpos) cout << "����������������������������������������" << endl;
	}
	//��Դ��㽫��벿�ָ��Ƶ��½��ǰ�벿��
	memmove(new_->getKey(0), src_->getKey(splitpos), (src_->keyused_ - splitpos) * src_->keylen_);
	memmove(new_->getRid(0), src_->getRid(splitpos), (src_->keyused_ - splitpos) * sizeof(Rid));
	
	//�޸��������ʹ�õ�key����
	new_->keyused_ = (src_->keyused_ - splitpos);
	src_->keyused_ = splitpos;
}