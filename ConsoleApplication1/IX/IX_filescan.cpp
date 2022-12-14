#include"IX_filescan.h"
#include<iostream>
#include<iomanip>
void IXFileScan::openScan(PfFileHdl file, Attributes attr){

	file_ = file;
	attr_ = attr;
	PfPageHandle page;
	file_->getPage(0, page);
	memcpy(&head_, page.getPageBuffer(), sizeof(BtreeFileHeader));

}

int IXFileScan::getNextIndex(keyPtr key,RmRid&rid) {

	if (curr_.getSlot() == -1) getNextIndex(key);

	PfPageHandle page;
	file_->getPage(curr_.getPage(), page);
	BtreeNodeheader head;
	memcpy(&head, page.getPageBuffer(), sizeof(BtreeNodeheader));
	if (curr_.getSlot() < head.keyused) {
		rid.rid_.setPage(curr_.getPage());
		rid.rid_.setSlot(curr_.getSlot());
		curr_.setSlot(curr_.getSlot() + 1);
		return 1;
	}
}

int IXFileScan::getNextIndex(keyPtr key){

	PfPageHandle page;
	file_->getPage(head_.root, page);
	TreeNodePtr node = new TreeNode(&page, head_.attrtype, head_.capacity);

	int pos=node->nodeSearch(key);
	if (pos < 0) {
		//key值小于最小的
		curr_.setPage(head_.leaf);
		curr_.setSlot(0);
		return 1;
	}
	else {
		Rid* rid = node->getRid(0);
		TreeNodePtr next = getNode(rid->getPage());
		posSearch(key,next);
		delete(next);
	}
}

int IXFileScan::posSearch(keyPtr key, TreeNodePtr node) {

	if (node->IsLeaf()) {
		int pos = node->nodeSearch(key);
		Rid* rid = node->getRid(pos);
		curr_.setPage(rid->getPage());
		curr_.setSlot(rid->getSlot());

		delete(rid);
		return 1;
	}
	int pos = node->nodeSearch(key);
	Rid* rid = node->getRid(pos);
	TreeNodePtr next = getNode(rid->getPage());
	int feedback=posSearch(key,next);

	delete(next);
	return feedback;
}

TreeNode* IXFileScan::getNode(Page num) {

	PfPageHdl page = new PfPageHandle();
	file_->getPage(num, *page);
	return new TreeNode(page, attr_.type, head_.capacity);
}


void IXFileScan::getLeafNode() {

	Page leaf = head_.leaf;
	PfPageHdl page = new PfPageHandle();
	BtreeNodeheader nodehead;
	TreeNodePtr nodeptr;

	file_->getPage(leaf, *page);
	memcpy(&nodehead, page->getPageBuffer(), sizeof(BtreeNodeheader));
	nodeptr = new TreeNode(page, head_.attrtype, head_.capacity);

	while (nodeptr->getRightPage() != -1) {
		std::cout << endl << "BTree Leaf Node   " << nodeptr->getPage() << "    BTree Node KeyUsed " << nodeptr->keyused_ << endl;
		for (int slot = 0; slot < nodehead.keyused; slot++) {
			keyPtr key;
			switch (head_.attrtype)
			{
			case INT:
				key=malloc(sizeof(int)); memcpy(key, nodeptr->getKey(slot), sizeof(int)); 
				std::cout << *(int*)key << "\t"<<"("<<nodeptr->getRid(slot)->getPage()<<","<< nodeptr->getRid(slot)->getSlot()<<")\t"; break;
			case VARCAHR:
				key=malloc(nodeptr->keylen_); memcpy(key, nodeptr->getKey(slot), nodeptr->keylen_); 
				std::cout << (char*)key << "\t" << "(" <<setw(3)<< nodeptr->getRid(slot)->getPage() << "," << setw(3) << nodeptr->getRid(slot)->getSlot() << ") \t"; break;
			case CHAR:
				key=malloc(sizeof(char)); memcpy(key, nodeptr->getKey(slot), sizeof(char)); 
				std::cout << *(char*)key << "\t" << "(" << nodeptr->getRid(slot)->getPage() << "," << nodeptr->getRid(slot)->getSlot() << ")\t"; break;
			case FLOAT:
				key=malloc(sizeof(float)); memcpy(key, nodeptr->getKey(slot), sizeof(float)); 
				std::cout << *(float*)key << "\t" << "(" << nodeptr->getRid(slot)->getPage() << "," << nodeptr->getRid(slot)->getSlot() << ")\t"; break;
			}
			if (slot % 10 == 0) std::cout << endl;
		}

		Page next = nodeptr->getRightPage();
		delete(nodeptr);
		page = new PfPageHandle();
		file_->getPage(next, *page);
		memcpy(&nodehead, page->getPageBuffer(), sizeof(BtreeNodeheader));
		nodeptr = new TreeNode(page, head_.attrtype, head_.capacity);
	}
}

void IXFileScan::getInternalNode() {

	Page root = head_.root;
	PfPageHdl page = new PfPageHandle();
	BtreeNodeheader nodehead;
	TreeNodePtr nodeptr;

	file_->getPage(root, *page);
	memcpy(&nodehead, page->getPageBuffer(), sizeof(BtreeNodeheader));
	nodeptr = new TreeNode(page, head_.attrtype, head_.capacity);

	std::cout << "BTree Internal Node   " << nodeptr->getPage() << endl;
	for (int slot = 0; slot < nodehead.keyused; slot++) {
		keyPtr key;
		switch (nodehead.type)
		{
		case INT:
			key = malloc(sizeof(int)); memcpy(key, nodeptr->getKey(slot), sizeof(int)); std::cout << *(int*)key << endl; break;
		case VARCAHR:
			key = malloc(nodeptr->keylen_); memcpy(key, nodeptr->getKey(slot), nodeptr->keylen_); std::cout << (char*)key << endl; break;
		case CHAR:
			key = malloc(sizeof(char)); memcpy(key, nodeptr->getKey(slot), sizeof(char)); std::cout << *(char*)key << endl; break;
		case FLOAT:
			key = malloc(sizeof(float)); memcpy(key, nodeptr->getKey(slot), sizeof(float)); std::cout << *(float*)key << endl; break;
		}
	}
	delete(nodeptr);
}