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

	insertIndex(root_, 1, key, rid);
	//�Ӹ����ݹ����
}

void IXHandle::insertIndex(TreeNodePtr node, int height, keyPtr key, Rid &rid) {
	
	if (height == head_.height) {
		//����Ҷ�ӽ�㣬��ҳ�ڲ���key
	}
	else {
		Rid next;
		int pos = node.nodeSearch(key);
		if (pos < 0) {
			//����С�Ľ��ִ���������
		}
		else {
			//�ҵ��м����ָ��
		}
		TreeNode nextNode = getNode(next.getPage());
		insertIndex(nextNode, height + 1, key, rid);
	}
	if (node.keyused_ < head_.capacity) {
		//������������ڴ˽�����
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