/**
 * @file SendMessageCommand.h
 * @brief Concrete command for sending messages
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-19
 */

#ifndef SENDMESSAGECOMMAND_H
#define SENDMESSAGECOMMAND_H

#include "Command.h"

/**
 * @class SendMessageCommand
 * @brief Command that sends a message to all users in a chat room
 */
class SendMessageCommand : public Command {
public:
    /**
     * @brief Constructor
     * @param room The target chat room
     * @param user The user sending the message
     * @param msg The message content
     */
    SendMessageCommand(ChatRoom* room, User* user, std::string msg);
    
    /**
     * @brief Execute the send message action
     */
    void execute() override;
};

#endif