/**
 * @file Command.h
 * @brief Defines the abstract Command class
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-19
 */

#ifndef COMMAND_H
#define COMMAND_H

#include <string>

class ChatRoom;
class User;

/**
 * @class Command
 * @brief Abstract command interface for encapsulating actions
 * 
 * COMMAND PATTERN: This is the abstract Command that encapsulates
 * a request as an object. It contains all information needed to
 * perform an action on a receiver (ChatRoom).
 */

class Command {
protected:
    ChatRoom* chatRoom;//reciever where actions will be performed
    User* fromUser;
    std::string message;

public:
    /**
     * @brief Constructor
     * @param room The target chat room
     * @param user The user sending the message
     * @param msg The message content
     * 
     * COMMAND PATTERN: Constructor stores all information needed
     * to execute the command later
     */
    Command(ChatRoom* room, User* user, std::string msg);
    
    /**
     * @brief Virtual destructor
     */
    virtual ~Command() {}
    
    /**
     * @brief Execute the command
     */
    virtual void execute() = 0;
};

#endif