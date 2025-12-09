#include <jni.h>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <android/log.h>
#include "./controller-protocol.h"

#define LOG_TAG "Controller"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

extern "C" JNIEXPORT jboolean JNICALL
Java_com_controller_MainActivity_sendMessage(
        JNIEnv* env,
        jobject,
        jstring serverIp,
        jstring message) {
    
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        LOGD("Failed to create socket");
        return JNI_FALSE;
    }
    
    sockaddr_in server = {};
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
    
    const char* ip = env->GetStringUTFChars(serverIp, nullptr);
    if (inet_pton(AF_INET, ip, &server.sin_addr) <= 0) {
        LOGD("Invalid IP: %s", ip);
        env->ReleaseStringUTFChars(serverIp, ip);
        close(sock);
        return JNI_FALSE;
    }
    env->ReleaseStringUTFChars(serverIp, ip);
    
    const char* msg = env->GetStringUTFChars(message, nullptr);
    ssize_t sent = sendto(sock, msg, strlen(msg), 0, 
                          (sockaddr*)&server, sizeof(server));
    
    LOGD("Sent %zd bytes: %s", sent, msg);
    env->ReleaseStringUTFChars(message, msg);
    
    close(sock);
    
    return (sent > 0) ? JNI_TRUE : JNI_FALSE;
}