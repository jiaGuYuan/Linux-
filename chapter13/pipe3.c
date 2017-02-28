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

        if (fork_result == 0) {//�ӽ���
            sprintf(buffer, "%d", file_pipes[0]);

            //�������Ǵ��ݵĲ����ֱ��ʾ(Ҫִ�г����·��, argv[0]ִ�г������, argv[1]�򿪵Ĺܵ��ļ��Ķ�������, NULL)
            (void)execl("pipe4", "pipe4", buffer, (char *)0);
			//��Ȼ:�������洫�ݵ�argv[0]�������г����������,ֻ��Ϊ����main()�Ĳ������Ʋ���ô����.
			//��ȫ����(void)execl("pipe4", buffer, (char *)0),Ȼ����pipe4��������argv[0]����ȡ���ݵ�buffer
			
            exit(EXIT_FAILURE);
        } else { //������
            data_processed = write(file_pipes[1], some_data,
                                   strlen(some_data));
            printf("%d - wrote %d bytes\n", getpid(), data_processed);
        }
    }
    exit(EXIT_SUCCESS);
}

/*
	int execl(const char * path, const char * arg, ...);
	����˵��������path��ʾҪִ�г����·��, 
	�������Ĳ�������ִ�иó���ʱ���ݹ�ȥ��argv(0), argv[1], ..., ���һ�����������ÿ�ָ��(NULL)������.
 */

