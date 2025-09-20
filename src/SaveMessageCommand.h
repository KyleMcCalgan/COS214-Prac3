/**
 * @file SaveMessageCommand.h
 * @brief Concrete command for saving messages
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-19
 */

#ifndef SAVEMESSAGECOMMAND_H
#define SAVEMESSAGECOMMAND_H

#include "Command.h"

/**
 * @class SaveMessageCommand
 * @brief Command that saves a message to chat history
 */
class SaveMessageCommand : public Command {
public:
    /**
     * @brief Constructor
     * @param room The target chat room
     * @param user The user who sent the message
     * @param msg The message content
     */
    SaveMessageCommand(ChatRoom* room, User* user, std::string msg);
    
    /**
     * @brief Execute the save message action
     */
    void execute() override;
};

#endif