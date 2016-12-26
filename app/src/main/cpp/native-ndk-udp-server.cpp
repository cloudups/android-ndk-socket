#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <android/log.h>


int server_udp_port = 5678;

void udp_server() {

    //服务器socket句柄
    int server_fd;

    //服务器地址信息
    struct sockaddr_in server_addr;

    //设置服务器地址信息
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_udp_port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //创建socket
    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        __android_log_print(ANDROID_LOG_INFO, "jni", "create socket fail!");
        return;
    }

    //绑定socket
    if ((bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)) {
        __android_log_print(ANDROID_LOG_INFO, "jni", "bind socket fail!");
        return;
    }

    int recybytes = 0;
    char buf[256];
    sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);

    //接收信息
    while (true) {
        if (recybytes = recvfrom(server_fd, buf, sizeof(buf), 0, (
                struct sockaddr *) &client_addr, &length) < 0) {
            __android_log_print(ANDROID_LOG_INFO, "jni", "recvfrom fail");
            continue;
        }

        __android_log_print(ANDROID_LOG_INFO, "jni", "recvfrom message is : %s", buf);
    }
}

extern "C"
void Java_com_cloudups_socket_MainActivity_startUpdServer(JNIEnv *env, jobject instance) {

    udp_server();
    return;
}