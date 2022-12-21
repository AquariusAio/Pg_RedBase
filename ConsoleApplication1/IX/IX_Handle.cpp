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


	//root结点的创建
	BtreeNodeheader nodehead;
	nodehead.keylen = attr.attrlen;
	nodehead.keyused = 0;
	nodehead.left = -1;
	nodehead.right = -1;
	nodehead.type = attr.type;

	file_->getPage(1, page);
	memcpy(page.getPageBuffer(), &nodehead, sizeof(BtreeFileHeader));
	page.setDirty();
	
	//root结点的根新
	root_ = new TreeNode(&page, attr_.type, head_.capacity);
}

bool IXHandle::insertIndex(keyPtr rcd, Rid rid) {

	keyPtr key;
	key = malloc(head_.keylen);
	memcpy(key, rcd, head_.keylen);
	//键值的复制

	std::cout << "IXHandle insertIndex" << *(int*)key << endl;
	bool fix = true,rootfix=false;
	rootfix=insertIndex(root_, 1, key, rid, fix);//从根结点递归插入

	if (!fix && rootfix) {
		//根结点分裂
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
		//到达叶子结点，在页内查找key
		int idx = node->search(key, rid);
		if (idx) {
			//此索引已经存在
			fix = true;
			return false;
		}
	}
	else {
		Rid *next;
		pos = node->nodeSearch(key);
		if (pos < 0) {
			//找最小的结点指针，最左侧
			//将此节点的0位置的key更新，取出最左侧的子节点
			pos = 0;
			node->keyUpdate(key,0);
			next = node->getRid(0);
		}
		else {
			//找到中间结点的指针
			next = node->getRid(pos);
		}
		TreeNode *nextNode = getNode(next->getPage());
		bool fixup = insertIndex(nextNode, height + 1, key, rid, fix);
		if (fix) return fixup;
	}

	if (node->keyused_ < head_.capacity) {
		fix = true;//不需要向上调整了
		//结点容量够，若为叶子结点在此结点插入
		if (height == head_.height) node->insertLeaf(key, rid);
		else node->insertInternal(key, rid, pos + 1);
	}
	else {
		//结点需要分裂
		fix = false;
		TreeNodePtr newNode = newTreeNode();
		NodeDevide(node, newNode);

		//插入rid
		if (node->Comp(newNode->keys_[0], key) < 0) newNode->insertLeaf(key, rid);
		else node->insertLeaf(key, rid);

		//更新需要插入的key值和rid值
		memcpy(key, node->getKey(0), node->keylen_);
		rid = Rid(newNode->getPage, -1);

		//树叶子链的调整
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

	//头部信息
	BtreeNodeheader nodehead;
	nodehead.keylen = attr.attrlen;
	nodehead.keyused = 0;
	nodehead.left = -1;
	nodehead.right = -1;
	nodehead.type = attr.type;
	
	//索引文件的头信息的修改
	Page num = file_->fileHeadInfo->freePage;
	file_->fileHeadInfo->freePage++;
	RedBase::Write(file_->fd_, file_->fileHeadInfo, sizeof(PfFileHdr));

	//写入页头部结点
	PfPageHandle page;
	file_->getPage(num, page);
	memcpy(page.getPageBuffer(), &nodehead, sizeof(BtreeNodeheader));

	return new TreeNode(&page, attr_.type, head_.capacity);
}

void IXHandle::NodeDevide(TreeNodePtr src_, TreeNodePtr new_) {

	int splitpos = src_->keyused_ / 2;

	//从源结点将后半部分复制到新结点前半部分
	memmove(new_->getKey(0), src_->getKey(splitpos), (src_->keyused_ - splitpos) * src_->keylen_);
	memmove(new_->getRid(0), src_->getRid(splitpos), (src_->keyused_ - splitpos) * sizeof(Rid));
	
	//修改两个结点使用的key个数
	new_->keyused_ = (src_->keyused_ - splitpos);
	src_->keyused_ = splitpos;
}