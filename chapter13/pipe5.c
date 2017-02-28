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
            close(0);  //�رձ�׼�����ļ�������

			//��ʱͨ��dup()ʹ��׼�����ļ���������file_pipes[0]ָ��ͬһ���ļ�(�ܵ��ļ�)
			//�˺�ý����κζ���׼�����ļ��Ĳ������Ƕ�ȡfile_pipes[0]ָ����ļ�
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

  ����:����һ���µ��ļ�������,��������µ��ļ�����������Ϊ���Ĳ������Ǹ������ļ�������ָ��ͬһ���ļ�(��ܵ�)��
  dup()��������������ȡ��С����ֵ
  dup2() ����������������file_descriptor_two��ͬ,�����Ǵ���file_descriptor_one�Ŀ���ֵ
 */

