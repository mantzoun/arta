/*
 * Copyright 2025 Stavros Mantzouneas
 */
#include "include/DiscordGuild.h"

namespace arta {
DiscordGuild::DiscordGuild(const dpp::snowflake id)
    : id(id) {
}

void DiscordGuild::loggerSet(Logger * logger) {
  this->logger = logger;
}

dpp::snowflake  DiscordGuild::idGet(void) const {
  return id;
}

int DiscordGuild::verifyAndAddChannel(const DiscordChannel& channel) {
  if (std::any_of(channels.begin(), channels.end(), [&](const auto& item) {
    return item == channel;
  })) {
    // channel exists
    logger->warn("Channel already exists : " + channel.nameGet() +
                 "," + std::to_string(channel.idGet()) +
                 "," + std::to_string(channel.parentGet()));
    return -1;
  }

  channels.push_back(channel);
  logger->debug("Added channel : " + channel.nameGet() +
                "," + std::to_string(channel.idGet()) +
                "," + std::to_string(channel.parentGet()));
  return 0;
}

int DiscordGuild::verifyAndDeleteChannel(const DiscordChannel& channel) {
  for (auto it = channels.begin(); it != channels.end(); ) {
    if (*it == channel) {
      channels.erase(it);
      logger->debug("Deleted channel : " + channel.nameGet() +
                    "," + std::to_string(channel.idGet()) +
                    "," + std::to_string(channel.parentGet()));
      return 0;
    }
    it++;
  }

  logger->warn("Could not delete channel : " + channel.nameGet() +
               "," + std::to_string(channel.idGet()) +
               "," + std::to_string(channel.parentGet()));
  return -1;
}

int DiscordGuild::verifyAndUpdateChannel(const DiscordChannel& channel) {
  for (auto it = channels.begin(); it != channels.end(); ) {
    if (it->idGet() == channel.idGet()) {
      it->nameSet(channel.nameGet());
      it->parentSet(channel.parentGet());
      logger->debug("Updated channel : " + channel.nameGet() +
                    "," + std::to_string(channel.idGet()) +
                    "," + std::to_string(channel.parentGet()));
      return 0;
    }
    it++;
  }

  logger->warn("Could not update channel : " + channel.nameGet() +
               "," + std::to_string(channel.idGet()) +
               "," + std::to_string(channel.parentGet()));
  return -1;
}

int DiscordGuild::channelAdd(const dpp::channel & channel) {
  return verifyAndAddChannel(DiscordChannel(channel.id,
                                    channel.parent_id, channel.name));
}

int DiscordGuild::channelAdd(const dpp::channel_create_t & channel) {
  return verifyAndAddChannel(DiscordChannel(channel.created.id,
                                                 channel.created.parent_id,
                                                 channel.created.name));
}

int DiscordGuild::channelUpdate(const dpp::channel_update_t & channel) {
  return verifyAndUpdateChannel(DiscordChannel(channel.updated.id,
                                                    channel.updated.parent_id,
                                                    channel.updated.name));
}

int DiscordGuild::channelDelete(const dpp::channel_delete_t & channel) {
  return verifyAndDeleteChannel(DiscordChannel(channel.deleted.id,
                                                    channel.deleted.parent_id,
                                                    channel.deleted.name));
}

DiscordChannel * DiscordGuild::channelGet(const std::string & name,
                                           const std::string & parent) {
  DiscordChannel * result = NULL;
  dpp::snowflake parentId = 0;

  if (parent != "") {
    DiscordChannel * parentChannel = this->channelGet(parent, "");
    if (parentChannel == NULL) {
      logger->error("Could not find parent channel");
      return NULL;
    }

    parentId = parentChannel->idGet();
  }

  for (DiscordChannel& channel : channels) {
    if (channel.nameGet() == name &&
      (parentId == 0 || channel.parentGet() == parentId)) {
      if (result == NULL) {
        result = &channel;
      } else {
        this->logger->error("Found duplicate channel " + name);
      }
    }
  }

  return result;
}

std::list<DiscordChannel *> DiscordGuild::channelGet(const std::string& name,
                                                    dpp::snowflake channelId,
                                                    dpp::snowflake parentId) {
  std::list<DiscordChannel *> result;

  for (DiscordChannel& channel : channels) {
    if (channel.nameGet() == name &&
                (parentId == 0 || channel.parentGet() == parentId) &&
                (channelId == 0 || channel.idGet() == channelId)) {
      logger->debug("Add search result channel : " + channel.nameGet() +
                    "," + std::to_string(channel.idGet()) +
                    "," + std::to_string(channel.parentGet()));
      result.push_back(&channel);
    }
  }

  return result;
}

DiscordChannel * DiscordGuild::channelGetById(dpp::snowflake id) {
  if (id == 0) {
    return NULL;
  }

  auto it = std::find_if(channels.begin(), channels.end(),
  [&](const auto& channel) { return id == channel.idGet(); });

  if (it != channels.end()) {
    return &(*it);
  }

  return NULL;
}
}  // namespace arta
