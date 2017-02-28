#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

/*client��server�����ݴ���ͨ��SERVER_FIFO_NAME�ܵ���ɡ�
  server��client�����ݴ���ͨ��CLIENT_FIFO_NAME�ܵ���ɡ�
  CLIENT_FIFO_NAME�ܵ�����data_to_pass_st��client_pid�ֶζ�̬���ɡ�*/
#define SERVER_FIFO_NAME "./serv_fifo"
#define CLIENT_FIFO_NAME "./cli_%d_fifo"

#define BUFFER_SIZE 20

struct data_to_pass_st {
    pid_t  client_pid;
    char   some_data[BUFFER_SIZE - 1];
};
