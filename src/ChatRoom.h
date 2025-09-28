/**
 * @file ChatRoom.h
 * @brief ChatRoom class header with Iterator pattern support
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-28
 */

#ifndef CHATROOM_H
#define CHATROOM_H

#include "Aggregate.h"
#include "Iterator.h"
#include <string>
#include <vector>

class User; // Forward declaration

/**
 * @brief Abstract ChatRoom class (Mediator and Aggregate)
 * Serves as both mediator for communication and aggregate for chat history iteration
 */
class ChatRoom : public Aggregate {
protected:
    std::vector<User*> users;                    // Users in this chat room
    std::vector<std::string> chatHistory;        // Chat history storage

public:
    virtual ~ChatRoom() = default;

    // MEDIATOR PATTERN METHODS
    virtual void registerUser(User* user) = 0;
    virtual void removeUser(User* user) = 0;
    virtual void sendMessage(std::string message, User* fromUser);
    virtual void saveMessage(std::string message, User* fromUser);

    // ITERATOR PATTERN METHODS
    /**
     * @brief Get chat history for admin access only
     * @param requestingUser The user requesting access
     * @return Pointer to chat history vector if user is admin, nullptr otherwise
     */
    virtual const std::vector<std::string>* getChatHistory(User* requestingUser) const;
    
    /**
     * @brief Create iterator for chat history (admin access only)
     * @param requestingUser The user requesting the iterator
     * @return Iterator pointer if user is admin, nullptr otherwise
     */
    virtual Iterator* createIterator(User* requestingUser);
    
    /**
     * @brief Base createIterator implementation (required by Aggregate)
     * @return Iterator for the full chat history
     */
    virtual Iterator* createIterator() override;
};

#endif // CHATROOM_H