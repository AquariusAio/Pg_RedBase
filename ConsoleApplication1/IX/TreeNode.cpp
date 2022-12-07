#include "TreeNode.h"

TreeNode::TrreNode(PfPageHdl pagehdl, AttrType type,int capacity):
	keyType(type)
{
	this->page = pagehdl;
	PageBuffer ptr=page->getPageBuffer();
	unsigned nodetype;


	memcpy(&nodetype, ptr, sizeof(unsigned));
	ptr = ptr + sizeof(unsigned);
	memcpy(this->left, ptr, sizeof(Page));
	ptr = ptr + sizeof(Page);
	memcpy(this->right, ptr, sizeof(Page));
	ptr = ptr + sizeof(Page);
	memcpy(this->keyused, ptr, sizeof(int));

	keys_ = ptr;
	rids_ = (Rid)(ptr + RID_LEN * capacity);

	switch (nodetype)
	{
	case IS_ROOT:isRoot = true; break;
	case IS_LEAF:isLeaf = true; break;
	default:printf("Ò³ÀàÐÍ´íÎó"); break;
	}
}

Rid TreeNode::getRid(int pos) {

	if (pos > this->keyused) printf("RID³¬³ö·¶Î§");
	return rids_[pos];
}

int TreeNode::nodeSearch(void* key) {
	int low = 0, high = keyused;
	while (high >= low) {
		int mid = (high + low) / 2 ;
		if (keys_[mid] == key) return mid;
		else if (keys_[mid] > key) high = mid;
		else low = mid;
	}

	if (low == 0) return -1;
	else if (high == keyused) return keyused + 1;
}

bool TreeNode::nodeInsert(void* key, int pos) {
	if (keyType == INT) {
		memcpy(keys_[pos], key; sizeof(int));
	}
	else if (keyType == VARCAHR) {
		memcpy(keys_[pos], key, strlen(key));
	}
	else if (keyType == FLOAT) {
		memcpy(keys_[pos], key, sizeof(float));
	}
	keyused += 1;
	return true;
}


