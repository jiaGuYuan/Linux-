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

    if (pipe(file_pipes) == 0) {
        fork_result = fork();
        if (fork_result == (pid_t)-1) {
            fprintf(stderr, "Fork failure");
            exit(EXIT_FAILURE);
        }

        if (fork_result == 0) {//子进程
            sprintf(buffer, "%d", file_pipes[0]);

            //这里我们传递的参数分别表示(要执行程序的路径, argv[0]执行程序的名, argv[1]打开的管道文件的读描述符, NULL)
            (void)execl("pipe4", "pipe4", buffer, (char *)0);
			//当然:我们上面传递的argv[0]不是运行程序所必须的,只是为了与main()的参数相似才这么做的.
			//完全可以(void)execl("pipe4", buffer, (char *)0),然后在pipe4程序中用argv[0]来获取传递的buffer
			
            exit(EXIT_FAILURE);
        } else { //父进程
            data_processed = write(file_pipes[1], some_data,
                                   strlen(some_data));
            printf("%d - wrote %d bytes\n", getpid(), data_processed);
        }
    }
    exit(EXIT_SUCCESS);
}

/*
	int execl(const char * path, const char * arg, ...);
	函数说明：参数path表示要执行程序的路径, 
	接下来的参数代表执行该程序时传递过去的argv(0), argv[1], ..., 最后一个参数必须用空指针(NULL)作结束.
 */

