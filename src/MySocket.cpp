/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * MySocket.cpp
 */
#include <fcntl.h>
#include <format>
#include <vector>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <sys/socket.h>
#include "include/MySocket.h"
#include <poll.h>

namespace arta {

void MySocket::pushToQueue(std::string message) {
    std::lock_guard<std::mutex> lock(queueMutex);
    messageQueue.push(message);
}

std::string MySocket::popFromQueue() {
    std::string message = "";

    if (queueMutex.try_lock()) {
        if (!messageQueue.empty()) {
            message = messageQueue.front();
            messageQueue.pop();
        }
        queueMutex.unlock();
    }

    return message;
}

bool MySocket::sendString(std::string str) {
    size_t sent = 0;
    int myFd = (mode == 0) ? clientFd : fd;

    while (sent < str.size()) {
        ssize_t n = send(myFd, str.data() + sent, str.size() - sent, 0);
        if (n <= 0) {
            return false;
        }
        sent += n;
    }
    return true;
}


bool MySocket::sendMessage(const std::string message) {
    std::string msgLen = std::format("{:04}", message.length());

    if (! sendString(msgLen)) {
        std::cout << "error sending message " << errno << "\n";
        return false;
    }

    return sendString(message);
}

std::string MySocket::readBytes(int bytes) {
    std::vector<char> recvBuffer(bytes);
    int bytesRead = 0;

    int myFd = (mode == 0) ? clientFd : fd;

    struct pollfd pfd;
    pfd.fd = myFd;
    pfd.events = POLLIN;

    while (bytesRead < bytes) {
        int ret = poll(&pfd, 1, -1);
        if (ret <= 0) continue;

        ssize_t n = read(myFd, recvBuffer.data() + bytesRead, bytes - bytesRead);

        if (n > 0) {
            bytesRead += n;
        } else if (n == 0) {
            // connection closed
            return "";
        } else {
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                // something is wrong
                return "";
            }
        }
    }

    return std::string(recvBuffer.data(), recvBuffer.size());
}

std::string MySocket::recvMessage() {
    std::string msgSize = readBytes(4);

    if (msgSize == "") {
        return "";
    }

    size_t msgLen = std::stoi(msgSize);

    return readBytes(msgLen);
}

void MySocket::modeSet(int m) {
    mode = m;
}

void MySocket::socketPathSet(std::string path) {
    socketPath = path;
}

void MySocket::serverLoop(sockaddr_un addr) {
    listen(fd, 1);

    while(true) {
        clientFd = accept(fd, nullptr, nullptr);
        while (clientFd < 0) {
            if (errno == EAGAIN) {
                // No pending connection yet, try again later
                usleep(1000);
                clientFd = accept(fd, nullptr, nullptr);
            } else {
                perror("accept failed");
                usleep(1000000);
            }
        }

        std::cout << "client connected" << "\n";

        while (clientFd >= 0) {
            std::string message = recvMessage();

            if (message == "") {
                close(clientFd);
                clientFd = -1;
                break;
            }

            std::cout << message << "\n";
            pushToQueue(message);
        }

        std::cout << "client disconnected" << "\n";
    }
}

void MySocket::clientLoop(sockaddr_un addr) {
    while(true) {
        int r = -1;

        while (r < 0) {
            usleep(1000);
            r = connect(fd, (sockaddr*)&addr, sizeof(addr));
        }

        std::cout << "connected to server" << "\n";

        while(true) {
            std::string message = recvMessage();

            if (message == "") {
                r = -1;
                close(fd);
                fd = socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0);
                break;
            }

            std::cout << message << "\n";
            pushToQueue(message);
        }

        std::cout << "disconnected from server" << "\n";
    }
}

int MySocket::init(int mode, std::string socketPath) {
    modeSet(mode);
    socketPathSet(socketPath);

    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    fcntl(fd, F_SETFL, O_NONBLOCK);

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, socketPath.c_str());

    if ( mode == 0) {
        unlink(socketPath.c_str());

        if (bind(fd, (sockaddr*)&addr, sizeof(addr))) {
            std::cout << "failed to bind socket" << "\n";
            std::cout << errno << "\n";
            return -1;
        }

        bgThread = new std::thread(&MySocket::serverLoop, this, addr);
    } else {
        bgThread = new std::thread(&MySocket::clientLoop, this, addr);
    }

    if (bgThread == NULL){
        std::cout << "failed to start connection thread" << "\n";
        return -1;
    }

    return 0;
}

void MySocket::listenForMessages() {
    while (true) {
        std::string msg = recvMessage();
        if (msg != "") {
            std::cout << "Client received: "
                      << msg << "\n";
        }
        usleep(1000);
    }
}

}  /* namespace arta */