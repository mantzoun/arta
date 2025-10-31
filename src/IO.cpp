/*
 * IO.cpp
 *
 * Implementation of IO class methods
 */

#include <fstream>
#include <unistd.h>

#include "include/IO.h"

namespace arta {
IO::IO(void) {
}

void IO::fifoRead(void) {
     logger->debug("IO: starting fifo");

    std::ifstream fifo{fifoPath};
    std::string line;

    while (!fifoStop && !fifo.fail()){
        while (std::getline(fifo, line)){
            logger->debug("IO: fifo input: " + line);
            consumer->messageCb(line);
        }
        if (fifo.eof()){
            fifo.clear();
        }

        usleep(500000);
    }

    logger->warn("IO: exiting fifo io loop");
}

bool IO::fifoInit(std::string path) {
    if (fifoThread != NULL){
        logger->warn("IO: IO thread already running");
        return false;
    }

    fifoPath = path;
    fifoStop = false;
    fifoThread = new std::thread(&IO::fifoRead, this);

    if (fifoThread == NULL){
        return false;
    }

    return true;
}

bool IO::fifoTerminate(void) {
    if (fifoThread == NULL){
        logger->warn("IO: IO thread not running");
        return false;
    }

    fifoStop = true;
    fifoThread->join();
    delete fifoThread;
    fifoThread = NULL;

    return true;
}

void IO::setLogger(Logger * l) {
    logger = l;
}

void IO::setConsumer(MessageConsumer * consumer) {
    this->consumer = consumer;
}
}  // namespace arta
