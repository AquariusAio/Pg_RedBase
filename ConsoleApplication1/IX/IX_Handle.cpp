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
	//��ֵ�ĸ���

	bool fix = true;
	insertIndex(root_, 1, key, rid, true);
	//�Ӹ����ݹ����
}

void IXHandle::insertIndex(TreeNodePtr node, int height, keyPtr key, Rid &rid,bool &fix) {
	
	int pos = -1;
	if (height == head_.height) {
		//����Ҷ�ӽ�㣬��ҳ�ڲ���key
		int idx = node.search(key, rid);
		if (idx)return;//�������Ѿ�����
	}
	else {
		Rid next;
		pos = node.nodeSearch(key);
		if (pos < 0) {
			//����С�Ľ��ָ�룬�����
			pos = 0;
			node.keyUpdate(key,0);
			next = node.getRid(0);
		}
		else {
			//�ҵ��м����ָ��
			next = node.getRid(pos);
		}
		TreeNode nextNode = getNode(next.getPage());
		insertIndex(nextNode, height + 1, key, rid, fix);
	}
	if (node.keyused_ < head_.capacity) {
		//�������������ΪҶ�ӽ���ڴ˽�����
		if (height == head_.height) node.insertLeaf(key, rid);
		else node.insertInternal(pos + 1, key, rid);
	}
	else {
		//�����Ҫ����
	}
}

TreeNode IXHandle::getNode(Page num) {

	PfPageHandle page;
	file_.getPage(num, page);
	return new TreeNode(page, attr_.type, head_.capacity);
}