/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * MySocket.h
 *
 * Class for areas
 */

#ifndef INCLUDE_MYSOCKET_H_
#define INCLUDE_MYSOCKET_H_

#include <string>
#include <stdint.h>
#include <thread>
#include <sys/un.h>
#include <mutex>
#include <queue>

namespace arta {
/**
 * @class MySocket
 *
 * @brief Arta Socket
 */
class MySocket {
    private:
        int mode;  // 0 Server, 1 client
        std::string socketPath = "";
        int connected = -1;
        int fd = -1;
        int clientFd = -1;
        std::thread * bgThread;

        std::queue<std::string> messageQueue;
        std::mutex              queueMutex;
        std::string readBytes(int bytes);
        bool sendString(std::string str);
        void pushToQueue(std::string);
        void socketPathSet(std::string path);
        void modeSet(int m);
    public:
        bool sendMessage(const std::string message);
        std::string recvMessage(void);
        int init(int mode, std::string socketPath);
        void listenForMessages(void);
        void serverLoop(sockaddr_un addr);
        void clientLoop(sockaddr_un addr);
        std::string popFromQueue();
};
}
#endif  /* INCLUDE_MYSOCKET_H_ */