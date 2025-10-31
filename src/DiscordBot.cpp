/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * DiscordBot.coo
 *
 * implementation of CDB_DiscordBot methods
 */
#include<thread>
#include <unistd.h>

#include "include/DiscordBot.h"

static std::vector<std::string> non_engine_categories = {"Text Channels"};

namespace arta {
void register_guild_commands();

DiscordBot::DiscordBot(void) {
}

void DiscordBot::ChildChannelCreateDelayed(std::string channel, std::string parent) {
  int retries = 10;
  DiscordChannel * c = this->guild->channelGet(parent, "");

  while ((retries > 0) && (c == NULL)) {
      retries--;
      usleep(1000 * 1000);
      this->logger->debug("LOOP1");
      c = this->guild->channelGet(parent, "");
  }

  if (retries == 0) {
    return;
  }

  this->channelCreate(c->idGet(), channel, dpp::CHANNEL_TEXT);
}

void DiscordBot::MessagePostDelayed(std::string channel, std::string parent, std::string message) {
  int retries = 10;
  DiscordChannel * c = this->guild->channelGet(channel, parent);

  while (retries > 0 && c == NULL) {
    retries--;
    usleep(1000 * 1000);
    c = this->guild->channelGet(channel, parent);
    this->logger->debug("LOOP2");
  }

    if (retries == 0) {
    return;
  }

  this->postMessage(channel, message);
}
// ===================================================================
//                          INTERFACE
// ===================================================================
void DiscordBot::messageCb(std::string * message) {
  logger->info(*message);
  std::thread * myThread;

  DiscordChannel * channel = this->guild->channelGet("area1", "system1");
  if (channel == NULL) {
    channel = this->guild->channelGet("system1", "");
    if (channel == NULL) {
      // No system, create system, start threads for area creation and message post
      this->channelCreate(0, "system1", dpp::CHANNEL_CATEGORY);
      //init area thread
      myThread = new std::thread(&DiscordBot::ChildChannelCreateDelayed, this,"area1", "system1");
      myThread->detach();
    }
  }

  //init message thread
  myThread = new std::thread(&DiscordBot::MessagePostDelayed, this, "area1", "system1", "THIS IS THE TEST MESSAGE");
  myThread->detach();
}

// ===================================================================
//                          MESSAGES
// ===================================================================
void DiscordBot::postMessage(const std::string & channelName,
                              const std::string & text) {
  dpp::message message;

  std::list<DiscordChannel *> channels = this->guild->channelGet(channelName);

  if (channels.size() == 0) {
    logger->error("No channels found to post : " + channelName);
    return;
  }

  for (DiscordChannel * channel : channels) {
    message.channel_id = channel->idGet();
    message.guild_id = this->guildId;
    message.content = text;
    logger->debug("Post to channel : " + channel->nameGet() + "," + std::to_string(channel->idGet()) +
                  "," + std::to_string(channel->parentGet()));
    this->discordIface->message_create(message);
  }
}

// ===================================================================
//                          CHANNELS
// ===================================================================
void DiscordBot::channelCreate(dpp::snowflake parent_id,
                                const std::string & name,
                                dpp::channel_type chanType) {
  dpp::channel chan;

  chan.name = name;
  chan.guild_id = this->guildId;
  chan.set_type(chanType);

  if (parent_id != 0) {
    chan.parent_id = parent_id;
  }

  this->discordIface->channel_create(chan);
}

void DiscordBot::channelDelete(const std::string & name) {
  std::list<DiscordChannel *> channels = this->guild->channelGet(name);

  if (channels.size() != 1) {
    logger->warn("Number of channels to delete not 1 : " +
                    std::to_string(channels.size()));
    return;
  }

  DiscordChannel * discord_channel = channels.front();
  this->discordIface->channel_delete(discord_channel->idGet(), NULL);
}

void DiscordBot::channelRename(const std::string & name,
                                std::string newName) {
  std::list<DiscordChannel *> channels = this->guild->channelGet(name);

  if (channels.size() != 1) {
    logger->warn("Number of channels to rename not 1 : " +
                    std::to_string(channels.size()));
    return;
  }

  DiscordChannel * discordChannel = channels.front();

  dpp::channel channel;

  channel.id = discordChannel->idGet();
  channel.set_name(newName);
  this->discordIface->channel_edit(channel, NULL);
}

dpp::command_completion_event_t DiscordBot::channelsGetCallback(dpp::confirmation_callback_t value) {
  this->logger->debug("channels Callback");

  if ( value.is_error() == true ) {
    dpp::error_info err = value.get_error();
    logger->error("Error " + err.message);
  }

  dpp::channel_map channelmap = std::get<dpp::channel_map>(value.value);

  dpp::snowflake id;
  dpp::channel channel;

  dpp::snowflake parent_id;

  std::list default_channels = {"syslog"};

  for (const auto& it : channelmap) {
    id = it.first;
    channel = it.second;

    parent_id = channel.parent_id;

    this->logger->debug("" + channel.name + " " +
                             channel.id.str() + " " +
                             channel.parent_id.str());
    this->guild->channelAdd(channel);

    this->logger->debug("Found channel " + channel.name +
                        " with id " + std::to_string(id));
    }

  for (std::string ch_name : default_channels) {
    std::list<DiscordChannel *> channels = this->guild->channelGet(ch_name);

    if (channels.size() > 0) {
      continue;
    }

    this->logger->debug("Init Create channel " + ch_name);

    dpp::channel chan;

    chan.name = ch_name;
    chan.guild_id = this->guildId;
    chan.set_type(dpp::CHANNEL_TEXT);

    // std::function<void(const dpp_cb_t&)> callback =
    // std::bind(&DiscordBot::channels_create_cb, this, std::placeholders::_1);
    this->discordIface->channel_create(chan);  //, callback);
  }

  this->initComplete = true;

  return NULL;
}

dpp::command_completion_event_t DiscordBot::userGetGuildsCallback(dpp::confirmation_callback_t value) {
  this->logger->debug("Guilds Callback");
  if (value.is_error() == true) {
    dpp::error_info err = value.get_error();
    this->logger->error("Error " + err.message);
  }

  dpp::guild_map guildmap = std::get<dpp::guild_map>(value.value);

  if (guildmap.size() > 1) {
    this->logger->error("Error, too many guilds ");
    return NULL;
  }

  dpp::snowflake id;
  dpp::guild g;

  for (const auto& it : guildmap) {
    id = it.first;
    g = it.second;

    this->initializeGuild(id);
    this->guildId = id;

    this->slashCommandsInit();

    std::function<void(const dpp::confirmation_callback_t&)> callback =
    std::bind(&DiscordBot::channelsGetCallback, this, std::placeholders::_1);

    this->discordIface->channels_get(id, callback);
  }

  return NULL;
}

void DiscordBot::initializeGuild(dpp::snowflake id) {
  this->guild = new DiscordGuild(id);
  this->guild->loggerSet(logger);
}

void DiscordBot::init(std::string token, const std::string & id) {
  this->discordIface = new dpp::cluster(token);

  // luxbracer_discord_bot = this;

  this->botId = id;

  // Use our own logger for output consistency
  this->discordIface->on_log([this](const dpp::log_t & event) {
  switch (event.severity) {
    case dpp::ll_trace:
    case dpp::ll_debug:
      this->logger->debug("Discord Bot: " + event.message);
      break;
    case dpp::ll_info:
      this->logger->info("Discord Bot: " +  event.message);
      break;
    case dpp::ll_warning:
      this->logger->warn("Discord Bot: " + event.message);
      break;
    case dpp::ll_error:
    case dpp::ll_critical:
    default:
      this->logger->error("Discord Bot: " + event.message);
      break;
    }
  });

  this->discordIface->on_channel_create(
          [this](const dpp::channel_create_t & channel) {
    this->guild->channelAdd(channel);
  });

  this->discordIface->on_channel_delete(
          [this](const dpp::channel_delete_t & channel) {
    this->guild->channelDelete(channel);
  });

  this->discordIface->on_channel_update(
          [this](const dpp::channel_update_t & channel) {
    this->guild->channelUpdate(channel);
  });

  this->discordIface->on_slashcommand(
          [this](const dpp::slashcommand_t & event) {
    this->slashCommandsHandle(event);
  });

  this->discordIface->on_ready([this](const dpp::ready_t& event) {
    std::function<void(const dpp::confirmation_callback_t&)> callback =
    std::bind(&DiscordBot::userGetGuildsCallback,
              this, std::placeholders::_1);

    this->discordIface->current_user_get_guilds(callback);
  });

  this->discordIface->start(dpp::st_return);
}

void DiscordBot::loggerSet(Logger * l) {
  this->logger = l;
}
}  // namespace arta
