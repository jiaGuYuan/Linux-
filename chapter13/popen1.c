#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    FILE *read_fp;
    char buffer[BUFSIZ + 1]; //BUFSIZ��stdio.h�ж���
    int chars_read;
    memset(buffer, '\0', sizeof(buffer));

	//��ǰ����ͨ��popen������һ����������uname����,'r'��ʾ��ǰ���̿��Զ�ȡ�����еĳ�������(��ȡ�ܵ�)
	//ʵ�����Ǵ�����һ���ܵ�,r:��ʾ����ǰ�������ڶ���,w:��ʾ����ǰ��������д��
	//��ǰ�������������Ľ��̼�ĵ��ȣ�linux���Զ����
    read_fp = popen("uname -a", "r");
    if (read_fp != NULL) {
		//ͨ��read_fp��ȡ��popen�����еĳ�������
        chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
        if (chars_read > 0) {
            printf("Output was:-\n%s\n", buffer);
        }
        pclose(read_fp);
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_FAILURE);
}


//ͨ��popen()����������ʱ��������������shell

