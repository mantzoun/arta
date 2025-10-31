/*
 * Copyright 2025 Stavros Mantzouneas
 */
#include "include/DiscordBot.h"

static std::vector<std::string> deleteCommands = {
    "location_create", "location_delete"
};

static std::vector<std::string> existingCommands = {};

static std::vector<std::vector<std::string>> guildCommands = {
    {"channel_create", "Create a new channel",
        "string", "name", "the channel name", "true",
        "string", "parent", "the channel parent", "false",
        "string_choice", "type", "the channel type", "false",
            "choice", "Text", "TextChannel",
            "choice", "Category", "Category",
    },
    {"channel_delete", "Delete a channel",
        "string", "name", "the channel name", "true",
    },
    {"channel_rename", "Rename a channel",
        "string", "name", "the channel name", "true",
        "string", "new_name", "the new name", "true",
    },
    {"system_create", "Add a system",
        "string", "name", "the system name", "true",
    },
    {"system_delete", "Delete a system",
        "string", "name", "the system name", "true",
    },
    {"planet_create", "Add a planet",
        "string", "name", "the planet name", "true",
        "string", "system", "the parent system name", "true",
    },
    {"planet_delete", "Delete a planet",
        "string", "name", "the planet name", "true",
    },
};

namespace arta {
dpp::command_completion_event_t DiscordBot::updateGuildCommands(dpp::confirmation_callback_t value) {
  this->discordIface->log(dpp::ll_debug, "Guild Command cleanup Callback");
  if ( value.is_error() == true ) {
    dpp::error_info err = value.get_error();
    this->discordIface->log(dpp::ll_error, "Error " + err.message);
  }

  dpp::slashcommand_map map = std::get<dpp::slashcommand_map>(value.value);

  for (const auto& it : map) {
    dpp::snowflake id = it.first;
    dpp::slashcommand command = it.second;

    existingCommands.push_back(command.name);

    bool found = std::find(deleteCommands.begin(),
                           deleteCommands.end(),
                           command.name) != deleteCommands.end();

    if (!found) {
      logger->debug("Skipping delete command " + command.name);
      continue;
    }

    this->discordIface->log(dpp::ll_debug, "Delete " + command.name);

    this->discordIface->guild_command_delete(id, this->guildId);
  }

  for (const auto& command : guildCommands) {
    bool found = std::find(existingCommands.begin(),
                           existingCommands.end(),
                           command[0]) != existingCommands.end();

    if (found) {
      logger->debug("Skipping create command " + command[0]);
      continue;
    }

    registerGuildCommand(command);
  }

  return NULL;
}

dpp::command_completion_event_t DiscordBot::deleteGlobalCommands(dpp::confirmation_callback_t value) {
  this->discordIface->log(dpp::ll_debug, "Global Command cleanup Callback");
  if (value.is_error() == true) {
    dpp::error_info err = value.get_error();
    this->discordIface->log(dpp::ll_error, "Error " + err.message);
  }

  dpp::slashcommand_map map = std::get<dpp::slashcommand_map>(value.value);

  for (const auto& it : map) {
    dpp::snowflake id = it.first;
    dpp::slashcommand command = it.second;

    bool found = std::find(deleteCommands.begin(),
                           deleteCommands.end(),
                           command.name) != deleteCommands.end();

    if (!found) {
      logger->debug("Skipping command " + command.name);
      continue;
    }

    this->discordIface->log(dpp::ll_debug, "Delete " + command.name);
    this->discordIface->global_command_delete(id, NULL);
  }

  return NULL;
}

void DiscordBot::registerGuildCommand(std::vector<std::string> command) {
  dpp::slashcommand cmd = dpp::slashcommand(command[0],
                                            command[1],
                                            this->discordIface->me.id);
  dpp::command_option option;

  size_t i = 2;
  while (i < command.size()) {
    if (command[i] == "string") {
      option = dpp::command_option(dpp::co_string,
                                    command[i+1],
                                    command[i+2],
                                    command[i+3] == "true");
      i += 4;
    } else if (command[i] == "string_choice") {
      option = dpp::command_option(dpp::co_string,
                                    command[i+1],
                                    command[i+2],
                                    command[i+3] == "true");
      i += 4;

      while (i < command.size() && command[i] == "choice") {
        dpp::command_option_choice choice = dpp::command_option_choice(
                                                    command[i+1],
                                                    command[i+2]);
        option.add_choice(choice);
        i += 3;
      }
    } else {
      this->discordIface->log(dpp::ll_error,
                                "Error type " + command[i] +
                                " in command " + command[0]);
      break;
    }

    cmd.add_option(option);
  }

  this->discordIface->log(dpp::ll_debug, "Register " + cmd.name);
  this->discordIface->guild_command_create(cmd, this->guildId);
}

void DiscordBot::slashCommandsInit() {
  this->discordIface->log(dpp::ll_debug, "Register slash commands");

  std::function<void(const dpp::confirmation_callback_t&)> callback =
  std::bind(&DiscordBot::updateGuildCommands, this, std::placeholders::_1);
  this->discordIface->guild_commands_get(guildId, callback);

  callback = std::bind(&DiscordBot::deleteGlobalCommands,
                            this, std::placeholders::_1);
  this->discordIface->global_commands_get(callback);
}

void DiscordBot::slashCommandsHandle(const dpp::slashcommand_t & event) {
  /* Check which command they ran */
  this->discordIface->log(dpp::ll_warning,
                            "COMMAND: " + event.command.get_command_name());
  if (event.command.get_command_name() == "channel_create") {
    event.reply("command received");

    this->slashCommandsHandleChannelCreate(event);
  } else if (event.command.get_command_name() == "channel_delete") {
    event.reply("command received");

    this->slashCommandsHandleChannelDelete(event);
  } else if (event.command.get_command_name() == "channel_rename") {
    event.reply("command received");

    this->slashCommandsHandleChannelRename(event);
  } else if (event.command.get_command_name() == "system_create") {
    event.reply("command received");

    this->slashCommandsHandleSystemCreate(event);
  } else if (event.command.get_command_name() == "system_delete") {
    event.reply("command received");

    this->slashCommandsHandleSystemDelete(event);
  } else if (event.command.get_command_name() == "planet_create") {
    event.reply("command received");

    this->slashCommandsHandlePlanetCreate(event);
  } else if (event.command.get_command_name() == "planet_delete") {
    event.reply("command received");

    this->slashCommandsHandlePlanetDelete(event);
  }
}

void DiscordBot::systemCreate(const std::string& name) {
}

void DiscordBot::planetCreate(const std::string& name,
                               const std::string& system) {
}

void DiscordBot::systemDelete(const std::string& name) {
  this->logger->debug("System delete");
}

void DiscordBot::planetDelete(const std::string& name) {
  this->logger->debug("Planet delete");
}

void DiscordBot::slashCommandsHandleSystemCreate(
                        const dpp::slashcommand_t & event) {
  std::string name = std::get<std::string>(event.get_parameter("name"));

  this->systemCreate(name);
}

void DiscordBot::slashCommandsHandleSystemDelete(
                        const dpp::slashcommand_t & event) {
  std::string name = std::get<std::string>(event.get_parameter("name"));

  this->systemDelete(name);
}

void DiscordBot::slashCommandsHandlePlanetCreate(
                        const dpp::slashcommand_t & event) {
  std::string name = std::get<std::string>(event.get_parameter("name"));
  std::string system = std::get<std::string>(event.get_parameter("system"));

  this->planetCreate(name, system);
}

void DiscordBot::slashCommandsHandlePlanetDelete(
                        const dpp::slashcommand_t & event) {
  std::string name = std::get<std::string>(event.get_parameter("name"));

  this->planetDelete(name);
}

void DiscordBot::slashCommandsHandleChannelDelete(
                        const dpp::slashcommand_t & event) {
  std::string name = std::get<std::string>(event.get_parameter("name"));

  this->channelDelete(name);
  // FIXME parents staff in channel hierarchy
}

void DiscordBot::slashCommandsHandleChannelRename(
                        const dpp::slashcommand_t & event) {
  std::string name = std::get<std::string>(event.get_parameter("name"));
  std::string new_name = std::get<std::string>(event.get_parameter("new_name"));

  this->channelRename(name, new_name);
  // FIXME parents staff in channel hierarchy
}

void DiscordBot::slashCommandsHandleChannelCreate(
                        const dpp::slashcommand_t & event) {
  std::string name = std::get<std::string>(event.get_parameter("name"));

  std::string type = "";
  try {
    type = std::get<std::string>(event.get_parameter("type"));
  } catch (const std::bad_variant_access& e) {
  }

  std::string parent = "";
  try {
    parent = std::get<std::string>(event.get_parameter("parent"));
  } catch (const std::bad_variant_access& e) {
  }

  uint64_t parentId = 0;

  this->logger->info(name + " # " + type + " # " + parent);

  if (parent != "") {
    std::list<DiscordChannel *> parents = this->guild->channelGet(parent);

    if (parents.size() == 0) {
      this->logger->warn("channel parent not found " + parent);
      return;
    }

    if (parents.size() > 1) {
      this->logger->warn("multiple parents found " + parent);
      return;
    }

    parentId = parents.front()->idGet();
  }

  dpp::channel_type chanType = dpp::CHANNEL_TEXT;

  if (type == "Category") {
    chanType = dpp::CHANNEL_CATEGORY;
  }

  this->channelCreate(parentId, name, chanType);
}
}  // namespace artaS
