#pragma once
#include "pf_filehandle.h"
#include "pf_filehandle.h"
#include "../../RM/rm_filehandle.h"
#include "util.h"
#include"header.h"
#include"../../RM/rm_filehandle.h"
#include"header.h"

#define _O_RDONLY      0x0000  // open for reading only
#define _O_WRONLY      0x0001  // open for writing only
#define _O_RDWR        0x0002  // open for reading and writing 
#define _O_CREAT       0x0100  // create and open file
#define _O_EXCL        0x0400  // open only if file doesn't already exist
#define CREATION_MASK		0600		// r/w privileges to owner only

void DbIni(DBConfig);
void PageIni(const char*, int);
int OpenFile(const char * pathfile,RmFileHdl& file);//打开一个文件
int CreateFile(const char*);//创建一个表
void CloseFile(RmFileHdl& rmfile);