/*
 * Copyright 2025 Stavros Mantzouneas
 */
#ifndef INCLUDE_DISCORDGUILD_H_
#define INCLUDE_DISCORDGUILD_H_

#include <string>
#include <list>

#include "dpp/dpp.h"
#include "include/DiscordChannel.h"
#include "include/Logger.h"

namespace arta {
class DiscordGuild {
 private:
  dpp::snowflake id;
  std::list<DiscordChannel> channels;
  Logger * logger = NULL;

  int verifyAndAddChannel(const DiscordChannel& channel);
  int verifyAndDeleteChannel(const DiscordChannel& channel);
  int verifyAndUpdateChannel(const DiscordChannel& channel);

 public:
  explicit DiscordGuild(const dpp::snowflake id);

  dpp::snowflake idGet(void) const;

  void loggerSet(Logger * logger);

  int channelAdd(const dpp::channel_create_t & channel);
  int channelAdd(const dpp::channel & channel);
  int channelDelete(const dpp::channel_delete_t & channel);
  int channelUpdate(const dpp::channel_update_t & channel);

  std::list<DiscordChannel *> channelGet(const std::string & name,
                                            dpp::snowflake channelId = 0,
                                            dpp::snowflake parentId = 0);
  DiscordChannel * channelGet(const std::string & name,
                               const std::string & parent);
  std::list<DiscordChannel> channelGetAll(void) const;
  DiscordChannel * channelGetById(dpp::snowflake id);
};
}  // namespace arta

#endif  // INCLUDE_DISCORDGUILD_H_
