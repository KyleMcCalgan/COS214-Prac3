/**
 * @file User.h
 * @brief Defines the User class, Colleague in Mediator, Invoker in Command
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-19
 */

#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

class ChatRoom;
class Command;

/**
 * @class User
 * @brief Represents a user in the chat system
 * 
 * User acts as both a Colleague in the Mediator pattern, commuicates through chatroom mediator
 * an Invoker in the Command pattern, creates and executes commands
 */
class User {
private:
    std::string name;
    std::vector<ChatRoom*> chatRooms;//all rooms user belongs too
    std::vector<Command*> commandQueue;

public:
    /**
     * @brief Constructor
     * @param userName The user's name
     */
    User(std::string userName);
    
    /**
     * @brief Destructor will clean up command queue
     */
    ~User();
    
    /**
     * @brief Get user's name
     * @return The user's name
     */
    std::string getName() const;
    

    //Mediator pattern functions ----------------------------------

    /**
     * @brief Send a message to a chat room
     * @param message The message to send
     * @param room The chat room to send to
     * 
     * MEDIATOR PATTERN: User doesn't send to other users directly.
     * Instead, user communicates through the ChatRoom mediator.
     * 
     * COMMAND PATTERN: This method creates SendMessageCommand and 
     * SaveMessageCommand objects and executes them.
     */
    void send(std::string message, ChatRoom* room);
    

    /**
     * @brief Receive a message from another user
     * @param message The message content
     * @param fromUser The user who sent it
     * @param room The chat room it came from
     * 
     * MEDIATOR PATTERN: Called by ChatRoom when another user sends a message.
     * User never calls this directly on another user.
     */
    void receive(std::string message, User* fromUser, ChatRoom* room);
    

      //command pattern methods---------------------------------


      
    /**
     * @brief Add a command to the queue
     * @param command Pointer to command to add
     */
    void addCommand(Command* command);
    
  
    /**
     * @brief Execute all queued commands
     */
    void executeAll();
    
    /**
     * @brief Add a chat room to user's list
     * @param room Pointer to chat room
     */
    void addChatRoom(ChatRoom* room);
};

#endif