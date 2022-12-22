#include"IX_filescan.h"
#include<iostream>

void IXFileScan::openScan(PfFileHdl file, Attributes attr) :file_(file), attr_(attr){

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
		rid.setRid(Rid(curr_.getPage(), curr_.getSlot()));
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
		curr_.setPage(Rid(head_.leaf);
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

	if (node->isLeaf()) {
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
	int feedback=posSearch(key,next, rmrid);

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
	PfPageHandle page;
	BtreeNodeheader nodehead;
	TreeNodePtr nodeptr;

	file_->getPage(leaf, page);
	memcpy(&nodehead, page.getPageBuffer(), sizeof(BtreeNodeheader));
	nodeptr = new TreeNode(&page, nodehead.type, head_.capacity);

	while (nodeptr->getRightPage() != -1) {

		for (int slot = 0; slot < nodehead.keyused; slot++) {
			switch (nodehead.type)
			{
			case INT:
				int key; memcpy(&key, nodeptr->getKey(slot), sizeof(int)); std::cout << key << endl; break;
			case VARCAHR:
				char key[nodeptr->keylen_]; memcpy(key, nodeptr->getKey(slot), sizeof(nodeptr->keylen_)); std::cout << key << emdl; break;
			case CHAR:
				char key; memcpy(&key, nodeptr->getKey(slot), sizeof(char)); std::cout << key << endl; break;
			case FLOAT:
				float key; memcpy(&key, nodeptr->getKey(slot), sizeof(float)); std::cout << key << endl; break;
			}
		}

		file_->getPage(nodeptr->getRightPage(), page);
		memcpy(&nodehead, page.getPageBuffer(), sizeof(BtreeNodeheader));
		delete(nodeptr);
		nodeptr = new TreeNode(&page, nodehead.type, head_.capacity);
	}
}