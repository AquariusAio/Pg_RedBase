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
	PfFileHdl file_;
	TreeNodePtr root_;
	Attributes attr_;
public:
	IXHandle(PfFileHdl file,Attributes attr);
	bool createIndex(Attributes);
	bool insertIndex(keyPtr,Rid rid);
private:
	bool insertIndex(TreeNodePtr, int, keyPtr, Rid &, bool&);
	TreeNode* getNode(Page page);
	TreeNode* newTreeNode();
	void NodeDevide(TreeNodePtr,TreeNodePtr);
};
