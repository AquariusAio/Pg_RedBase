#include "IX_Handle.h"

IXHandle::IXHandle(PfFileHandle file,Attributes attr):file_(file),attr_(attr)
{
	
}

bool IXHandle::insertIndex(keyPtr rcd, Rid rid) {

	keyPtr key;
	key = malloc(head_.keylen);
	memcpy(key, rcd, head_.keylen);

	insertIndex(root_, 1, key, rid);
}

void IXHandle::insertIndex(TreeNodePtr node, int height, keyPtr key, Rid rid) {
	
	if (height == head_.height) {

	}
	else {
		Rid next;
		int pos = node.nodeSearch(key);
		if (pos < 0) {
			//找最小的结点执政，最左侧
		}
		else {
			next = ;
		}
	}
}