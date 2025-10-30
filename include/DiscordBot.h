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

  std::string     bot_id = "";
  DiscordGuild *  guild = NULL;

  dpp::cluster    * discord_iface = NULL;
  dpp::snowflake  guild_id = 0;

 public:
  DiscordBot(void);

  bool init_complete = false;

  void messageCb(std::string * message) override;

  /**
   * @brief Initiaze the bot
   *
   * @param token Thi discord authentication token
   */
  void init(std::string token, const std::string & id);

  void initialize_guild(dpp::snowflake);

  void channel_added_callback(const dpp::channel_create_t & channel);
  void channel_deleted_callback(const dpp::channel_delete_t & channel);
  void channel_discovered_callback(const dpp::channel & channel);
  void channel_updated_callback(const dpp::channel_update_t & channel);

  /**
   * @brief set the logger object
   *
   * @param logger The logger object
   */
  void set_logger(Logger * l);

  /**
   * @brief Initialize commands
   ()
   */
  void slash_commands_init();

  void ChildChannelCreateDelayed(std::string channel, std::string parent);
  void MessagePostDelayed(std::string channel, std::string parent, std::string message);

  /**
   * @brief handle commands
   ()
   */
  void slash_commands_handle(const dpp::slashcommand_t & event);

  /**
   * @brief Create a channel
   *
   * @param guild_id The server where teh category will be created
   * @param name The category name
   */
  void category_create(dpp::snowflake guild_id,
                       dpp::snowflake parent_id,
                       std::string name);

  /**
   * @brief Create a channel
   *
   * @param guild_id The server where teh channel will be created
   * @param name The channel name
   */
  void channel_create(dpp::snowflake parent_id,
                      const std::string & name,
                      dpp::channel_type chanType);

  /**
   * @brief Delete a channel
   *
   * @param name The channel name
   */
  void channel_delete(const std::string & name);

  void channel_rename(const std::string & name, std::string new_name);

  void slash_commands_handle_channel_create(const dpp::slashcommand_t & event);
  void slash_commands_handle_channel_delete(const dpp::slashcommand_t & event);
  void slash_commands_handle_channel_rename(const dpp::slashcommand_t & event);

  void system_create(const std::string & name);
  void planet_create(const std::string & name, const std::string & system);

  void system_delete(const std::string & name);
  void planet_delete(const std::string & name);

  void slash_commands_handle_system_create(const dpp::slashcommand_t & event);
  void slash_commands_handle_system_delete(const dpp::slashcommand_t & event);
  void slash_commands_handle_planet_create(const dpp::slashcommand_t & event);
  void slash_commands_handle_planet_delete(const dpp::slashcommand_t & event);

  void register_guild_command(std::vector<std::string> command);

    /**
     * @brief Getter function for the bot id
     *
     * @return The bot id
     */
    std::string bot_id_get(void);

  /**
   * @brief Post a message to a channel
   *
   * @param channel The channel to post in
   * @param message The text to post
   *
   * @return The bot id
   */
  void post_message(const std::string & channel_name, const std::string & text);

  dpp::command_completion_event_t user_get_guilds_callback(dpp::confirmation_callback_t value);
  dpp::command_completion_event_t channels_get_callback(dpp::confirmation_callback_t value);
  dpp::command_completion_event_t delete_global_commands(dpp::confirmation_callback_t value);
  dpp::command_completion_event_t update_guild_commands(dpp::confirmation_callback_t value);
  dpp::command_completion_event_t channels_create_cb(dpp::confirmation_callback_t value);
};
}  // namespace arta

#endif  // INCLUDE_DISCORDBOT_H_
