#include <cstdio>
#include "PF/headfile/pf_operator.h"
#include "PF/headfile/pf_filehandle.h"
#include  "RM/rm_filehandle.h"
#include "PF/headfile/pf_buffer.h"
#include "PF/headfile/header.h"
PfBuffer* pfBuffer = PfBuffer::instance();

int main()
{
    printf("%s 向你问好!\n", "ConsoleApplication1");
    RmFileHdl file;


    OpenFile("test", file);
    file->catFile(1);


    return 0;
}