#pragma once
#pragma once
#include<memory>
#include"pf_error.h"

#define PF_PAGE_SIZE  4096
#define PF_MEM_BLOCKS 30
#define PF_LINP_NUM 210


typedef int Page;
typedef char* PageBuffer;
typedef int PageSlot;
typedef unsigned int uint;
typedef int Page;
typedef int RC;

class DBConfig {
public:
	int 
		memorySpace:81920,   //内存大小
	    diskSpace:81920,      //硬盘大小
	    pageSize : 4096;
};



