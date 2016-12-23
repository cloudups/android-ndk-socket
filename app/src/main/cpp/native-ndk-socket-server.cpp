#include <jni.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <android/log.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 3333
#define MAXLINKNUM 10

void server() {

    //socket 句柄
    int server_fd, client_fd;

    //声明服务器和客户端地址数据结构
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    //创建socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        __android_log_print(ANDROID_LOG_INFO, "jni", "create server socket fail");
        return;
    }

    //清空服务器地址信息
    memset(&server_addr, 0, sizeof(server_addr));

    //配置服务器地址信息
    server_addr.sin_family = AF_INET;//ip v4
    server_addr.sin_port = htons(PORT);//端口
    server_addr.sin_addr.s_addr = INADDR_ANY;//本机ip地址

    //绑定socket
    if (bind(server_fd, (sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        __android_log_print(ANDROID_LOG_INFO, "jni", "bind socket fail");
        return;
    }

    //监听socket
    if (listen(server_fd, MAXLINKNUM) == -1) {
        __android_log_print(ANDROID_LOG_INFO, "jni", "listen socket fail");
        return;
    }

    //接受客户端的连接
    while (true) {

        socklen_t client_addr_len = sizeof(client_addr);
        //accept socket
        if ((client_fd = accept(server_fd, (sockaddr *) &client_addr, &client_addr_len)) == -1) {
            __android_log_print(ANDROID_LOG_INFO, "jni", "accept socket fail");
            continue;
        }

        //开启子进程
        if (!fork()) {
            __android_log_print(ANDROID_LOG_INFO, "jni", "accept a client from : %s",
                                inet_ntoa(client_addr.sin_addr));

            //定义发送信息
            char *message = "already link to server";
            size_t len = sizeof(message);

            //发送信息到客户端
            if (send(client_fd, message, len, 0) == -1) {

                __android_log_print(ANDROID_LOG_INFO, "jni", "send fail");
                //关闭连接
                close(client_fd);
                //关闭子进程
                exit(0);
            }

            //关闭连接
            close(client_fd);
        }
    }

    return;
}


extern "C"
void Java_com_cloudups_socket_MainActivity_startServer(JNIEnv *env, jobject instance) {

    server();
    return;
}
