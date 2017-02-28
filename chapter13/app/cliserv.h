//这个文件定义了server与client的接口

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

/*
	定义命名管道，为服务器和每个客户端分别设置一个管道。
	因为可能会有多个客户，所以客户管道名由进程id动态生成
*/

#define SERVER_PIPE ".server_pipe"
#define CLIENT_PIPE ".client_%d_pipe"	//%d由进程id替换

#define ERR_TEXT_LEN 80

/*
 定义命令。
 client_request_e　给出了发送给服务器的请求类型
 server_response_e　给出了服务器返回给客户的响应类型
*/
typedef enum {
    s_create_new_database = 0,
    s_get_cdc_entry,
    s_get_cdt_entry,
    s_add_cdc_entry,
    s_add_cdt_entry,
    s_del_cdc_entry,
    s_del_cdt_entry,
    s_find_cdc_entry
} client_request_e;

typedef enum {
    r_success = 0,
    r_failure,
    r_find_no_more
} server_response_e;


/* 声明一个结构,用来在两个进程间进行双向的消息传递。
	因为我们实际上并不需要同时包含cdc_entry和cdt_entry,我们可以使用一个union。
	为简单起见,我们将它们区分开来。这也使代码更易于维护。 */
typedef struct {
    pid_t               client_pid;
    client_request_e    request;
    server_response_e   response;
    cdc_entry           cdc_entry_data;
    cdt_entry           cdt_entry_data;
    char                error_text[ERR_TEXT_LEN + 1];
} message_db_t;

/*  执行数据传输工作的管道接口函数 */
/* 服务端函数 */
int server_starting(void);
void server_ending(void);
int read_request_from_client(message_db_t *rec_ptr);
int start_resp_to_client(const message_db_t mess_to_send);
int send_resp_to_client(const message_db_t mess_to_send);
void end_resp_to_client(void);

/* 客户端函数 */
int client_starting(void);
void client_ending(void);
int send_mess_to_server(message_db_t mess_to_send);
int start_resp_from_server(void);
int read_resp_from_server(message_db_t *rec_ptr);
void end_resp_from_server(void);

