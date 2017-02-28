#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    int data_processed;
    int file_pipes[2];
    const char some_data[] = "123";
    pid_t fork_result;

    if (pipe(file_pipes) == 0) {
        fork_result = fork();
        if (fork_result == (pid_t)-1) {
            fprintf(stderr, "Fork failure");
            exit(EXIT_FAILURE);
        }

        if (fork_result == (pid_t)0) {
            close(0);  //关闭标准输入文件描述符

			//此时通过dup()使标准输入文件描述符与file_pipes[0]指向同一个文件(管道文件)
			//此后该进程任何读标准输入文件的操作都是读取file_pipes[0]指向的文件
            dup(file_pipes[0]); 
            close(file_pipes[0]);
            close(file_pipes[1]);

            execlp("od", "od", "-c", (char *)0);
            exit(EXIT_FAILURE);
        }
        else {
            close(file_pipes[0]);
            data_processed = write(file_pipes[1], some_data,
                                   strlen(some_data));
            close(file_pipes[1]);
            printf("%d - wrote %d bytes\n", (int)getpid(), data_processed);
        }
    }
    exit(EXIT_SUCCESS);
}

/*
  int dup(int file_descriptor)
  int dup2(int file_descriptor_one, int file_descriptor_two)

  功能:创建一个新的文件描述符,并且这个新的文件描述符与作为它的参数的那个已有文件描述符指向同一个文件(或管道)。
  dup()创建的新描述符取最小可用值
  dup2() 创建的新描述符与file_descriptor_two相同,或者是大于file_descriptor_one的可能值
 */

