#include"../RM/rm_rcdrid.h"
#include"../RM/rm_filehandle.h"
#include"../PF/headfile/data_dictionary.h"
#include"../PF/headfile/pf_pagehandle.h"
#include"../PF/headfile/header.h"
#include"IX_Header.h"
#include"TreeNode.h"

class IXFileScan
{
	PfFileHdl file_;
	Rid curr_;
	Attributes attr_;
	BtreeFileHeader head_;
public:
	IXFileScan() : curr_(1, -1) {}
	~IXFileScan() {}
public:
	void openScan(PfFileHdl file, Attributes attr);
	int getNextIndex(keyPtr,RmRid&);
	void getLeafNode();
private:
	int getNextIndex(keyPtr);
	int posSearch(keyPtr, TreeNodePtr);
	TreeNode* getNode(Page num)
};

