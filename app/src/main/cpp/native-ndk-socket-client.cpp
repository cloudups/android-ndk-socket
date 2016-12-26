#include <jni.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <android/log.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVERPORT 3336
#define MAXDATASIZE 1024

void client() {

    //客户端socket句柄
    int client_fd;

    //数据
    char buf[MAXDATASIZE];

    //服务器地址
    struct sockaddr_in server_addr;

    //创建socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        __android_log_print(ANDROID_LOG_INFO, "jni", "socket create fail");
        return;
    }

    //置空服务器地址信息
    memset(&server_addr, 0, sizeof(server_addr));

    //填充服务器地址信息
    server_addr.sin_family = AF_INET;//ip v4
    server_addr.sin_port = htons(SERVERPORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //连接服务器
    if (connect(client_fd, (sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        __android_log_print(ANDROID_LOG_INFO, "jni", "socket connect fail");
        return;
    }


    int recvbytes = 10;
    //接收数据
    if ((recvbytes = recv(client_fd, buf, MAXDATASIZE, 0)) == -1) {
        __android_log_print(ANDROID_LOG_INFO, "jni", "socket recv fail");
        return;
    }

    //输出接收到的数据
//    buf[recvbytes] = '\0';
    __android_log_print(ANDROID_LOG_INFO, "jni", "socket recv: %s, message length is : %d", buf,
                        recvbytes);

    //关闭socket
    close(client_fd);
    return;
}

extern "C"
void Java_com_cloudups_socket_MainActivity_startClient(JNIEnv *env, jobject instance) {

    client();
    return;
}
