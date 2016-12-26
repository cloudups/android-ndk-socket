#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>
#include <android/log.h>

int UDP_PORT = 5678;

void upd_client() {

    //客户端socket句柄
    int client_fd;

    //服务器地址信息
    struct sockaddr_in server_addr;

    //设置服务器地址信息
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //创建socket
    client_fd = socket(AF_INET, SOCK_DGRAM, 0);

    char *buf = "message";
    socklen_t length = sizeof(server_addr);
    //发送udp包
    if (sendto(client_fd, buf, sizeof(buf), 0, (struct sockaddr *) &server_addr, length) == -1) {
        __android_log_print(ANDROID_LOG_INFO, "jni", "send to fail!");
        return;
    }

    close(client_fd);
    return;
}

extern "C"
void Java_com_cloudups_socket_MainActivity_startUdpClient(JNIEnv *env, jobject instance) {

    upd_client();
    return;
}
