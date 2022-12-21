#include <cstdio>
#include "PF/headfile/pf_operator.h"
#include "PF/headfile/pf_filehandle.h"
#include  "RM/rm_filehandle.h"
#include "PF/headfile/pf_buffer.h"
#include "PF/headfile/header.h"
#include "IX/IX_operator.h"
#include <iostream>
#include <cstdlib>

using namespace std;
PfBuffer* pfBuffer = PfBuffer::instance();
void fun(RmFileHdl &file) {

}
int main()
{
    printf("%s 向你问好!\n", "ConsoleApplication1");
    RmFileHdl file;
    CreateIndex("account", "account");
    
    return 0;
}

/*
    //实验1测试代码
    CreateFile("account");
    OpenFile("account", file);
    const char* str = "abcdefghijklmnopqrstuvwxyz";
    int account = 100000000;
    for (int i = 0; i < 5000; i++) {
        char name[6], rcd[20];
        for (int j = 0; j < 6; j++) {
            int pos = rand() % 26;
            name[j] = str[pos];
        }
        memcpy(rcd, &account, sizeof(int));
        memcpy(rcd+sizeof(int), name, strlen(name));

        file->insertRcord(rcd);
        account += rand() % 10000;
    }

    file->catFile(2);
    file->catFile(25);
    CloseFile(file);
    
*/