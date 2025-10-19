/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * System.cpp
 */

#include "include/System.h"

namespace arta {
void System::init(void) {
    for (int i = 0; i < 5; i++) {
        Area a = Area(this->logger, timeManager, utils, utils->idGet(), this->nameGet() + " Area " + std::to_string(i));

        a.populationSet(5000000000);
        a.typeSet(static_cast<AreaType> (utils->roll(AREA_TYPE_MAX)));

        if (a.typeGet() == AREA_TYPE_PLANET) {
            a.consumerTypeSet(CONSUMER_TYPE_PLANET);
        } else if (a.typeGet() == AREA_TYPE_STATION) {
            a.consumerTypeSet(CONSUMER_TYPE_STATION);
        }
        a.populationBaseGrowthSet(utils->roll(1, 10));
        areas.push_back(a);
    }

    this->consumerTypeSet(CONSUMER_TYPE_SYSTEM);
}

void System::tikAdvance(void) {
    logger->debug("System " + name + " advancing");

    for (std::list<Area>::iterator it = areas.begin(); it != areas.end(); ++it){
        (*it).tikAdvance();
    }
}
}