#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    FILE *read_fp;
    char buffer[BUFSIZ + 1]; //BUFSIZ在stdio.h中定义
    int chars_read;
    memset(buffer, '\0', sizeof(buffer));

	//当前进程通过popen启动另一个进程运行uname程序,'r'表示当前进程可以读取被运行的程序的输出(读取管道)
	//实际上是创建了一个管道,r:表示将当前进程置于读端,w:表示将当前进程置于写端
	//当前进程与它启动的进程间的调度，linux会自动完成
    read_fp = popen("uname -a", "r");
    if (read_fp != NULL) {
		//通过read_fp读取由popen所运行的程序的输出
        chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
        if (chars_read > 0) {
            printf("Output was:-\n%s\n", buffer);
        }
        pclose(read_fp);
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_FAILURE);
}


//通过popen()来启动进程时，它会首先启动shell

