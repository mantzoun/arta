/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * System.cpp
 */

#include "include/System.h"

namespace arta {
void System::init(void) {
    for (int i = 0; i < 5; i++) {
        Area * a = new Area(this->logger, timeManager, idManager, idManager->idGet(), "Area " + std::to_string(i));

        areas.push_back(a);
    }
}

void System::tikAdvance(void) {
    logger->debug("System " + name + " advancing");

    for (std::list<Area*>::iterator it = areas.begin(); it != areas.end(); ++it){
        (*it)->tikAdvance();
    }
}
}