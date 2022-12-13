#include "IX_Handle.h"

IXHandle::IXHandle(PfFileHandle file,Attributes attr):file_(file),attr_(attr)
{

	PfPageHandle page;
	file_.getPage(num, page);
	root_ = new TreeNode(page, attr_.type, head_.capacity);

}

bool IXHandle::insertIndex(keyPtr rcd, Rid rid) {

	keyPtr key;
	key = malloc(head_.keylen);
	memcpy(key, rcd, head_.keylen);
	//键值的复制

	bool fix = true;
	insertIndex(root_, 1, key, rid, true);
	//从根结点递归插入
}

void IXHandle::insertIndex(TreeNodePtr node, int height, keyPtr key, Rid &rid,bool &fix) {
	
	int pos = -1;
	if (height == head_.height) {
		//到达叶子结点，在页内查找key
		int idx = node.search(key, rid);
		if (idx)return;//此索引已经存在
	}
	else {
		Rid next;
		pos = node.nodeSearch(key);
		if (pos < 0) {
			//找最小的结点指针，最左侧
			pos = 0;
			node.keyUpdate(key,0);
			next = node.getRid(0);
		}
		else {
			//找到中间结点的指针
			next = node.getRid(pos);
		}
		TreeNode nextNode = getNode(next.getPage());
		insertIndex(nextNode, height + 1, key, rid, fix);
	}
	if (node.keyused_ < head_.capacity) {
		//结点容量够，若为叶子结点在此结点插入
		if (height == head_.height) node.insertLeaf(key, rid);
		else node.insertInternal(pos + 1, key, rid);
	}
	else {
		//结点需要分裂
	}
}

TreeNode IXHandle::getNode(Page num) {

	PfPageHandle page;
	file_.getPage(num, page);
	return new TreeNode(page, attr_.type, head_.capacity);
}