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
  dpp::snowflake  _id;
  dpp::snowflake  _parent;
  std::string _name;
 public:
  DiscordChannel(dpp::snowflake id,
                 dpp::snowflake parent,
                 const std::string & name);

  dpp::snowflake  id(void) const;
  dpp::snowflake  parent(void) const;
  std::string     name(void) const;

  void set_parent(dpp::snowflake parent);
  void set_name(const std::string& name);

  bool operator==(const DiscordChannel& other) const {
    return (_id == other.id() &&
              _parent == other.parent() &&
              _name == other.name());
  }
};
}  // namespace arta

#endif  // INCLUDE_DISCORDCHANNEL_H_
