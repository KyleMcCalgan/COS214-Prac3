/**
 * @file ChatRoom.h
 * @brief Defines the abstract ChatRoom class (Mediator pattern)
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-19
 */


#ifndef CHATROOM_H
#define CHATROOM_H

#include <vector>
#include <string>



class User;


/**
 * @class ChatRoom
 * @brief Abstract mediator that handles communication between users
 * 
 * This class implements the Mediator pattern, managing all communication
 * between users in a chat room without users knowing about each other.
 */


class ChatRoom {
protected:
    std::vector<User*> users;  //         users registered in this room
    std::vector<std::string> chatHistory; //histroy of message sent

public:

    /**
     * @brief Virtual destructor
     */

    virtual ~ChatRoom() {}
    
    /**
     * @brief Register a user with the chat room, abstract
     * @param user Pointer to user to register
     */
    virtual void registerUser(User* user) = 0;
    

    /**
     * @brief Remove a user from the chat room, abstract
     * @param user Pointer to user to remove
     */
    virtual void removeUser(User* user) = 0;
    

    //command pattern functions -----------------------------------------


    /**
     * @brief Send message to all users in the room
     * @param message The message content
     * @param fromUser The user who will send message
     */
    virtual void sendMessage(std::string message, User* fromUser);
    
    /**
     * @brief Save message to chat history
     * @param message The message content
     * @param fromUser The user who sent the message
     */
    virtual void saveMessage(std::string message, User* fromUser);
    
   
};

#endif