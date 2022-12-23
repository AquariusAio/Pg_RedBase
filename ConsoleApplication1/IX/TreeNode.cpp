#include "TreeNode.h"
#include<iostream>

TreeNode::TreeNode(PfPageHdl pagehdl, AttrType type,int capacity):
	keyType(type)
{
	this->page_ = pagehdl;
	this->keyType = type;
	this->comp = IXCompFactory::generateComp(type);

	PageBuffer ptr=page_->getPageBuffer();
	AttrType nodetype;


	memcpy(&nodetype, ptr, sizeof(AttrType));
	ptr = ptr + sizeof(unsigned);
	memcpy(&(this->left), ptr, sizeof(Page));
	ptr = ptr + sizeof(Page);
	memcpy(&(this->right), ptr, sizeof(Page));
	ptr = ptr + sizeof(Page);
	memcpy(&(this->keyused_), ptr, sizeof(int));
	ptr = ptr + sizeof(int);
	memcpy(&(this->keylen_), ptr, sizeof(int));
	ptr = ptr + sizeof(int);
	keys_ = ptr;
	rids_ = (Rid*)(ptr + keylen_ * capacity);

	switch (nodetype)
	{
	case INT:break;
	case VARCAHR:break;
	case CHAR:break;
	case FLOAT:break;
	default:printf("ҳ���ʹ���");
		break;
	}
}

TreeNode::~TreeNode() {

	PageBuffer ptr = page_->getPageBuffer();
	memcpy(ptr, &keyType, sizeof(AttrType));
	ptr = ptr + sizeof(unsigned);
	memcpy( ptr, &(this->left), sizeof(Page));
	ptr = ptr + sizeof(Page);
	memcpy( ptr, &(this->right), sizeof(Page));
	ptr = ptr + sizeof(Page);
	memcpy( ptr, &(this->keyused_), sizeof(int));
	if(page_!=nullptr) delete(page_);

}

Rid* TreeNode::getRid(int pos) {

	if (pos > this->keyused_) printf("RID������Χ");
	return rids_+pos;
}

keyPtr TreeNode::getKey(int pos) {

	if (pos > this->keyused_) printf("KEY������Χ");
	return keys_+pos*keylen_;
}

int TreeNode::search(void* key, Rid& rid) {

	int idx = nodeSearch(key);
	if (idx < 0) return 0;
	for (int i = idx; i < keyused_; i++) {
		if (Comp(getKey(idx), key) != 0) return 0;//��keyֵ�����ʱ��ֹͣƥ��
		if (rids_[i].ridEqual(rid)) return i;//rid��ȣ��ҵ���Ӧλ��
	}
	return 0;
}

int TreeNode::nodeSearch(void* key) {
	int low = 0, high = keyused_-1;
	int mid = (high + low) / 2;
	while (high >= low) {
		mid = (high + low) / 2;
		switch (Comp(getKey(mid), key))
		{
		case 0:high = mid - 1; break;//�ҵ���Ӧ�ļ�ֵ
		case 1:high = mid - 1; break;
		case -1:low = mid + 1; break;
		}
	}

	if (low == 0) return Comp(getKey(0), key) > 0 ? -1 : 0;//С�����е�keyֵ
	else if (low >= keyused_) return keyused_-1;//�������еļ�ֵ
	return low - 1;
}

bool TreeNode::insertLeaf(void* key, Rid rid) {
	
	int i = -1;
	keyPtr prev = nullptr, curr = nullptr;
	// �Ӻ���ǰɨ��
	for (i = keyused_-1; i >= 0; i--)
	{
		prev = curr;
		curr = getKey(i);
		if (Comp(key, curr) > 0) break;
		//rids_[i + 1] = rids_[i];
		memcpy(getRid(i + 1), getRid(i), sizeof(Rid));
		memcpy(getKey(i + 1), getKey(i), keylen_);
	}
	//rids_[i + 1] = rid;
	memcpy(getRid(i + 1), &rid, sizeof(Rid));
	keyUpdate(key, i + 1);
	//std::cout << "insertinto " << this->page_->getPage() << " "<<this->keyused_<<endl;
	keyused_++;
	page_->setDirty();
	return true;
}

bool TreeNode::insertInternal(void* key, Rid rid, int pos) {

	if ((pos < 0) || (pos >keyused_)) return false;
	int count = keyused_ - pos;
	std::cout << "shuchu" << endl;
	for (int i = 0; i < keyused_; i++) std::cout << rids_[i].getPage() << rids_[i].getSlot() << endl;
	memmove(getKey(pos+1), getKey(pos), count * keylen_);
	memmove(getRid(pos+1), getRid(pos), count * sizeof(Rid));
	memcpy(getKey(pos), key, keylen_);
	ridUpdate(rid,pos);
	std::cout << "charujieshu" << endl;
	for (int i = 0; i < keyused_+1; i++) std::cout << rids_[i].getPage() << rids_[i].getSlot() << endl;
	keyused_++;
	page_->setDirty();
	return true;


}

bool TreeNode::keyUpdate(void* key, int pos) {

	if (keyType == INT) {
		memcpy(getKey(pos), key, sizeof(int));
	}
	else if (keyType == VARCAHR || keyType == CHAR) {
		memcpy(getKey(pos), key, strlen((char*)key));
	}
	else if (keyType == FLOAT) {
		memcpy(getKey(pos), key, sizeof(float));
	}
	return true;
}

bool TreeNode::ridUpdate(Rid rid, int pos) {

	memcpy(&rids_[pos], &rid, sizeof(Rid));
	return true;
}