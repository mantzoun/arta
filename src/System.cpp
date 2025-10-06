/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * System.cpp
 */

#include "include/System.h"

namespace arta {
void System::init(void) {
    for (int i = 0; i < 5; i++) {
        Area * a = new Area(this->logger, timeManager, utils, utils->idGet(), "Area " + std::to_string(i));

        a->populationSet(utils->roll(1000, 10000));
        a->typeSet(static_cast<AreaType> (utils->roll(AREA_TYPES_MAX)));

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