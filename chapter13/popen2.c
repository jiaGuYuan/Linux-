#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    FILE *write_fp;
    char buffer[BUFSIZ + 1];

    sprintf(buffer, "Once upon a time, there was...\n");
	
	//当前进程通过popen启动另一个进程运行od程序,'w'表示可以将当前进程的输出送到被运行的程序(写入管道)
    write_fp = popen("od -c", "w");
    if (write_fp != NULL) {
        fwrite(buffer, sizeof(char), strlen(buffer), write_fp);
        pclose(write_fp);
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_FAILURE);
}

//效果与 od -c file.txt相同。  其中file.txt的内容为Once upon a time, there was...