/*
 * Copyright 2025 Stavros Mantzouneas
 */
#include "include/DiscordChannel.h"

namespace arta {
DiscordChannel::DiscordChannel(dpp::snowflake id,
                               dpp::snowflake parent,
                               const std::string & name) :
                                    id(id),
                                    parent(parent),
                                    name(name) {
}

dpp::snowflake DiscordChannel::idGet(void) const {
  return id;
}

dpp::snowflake DiscordChannel::parentGet(void) const {
  return parent;
}

std::string DiscordChannel::nameGet(void) const {
  return name;
}

void DiscordChannel::parentSet(dpp::snowflake parent) {
  this->parent = parent;
}

void DiscordChannel::nameSet(const std::string& name) {
  this->name = name;
}
}  // namespace arta
