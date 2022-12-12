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

	insertIndex(root_, 1, key, rid);
	//从根结点递归插入
}

void IXHandle::insertIndex(TreeNodePtr node, int height, keyPtr key, Rid &rid) {
	
	if (height == head_.height) {
		//到达叶子结点，在页内查找key
	}
	else {
		Rid next;
		int pos = node.nodeSearch(key);
		if (pos < 0) {
			//找最小的结点执政，最左侧
		}
		else {
			//找到中间结点的指针
		}
		TreeNode nextNode = getNode(next.getPage());
		insertIndex(nextNode, height + 1, key, rid);
	}
	if (node.keyused_ < head_.capacity) {
		//结点容量够，在此结点插入
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