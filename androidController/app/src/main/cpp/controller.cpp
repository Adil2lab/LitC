#include "controller-protocol.h"
#include <android/log.h>
#include <arpa/inet.h>
#include <cstdint>
#include <cstring>
#include <jni.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define LOG_TAG "Controller"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static int g_socket = -1;
static sockaddr_in g_server_addr = {};
static int32_t g_sequence = 0;

extern "C" JNIEXPORT jboolean JNICALL
Java_com_controller_MainActivity_initNetwork(JNIEnv *env, jobject,
                                             jstring serverIp) {
  // Close existing socket
  if (g_socket >= 0) {
    close(g_socket);
    g_socket = -1;
  }

  // Create socket
  g_socket = socket(AF_INET, SOCK_DGRAM, 0);
  if (g_socket < 0) {
    LOGE("Failed to create socket");
    return JNI_FALSE;
  }

  // === LOW LATENCY SOCKET OPTIONS ===

  // 1. Small send buffer to reduce kernel buffering delay
  int sndbuf = 4096; // 4KB - enough for our 18-byte packets
  setsockopt(g_socket, SOL_SOCKET, SO_SNDBUF, &sndbuf, sizeof(sndbuf));

  // 2. Set IP TOS for low-delay traffic (DSCP class)
  int tos = 0x10; // IPTOS_LOWDELAY
  setsockopt(g_socket, IPPROTO_IP, IP_TOS, &tos, sizeof(tos));

  // Setup server address
  g_server_addr = {};
  g_server_addr.sin_family = AF_INET;
  g_server_addr.sin_port = htons(8888);

  const char *ip = env->GetStringUTFChars(serverIp, nullptr);
  if (inet_pton(AF_INET, ip, &g_server_addr.sin_addr) <= 0) {
    LOGE("Invalid IP address: %s", ip);
    env->ReleaseStringUTFChars(serverIp, ip);
    close(g_socket);
    g_socket = -1;
    return JNI_FALSE;
  }
  env->ReleaseStringUTFChars(serverIp, ip);
  LOGD("Network initialized with low-latency options.");
  return JNI_TRUE;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_controller_MainActivity_sendControllerData(
    JNIEnv *env, jobject, jint buttons, jint leftX, jint leftY, jint rightX,
    jint rightY, jint leftTrigger, jint rightTrigger) {
  if (g_socket < 0) {
    LOGE("Socket not initialized");
    return JNI_FALSE;
  }

  // Pre-allocate static packet to avoid stack allocation overhead in hot path
  static controller::Packet packet;

  packet.packetId = g_sequence++;
  packet.buttons = static_cast<uint16_t>(buttons);
  packet.leftX = static_cast<int16_t>(leftX);
  packet.leftY = static_cast<int16_t>(leftY);
  packet.rightX = static_cast<int16_t>(rightX);
  packet.rightY = static_cast<int16_t>(rightY);
  packet.leftTrigger = static_cast<uint8_t>(leftTrigger);
  packet.rightTrigger = static_cast<uint8_t>(rightTrigger);
  packet.checksum = controller::calculateChecksum(&packet);

  ssize_t sent = sendto(g_socket, &packet, sizeof(packet), 0,
                        reinterpret_cast<sockaddr *>(&g_server_addr),
                        sizeof(g_server_addr));
  return (sent == sizeof(packet)) ? JNI_TRUE : JNI_FALSE;
}

extern "C" JNIEXPORT void JNICALL
Java_com_controller_MainActivity_closeNetwork(JNIEnv *env, jobject) {
  if (g_socket >= 0) {
    close(g_socket);
    g_socket = -1;
  }
  LOGD("Network closed.");
}