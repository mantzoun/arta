/*
 * Copyright 2025 Stavros Mantzouneas
 */
#ifndef INCLUDE_DISCORDCHANNEL_H_
#define INCLUDE_DISCORDCHANNEL_H_

#include <string>

#include "dpp/dpp.h"

namespace arta {
class DiscordChannel {
 private:
  dpp::snowflake id;
  dpp::snowflake parent;
  std::string    name;
 public:
  DiscordChannel(dpp::snowflake id,
                 dpp::snowflake parent,
                 const std::string & name);

  dpp::snowflake  idGet(void) const;
  dpp::snowflake  parentGet(void) const;
  std::string     nameGet(void) const;

  void parentSet(dpp::snowflake parent);
  void nameSet(const std::string& name);

  bool operator==(const DiscordChannel& other) const {
    return (id == other.idGet() &&
              parent == other.parentGet() &&
              name == other.nameGet());
  }
};
}  // namespace arta

#endif  // INCLUDE_DISCORDCHANNEL_H_
