/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * DiscordBot.h
 *
 * handle connection with the Discord servers
 * uses the DPP library
 * https://dpp.dev/
 */

#ifndef INCLUDE_DISCORDBOT_H_
#define INCLUDE_DISCORDBOT_H_

#include <stdio.h>
#include <string>
#include <map>
#include <mutex>
#include <vector>

#include "dpp/dpp.h"
#include "dpp/restresults.h"
#include "dpp/guild.h"

#include "include/Logger.h"
#include "include/DiscordGuild.h"
#include "include/DiscordChannel.h"
#include "include/MessageConsumer.h"

namespace arta {
/**
 * @class DiscordBot
 *
 * @brief The bot will run on the DPP library to connect to
 *        discord, and offer a callback function for message
 *        posting
 */
class DiscordBot : public MessageConsumer {
 private:
  Logger *        logger = NULL;

  std::string     botId = "";
  DiscordGuild *  guild = NULL;

  dpp::cluster    * discordIface = NULL;
  dpp::snowflake  guildId = 0;

 public:
  DiscordBot(void);

  bool initComplete = false;

  void messageCb(std::string message) override;

  /**
   * @brief Initiaze the bot
   *
   * @param token Thi discord authentication token
   */
  void init(std::string token, const std::string & id);

  void initializeGuild(dpp::snowflake);

  void channelAddedCallback(const dpp::channel_create_t & channel);
  void channelDeletedCallback(const dpp::channel_delete_t & channel);
  void channelDiscoveredCallback(const dpp::channel & channel);
  void channelUpdatedCallback(const dpp::channel_update_t & channel);

  /**
   * @brief set the logger object
   *
   * @param logger The logger object
   */
  void loggerSet(Logger * l);

  /**
   * @brief Initialize commands
   ()
   */
  void slashCommandsInit();

  void ChildChannelCreateDelayed(std::string channel, std::string parent);
  void MessagePostDelayed(std::string channel, std::string parent, std::string message);

  /**
   * @brief handle commands
   ()
   */
  void slashCommandsHandle(const dpp::slashcommand_t & event);

  /**
   * @brief Create a channel
   *
   * @param guildId The server where teh category will be created
   * @param name The category name
   */
  void categoryCreate(dpp::snowflake guildId,
                       dpp::snowflake parentId,
                       std::string name);

  /**
   * @brief Create a channel
   *
   * @param guildId The server where teh channel will be created
   * @param name The channel name
   */
  void channelCreate(dpp::snowflake parentId,
                      const std::string & name,
                      dpp::channel_type chanType);

  /**
   * @brief Delete a channel
   *
   * @param name The channel name
   */
  void channelDelete(const std::string & name);

  void channelRename(const std::string & name, std::string new_name);

  void slashCommandsHandleChannelCreate(const dpp::slashcommand_t & event);
  void slashCommandsHandleChannelDelete(const dpp::slashcommand_t & event);
  void slashCommandsHandleChannelRename(const dpp::slashcommand_t & event);

  void systemCreate(const std::string & name);
  void planetCreate(const std::string & name, const std::string & system);

  void systemDelete(const std::string & name);
  void planetDelete(const std::string & name);

  void slashCommandsHandleSystemCreate(const dpp::slashcommand_t & event);
  void slashCommandsHandleSystemDelete(const dpp::slashcommand_t & event);
  void slashCommandsHandlePlanetCreate(const dpp::slashcommand_t & event);
  void slashCommandsHandlePlanetDelete(const dpp::slashcommand_t & event);

  void registerGuildCommand(std::vector<std::string> command);

    /**
     * @brief Getter function for the bot id
     *
     * @return The bot id
     */
    std::string botIdGet(void);

  /**
   * @brief Post a message to a channel
   *
   * @param channel The channel to post in
   * @param message The text to post
   *
   * @return The bot id
   */
  void postMessage(const std::string & channel_name, const std::string & text);

  dpp::command_completion_event_t userGetGuildsCallback(dpp::confirmation_callback_t value);
  dpp::command_completion_event_t channelsGetCallback(dpp::confirmation_callback_t value);
  dpp::command_completion_event_t deleteGlobalCommands(dpp::confirmation_callback_t value);
  dpp::command_completion_event_t updateGuildCommands(dpp::confirmation_callback_t value);
  dpp::command_completion_event_t channelsCreateCb(dpp::confirmation_callback_t value);
};
}  // namespace arta

#endif  // INCLUDE_DISCORDBOT_H_
