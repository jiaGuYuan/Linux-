//客户和服务器的接口函数
/* Include files */

#include "cd_data.h"
#include "cliserv.h"

/* define some values that we need in different functions within the file. */

static int server_fd = -1;
static pid_t mypid = 0;
static char client_pipe_name[PATH_MAX + 1] = {'\0'};
static int client_fd = -1;
static int client_write_fd = -1;


/*************服务端函数******************/
/*
The server_starting routine creates the named pipe from which the server will read commands.
It then opens it for reading. This open will block until a client opens the pipe for writing.
We use a blocking mode, so that the server can perform blocking reads on the pipe while waiting for commands to be sent to it.
*/

//创建管道SERVER_PIPE，服务器从中读数据
int server_starting(void)
{
    #if DEBUG_TRACE    
        printf("%d :- server_starting()\n",  getpid());
    #endif

    	unlink(SERVER_PIPE);
    if (mkfifo(SERVER_PIPE, 0777) == -1) {
        fprintf(stderr, "Server startup error, no FIFO created\n");
        return(0);
    }

	//服务端从SERVER_PIPE管道读数据
    if ((server_fd = open(SERVER_PIPE, O_RDONLY)) == -1) {
        fprintf(stderr, "Server startup error, no FIFO opened\n");
        return(0);
    }
    return(1);
}

/* 当服务器结束,它删除了命名管道,所以客户可以检测到没有服务器正在运行. */
void server_ending(void)
{
    #if DEBUG_TRACE    
        printf("%d :- server_ending()\n",  getpid());
    #endif

    (void)close(server_fd);
    (void)unlink(SERVER_PIPE);
}

/* 阻塞读取客户端写入服务器管道的一个消息. 
   成功读取:返回1
 */
int read_request_from_client(message_db_t *rec_ptr)
{
    int return_code = 0;
    int read_bytes;

    #if DEBUG_TRACE    
        printf("%d :- read_request_from_client()\n",  getpid());
    #endif

    if (server_fd != -1) {
        read_bytes = read(server_fd, rec_ptr, sizeof(*rec_ptr)); 

	/*　在特殊情况下,当没有客户端为写打开管道,read将返回0,即它检测到一个EOF。
        然后服务器关闭管道并重新打开了它,直到客户端打开管道。
        这与服务器第一次启动时的情况完全一样　*/
        if (read_bytes == 0) {
            (void)close(server_fd);
            if ((server_fd = open(SERVER_PIPE, O_RDONLY)) == -1) {
               fprintf(stderr, "Server error, FIFO open failed\n");
               return(0);
            }
            read_bytes = read(server_fd, rec_ptr, sizeof(*rec_ptr));
        }
        if (read_bytes == sizeof(*rec_ptr)) return_code = 1;
    }
    return(return_code);
}

/* open the client pipe. */
int start_resp_to_client(const message_db_t mess_to_send)
{
    #if DEBUG_TRACE
        printf("%d :- start_resp_to_client()\n", getpid());
    #endif

    (void)sprintf(client_pipe_name, CLIENT_PIPE, mess_to_send.client_pid);
    if ((client_fd = open(client_pipe_name, O_WRONLY)) == -1) return(0);
    return(1);
}

/*
The messages are all sent using calls to this function.
We'll look at the corresponding client-side functions that field the message soon.
*/
int send_resp_to_client(const message_db_t mess_to_send)
{
    int write_bytes;

    #if DEBUG_TRACE
        printf("%d :- send_resp_to_client()\n", getpid());
    #endif

    if (client_fd == -1) return(0);
    write_bytes = write(client_fd, &mess_to_send, sizeof(mess_to_send));
    if (write_bytes != sizeof(mess_to_send)) return(0);
    return(1);
}

/* close the client pipe. */

void end_resp_to_client(void)
{
    #if DEBUG_TRACE
        printf("%d :- end_resp_to_client()\n",  getpid());
    #endif

    if (client_fd != -1) {
        (void)close(client_fd);
        client_fd = -1;
    }
}

/* 检查服务器访问后,client_starting函数初始化客户端管道. 
   以只写的方式打开了由client-->server传输的管道
   并创建了server-->client传输的管道
 */
int client_starting(void)
{
    #if DEBUG_TRACE
        printf("%d :- client_starting\n",  getpid());
    #endif

    mypid = getpid();
    if ((server_fd = open(SERVER_PIPE, O_WRONLY)) == -1) {
        fprintf(stderr, "Server not running\n");
        return(0);
    }

    (void)sprintf(client_pipe_name, CLIENT_PIPE, mypid);
    (void)unlink(client_pipe_name);
    if (mkfifo(client_pipe_name, 0777) == -1) {
        fprintf(stderr, "Unable to create client pipe %s\n", 
                   client_pipe_name);
        return(0);
    }
    return(1);
}

/* The client_ending function closes file descriptors and deletes the now redundant named pipe. */

void client_ending(void)
{
    #if DEBUG_TRACE    
        printf("%d :- client_ending()\n",  getpid()); 
    #endif

    if (client_write_fd != -1) (void)close(client_write_fd);
    if (client_fd != -1) (void)close(client_fd);
    if (server_fd != -1) (void)close(server_fd);
    (void)unlink(client_pipe_name);
}

/* 函数通过管道向服务器传递请求. */
int send_mess_to_server(message_db_t mess_to_send)
{
    int write_bytes;

    #if DEBUG_TRACE    
        printf("%d :- send_mess_to_server()\n",  getpid());
    #endif

    if (server_fd == -1) return(0);
    mess_to_send.client_pid = mypid;
    write_bytes = write(server_fd, &mess_to_send, sizeof(mess_to_send));
    if (write_bytes != sizeof(mess_to_send)) return(0);
    return(1);
}

/* This client function starts to listen for the server response.
It opens a client pipe as read-only and then reopens this pipe's file as write-only.
We'll see why in a moment.
*/

int start_resp_from_server(void)
{
    #if DEBUG_TRACE    
        printf("%d :- start_resp_from_server()\n",  getpid());    
    #endif

    if (client_pipe_name[0] == '\0') return(0);
    if (client_fd != -1) return(1);

	//以只读方式打开client_pipe_name管道,通过它接收服务器发给客户端的响应
    client_fd = open(client_pipe_name, O_RDONLY);
    if (client_fd != -1) {
        client_write_fd = open(client_pipe_name, O_WRONLY);
        if (client_write_fd != -1) return(1);
        (void)close(client_fd);
        client_fd = -1;
    }
    return(0);
}

/* Here's the main read from the server which gets the matching database entries. */

int read_resp_from_server(message_db_t *rec_ptr)
{
    int read_bytes;
    int return_code = 0;

    #if DEBUG_TRACE    
        printf("%d :- read_resp_from_server()\n",  getpid());    
    #endif

    if (!rec_ptr) return(0);
    if (client_fd == -1) return(0);

    read_bytes = read(client_fd, rec_ptr, sizeof(*rec_ptr));
    if (read_bytes == sizeof(*rec_ptr)) return_code = 1;
    return(return_code);
}

/* And finally, the client function that marks the end of the server response. */

void end_resp_from_server(void)
{
    #if DEBUG_TRACE    
        printf("%d :- end_resp_from_server()\n",  getpid());        
    #endif

    /* This function is empty in the pipe implementation */
}

