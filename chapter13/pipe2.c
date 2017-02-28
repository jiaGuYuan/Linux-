#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    int data_processed;
    int file_pipes[2];
    const char some_data[] = "123";
    char buffer[BUFSIZ + 1];
    pid_t fork_result;

    memset(buffer, '\0', sizeof(buffer));

	//创建管道--它生成两个文件描述符,file_pipes[0]对应读,file_pipes[1]对应写
	//注意:这里的管道是无名管道,它只能在父子或兄弟进程之间进行数据通信,
	//并且只有无名管道的创建在fork出子进程之前时才能进行通信，因为这时打开的进程会共享同一个文件描述符
    if (pipe(file_pipes) == 0) {
        fork_result = fork();	//创建进程
        if (fork_result == -1) {
            fprintf(stderr, "Fork failure");
            exit(EXIT_FAILURE);
        }

        if (fork_result == 0) { //子进程从管道接收数据
            data_processed = read(file_pipes[0], buffer, BUFSIZ);
            printf("Read %d bytes: %s\n", data_processed, buffer);
            exit(EXIT_SUCCESS);
        }
        else { //父进程向管道写数据
            data_processed = write(file_pipes[1], some_data,
                                   strlen(some_data));
            printf("Wrote %d bytes\n", data_processed);
        }
    }
    exit(EXIT_SUCCESS);
}

