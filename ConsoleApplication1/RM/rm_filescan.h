#pragma once
#include"../IX/RcdRid.h"
#include"rm_filehandle.h"
#include "rm_pagehandle.h"
#include"rm_rcdrid.h"

class RmFileScan {
public:
	RmFileHdl file_;
	Rid curr_;
	int offset_;
public:
	RmFileScan(RmFileHdl file,int offset) :file_(file), offset_(offset),curr_(1, -1) {};
	int getNextRcd(RmRid&);
};

#define SCAN_EOF 0;
