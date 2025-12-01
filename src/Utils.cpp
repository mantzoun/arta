/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * Utils.cpp
 */
#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>

#include "include/Utils.h"

namespace arta {
Utils::Utils(void) {
    srand(time(0));
}

id_t Utils::idGet(void) {
    return nextId++;
}

int Utils::roll(int max) {
    return rand() % (1 + max);
}

int Utils::roll(int min, int max) {
    return min + (rand() %(1 + max - min));
}

int Utils::sendMessage(std::string message) {
    // Open pipe in non-blocking write-only mode
    int fd = open("/tmp/discord_client.fifo", O_WRONLY | O_NONBLOCK);
    if (fd == -1) {
        return -1;
    }

    ssize_t written = write(fd, message.c_str(), message.size());
    if (written == -1) {
        return -2;
    }
    write(fd, "\n", 1);

    close(fd);
    return 0;
}

std::vector<std::string> Utils::tokenize(const std::string &string_with_spaces) {
    std::istringstream iss(string_with_spaces);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) tokens.push_back(token);
    return tokens;
}
}  // namespace arta
