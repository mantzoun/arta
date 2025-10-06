/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * Universe.cpp
 */

#include "include/Universe.h"

namespace arta {
void Universe::init(void) {
    for (int i = 0; i < 5; i++) {
        System * s = new System(this->logger, timeManager, utils, utils->idGet(), "system " + std::to_string(i));

        s->init();

        systems.push_back(s);
    }
}

void Universe::tikAdvance(void) {
    logger->debug("Universe " + name + " advancing");

    for (std::list<System*>::iterator it = systems.begin(); it != systems.end(); ++it){
        (*it)->tikAdvance();
    }
}
}