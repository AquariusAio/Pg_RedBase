#include "IX_Handle.h"
#include"../PF/headfile/util.h"
#include<iostream>

IXHandle::IXHandle(PfFileHdl file,Attributes attr):file_(file),attr_(attr)
{
	
	PfPageHandle page;
	file_->getPage(0, page);
	PageBuffer ptr = page.getPageBuffer();
	memcpy(&head_, ptr, sizeof(BtreeFileHeader));
	file_->getPage(1, page);
	root_ = new TreeNode(&page, attr_.type, head_.capacity);

}

bool IXHandle::createIndex(Attributes attr) {
	PfPageHandle page;
	file_->getPage(0, page);
	BtreeFileHeader head;
	head.attrtype = attr.type;
	head.capacity = PF_PAGE_SIZE/attr.attrlen;
	head.height = 1;
	head.leaf = -1;
	head.root = 1;
	head.used = 1;
	head.keylen = attr.attrlen;
	memcpy(page.getPageBuffer(),&head,sizeof(BtreeFileHeader));
	page.setDirty();


	//root���Ĵ���
	BtreeNodeheader nodehead;
	nodehead.keylen = attr.attrlen;
	nodehead.keyused = 0;
	nodehead.left = -1;
	nodehead.right = -1;
	nodehead.type = attr.type;

	file_->getPage(1, page);
	memcpy(page.getPageBuffer(), &nodehead, sizeof(BtreeFileHeader));
	page.setDirty();
	
	//root���ĸ���
	root_ = new TreeNode(&page, attr_.type, head_.capacity);
}

bool IXHandle::insertIndex(keyPtr rcd, Rid rid) {

	keyPtr key;
	key = malloc(head_.keylen);
	memcpy(key, rcd, head_.keylen);
	//��ֵ�ĸ���

	std::cout << "IXHandle insertIndex" << *(int*)key << endl;
	bool fix = true,rootfix=false;
	rootfix=insertIndex(root_, 1, key, rid, fix);//�Ӹ����ݹ����

	if (!fix && rootfix) {
		//��������
		TreeNodePtr newroot = new TreeNode();
		if (root_->getRightPage() == rid.getPage()) {
			newroot->keyUpdate(root_->getKey(0), 0);
			newroot->keyUpdate(key, 1);
			newroot->ridUpdate(Rid(root_->getPage(), -1), 1);
			newroot->ridUpdate(key, 0);
		}
		else {
			newroot->keyUpdate(key, 0);
			newroot->keyUpdate(root_->getKey(0), 1);
			newroot->ridUpdate(rid, 0);
			newroot->ridUpdate(Rid(root_->getPage(), -1), 1));
		}
		newroot->keyused_ = 2;
		root_ = newroot;
		head_.height += 1;
		free(key);
	}
	return false;

}

bool IXHandle::insertIndex(TreeNodePtr node, int height, keyPtr key, Rid &rid,bool &fix) {
	
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
		if (node->Comp(newNode->keys_[0], key) < 0) newNode->insertLeaf(key, rid);
		else node->insertLeaf(key, rid);

		//������Ҫ�����keyֵ��ridֵ
		memcpy(key, node->getKey(0), node->keylen_);
		rid = Rid(newNode->getPage, -1);

		//��Ҷ�����ĵ���
		newNode->setRightPage(node->getRightPage());
		if (right != -1) {
			TreeNodePtr right = getNode(node->getRightPage());
			right->setLeftPage(newNode->getPage());
		}
		newNode->setLefitPage(node->getPage());
		node->setRightPage(newNode->getPage());
	}
	return true;
}

TreeNode* IXHandle::getNode(Page num) {

	PfPageHandle page;
	file_->getPage(num, page);
	return new TreeNode(&page, attr_.type, head_.capacity);
}

TreeNode* IXHandle::newTreeNode() {

	//ͷ����Ϣ
	BtreeNodeheader nodehead;
	nodehead.keylen = attr.attrlen;
	nodehead.keyused = 0;
	nodehead.left = -1;
	nodehead.right = -1;
	nodehead.type = attr.type;
	
	//�����ļ���ͷ��Ϣ���޸�
	Page num = file_->fileHeadInfo->freePage;
	file_->fileHeadInfo->freePage++;
	RedBase::Write(file_->fd_, file_->fileHeadInfo, sizeof(PfFileHdr));

	//д��ҳͷ�����
	PfPageHandle page;
	file_->getPage(num, page);
	memcpy(page.getPageBuffer(), &nodehead, sizeof(BtreeNodeheader));

	return new TreeNode(&page, attr_.type, head_.capacity);
}

void IXHandle::NodeDevide(TreeNodePtr src_, TreeNodePtr new_) {

	int splitpos = src_->keyused_ / 2;

	//��Դ��㽫��벿�ָ��Ƶ��½��ǰ�벿��
	memmove(new_->getKey(0), src_->getKey(splitpos), (src_->keyused_ - splitpos) * src_->keylen_);
	memmove(new_->getRid(0), src_->getRid(splitpos), (src_->keyused_ - splitpos) * sizeof(Rid));
	
	//�޸��������ʹ�õ�key����
	new_->keyused_ = (src_->keyused_ - splitpos);
	src_->keyused_ = splitpos;
}