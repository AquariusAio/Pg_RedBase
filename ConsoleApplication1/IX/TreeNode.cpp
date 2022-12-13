#include "TreeNode.h"


TreeNode::TrreNode(PfPageHdl pagehdl, AttrType type,int capacity):
	keyType(type)
{
	this->page = pagehdl;
	this->keyType = type;
	this->comp = IXCompFactory::generateComp(type);

	PageBuffer ptr=page->getPageBuffer();
	unsigned nodetype;


	memcpy(&nodetype, ptr, sizeof(unsigned));
	ptr = ptr + sizeof(unsigned);
	memcpy(this->left, ptr, sizeof(Page));
	ptr = ptr + sizeof(Page);
	memcpy(this->right, ptr, sizeof(Page));
	ptr = ptr + sizeof(Page);
	memcpy(this->keyused_, ptr, sizeof(int));
	ptr = ptr + sizeof(int);
	memcpy(this->keylen_, ptr, sizeof(int));
	ptr = ptr + sizeof(int);
	keys_ = ptr;
	rids_ = (Rid)(ptr + keylen_ * capacity);

	switch (nodetype)
	{
	case IS_ROOT:isRoot = true; break;
	case IS_LEAF:isLeaf = true; break;
	default:printf("页类型错误"); break;
	}
}

Rid TreeNode::getRid(int pos) {

	if (pos > this->keyused_) printf("RID超出范围");
	return rids_[pos];
}

keyPtr TreeNode::getKey(int pos) {

	if (pos > this->keyused_) printf("KEY超出范围");
	return keys_+pos*keylen_;
}

int TreeNode::search(void* key, Rid& rid) {

	int idx = nodeSearch(key);
	if (idx < 0) return 0;
	for (int i = idx; i < keyused_; i++) {
		if (comp(keys_[i], key) != 0) return 0;//当key值不相等时，停止匹配
		if (rids_[i].ridEqual(rid)) return i;//rid相等，找到对应位置
	}
	return 0;
}

int TreeNode::nodeSearch(void* key) {
	int low = 0, high = keyused_;
	while (high >= low) {
		int mid = (high + low) / 2 ;
		switch (comp(keys_[mid], key)) 
		{
		case 0:return mid; break;//找到对应的键值
		case 1:high = mid; break;
		case -1:low = mid; break;
		}
	}

	if (low == 0) return -1;//小于所有的key值
	else if (high == keyused_) return keyused_ + 1;//大于所有的键值
	return 0;
}

bool TreeNode::insertLeaf(void* key, Rid rid) {
	
	int i = -1;
	keyPtr prev = nullptr, curr = nullptr;
	// 从后往前扫描
	for (i = keyused_; i >= 0; i--)
	{
		prev = curr;
		curr = getKey(i);
		if (comp(key, curr) > 0) break;
		rids_[i + 1] = rids_[i];
		keyUpdate(i + 1, curr);
	}
	rids_[i + 1] = rid;
	keyUpdate(key, i + 1);

	keyused_++;
	page_.setDirty();
	return true;
}

bool TreeNode::insertInternal(void* key, Rid rid, int pos) {

	if ((pos < 0) || (pos > size_)) return false;
	int count = keyused_ - pos;
	memmove(getKey(pos+1), getKey(pos), count * keylen_);
	memmove(&rids_[idx + 1], &rids_[idx], count * sizeof(RID));
	memcpy(getKey(idx), key, keylen_);
	ridUpdate(idx, rid);

	keyused_++;
	page.setDirty();
	return true;


}

bool TreeNode::keyUpdate(void* key, int pos) {

	if (keyType == INT) {
		memcpy(keys_[pos], key; sizeof(int));
	}
	else if (keyType == VARCAHR || keyType == CHAR) {
		memcpy(keys_[pos], key, strlen(key));
	}
	else if (keyType == FLOAT) {
		memcpy(keys_[pos], key, sizeof(float));
	}
	keyused_ += 1;
	return true;
}

bool TreeNode::ridUpdate(Rid rid, int pos) {

	memcpy(rids_ + pos * sizeof(Rid), &rid, sizeof(Rid));
	return true;
}