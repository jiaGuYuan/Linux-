#include "client.h"
#include <ctype.h>

int main()
{
    int server_fifo_fd, client_fifo_fd;
    struct data_to_pass_st my_data;
    int read_res;
    char client_fifo[256];
    char *tmp_char_ptr;

    mkfifo(SERVER_FIFO_NAME, 0777);//创建管道，用作服务器
    server_fifo_fd = open(SERVER_FIFO_NAME, O_RDONLY);//以阻塞方式打开管道
    if (server_fifo_fd == -1) {
        fprintf(stderr, "Server fifo failure\n");
        exit(EXIT_FAILURE);
    }

    sleep(10); /* 睡眠，为了演示 */

    do {
        read_res = read(server_fifo_fd, &my_data, sizeof(my_data));
        if (read_res > 0) {


　　	/*在这下一个阶段,我们读取客户端的数据并进行一些处理。
		  我们将some_data中的所有字符转换为大写,将接收到的client_pid生成用于返回给客户端数据的管道的名字*/
            tmp_char_ptr = my_data.some_data;
            while (*tmp_char_ptr) {
                *tmp_char_ptr = toupper(*tmp_char_ptr);
                tmp_char_ptr++;
            }
            sprintf(client_fifo, CLIENT_FIFO_NAME, my_data.client_pid);

			/*然后我们将处理过的数据,通过以只写打开的客户端管道送回客户端.*/
            client_fifo_fd = open(client_fifo, O_WRONLY);
            if (client_fifo_fd != -1) {
                write(client_fifo_fd, &my_data, sizeof(my_data));
                close(client_fifo_fd);
            }
        }
    } while (read_res > 0);
    close(server_fifo_fd);
    unlink(SERVER_FIFO_NAME);//删除client到server的数据传递的管道。
    exit(EXIT_SUCCESS);
}

