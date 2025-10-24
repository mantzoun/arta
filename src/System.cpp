/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * System.cpp
 */

#include "include/System.h"

namespace arta {
void System::init(void) {
    for (std::list<Area>::iterator it = areas.begin(); it != areas.end(); ++it){
        (*it).loggerSet(logger);
        (*it).utilsSet(utils);
        (*it).timeManagerSet(timeManager);

        (*it).init();
    }
}

void System::tikAdvance(void) {
    logger->debug("System " + name + " advancing");

    for (std::list<Area>::iterator it = areas.begin(); it != areas.end(); ++it){
        (*it).tikAdvance();
    }
}
}
