#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

/*client到server的数据传递通过SERVER_FIFO_NAME管道完成。
  server到client的数据传递通过CLIENT_FIFO_NAME管道完成。
  CLIENT_FIFO_NAME管道名由data_to_pass_st的client_pid字段动态生成　*/
#define SERVER_FIFO_NAME "./serv_fifo"
#define CLIENT_FIFO_NAME "./cli_%d_fifo"

#define BUFFER_SIZE 20

struct data_to_pass_st {
    pid_t  client_pid;
    char   some_data[BUFFER_SIZE - 1];
};
