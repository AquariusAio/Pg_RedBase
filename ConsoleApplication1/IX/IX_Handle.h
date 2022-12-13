#pragma once
#include "../PF/headfile/pf_filehandle.h"
#include "../PF/headfile/data_dictionary.h"
#include"RcdRid.h"
#include"TreeNode.h"
#include "IX_Header.h"


#define NODE_CAPACITY 200;
class IXHandle{

public:
	BtreeFileHeader head_;
	PfFileHandle file_;
	TreeNodePtr root_;
	Attributes attr_;
public:
	IXHandle(PfFileHandle file,Attributes attr);
	bool insertIndex(keyPtr,Rid rid);
private:
	void insertIndex(TreeNodePtr, int, keyPtr, Rid &, bool&);
	TreeNode getNode(Page page);
};
