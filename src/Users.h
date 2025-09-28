/**
 * @file Users.h
 * @brief Declaration of User classes with Mediator Colleague and Command Invoker roles
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-28
 */

#ifndef USERS_H
#define USERS_H

#include <string>
#include <vector>
#include <iostream>

// Forward declarations
class ChatRoom;
class Command;
class Iterator;

/**
 * @enum UserType
 * @brief Defines the different types of users in the system
 */
enum class UserType {
    FREE,    ///< Free user with message limitations
    PREMIUM, ///< Premium user with unlimited messaging
    ADMIN    ///< Admin user with administrative privileges
};

/**
 * @class User
 * @brief Base class for all users in the chat system
 * 
 * Implements the Colleague role in the Mediator pattern and 
 * the Invoker role in the Command pattern.
 */
class User {
protected:
    std::string name;                        ///< User's display name
    UserType userType;                       ///< Type of user (Free, Premium, Admin)
    std::vector<ChatRoom*> chatRooms;        ///< Chat rooms the user is registered in
    std::vector<Command*> commandQueue;      ///< Queue of commands to execute

    /**
     * @brief Internal method to perform the actual message sending
     * @param message The message content
     * @param room The chat room to send the message to
     */
    void performSend(std::string message, ChatRoom* room);

public:
    /**
     * @brief Constructor for User
     * @param userName The user's name
     * @param type The type of user
     */
    User(std::string userName, UserType type);

    /**
     * @brief Virtual destructor
     */
    virtual ~User();

    /**
     * @brief Get the user's name
     * @return The user's name
     */
    std::string getName() const;

    /**
     * @brief Get the user's type
     * @return The user's type
     */
    UserType getUserType() const;

    /**
     * @brief Get the user's type as a string
     * @return String representation of user type
     */
    std::string getUserTypeString() const;

    /**
     * @brief Generate debug information about the user
     * @return String containing all user information for debugging
     */
    virtual std::string toString() const;

    /**
     * @brief Send a message to a chat room (pure virtual)
     * @param message The message to send
     * @param room The chat room to send to
     * @return true if message was sent successfully, false otherwise
     */
    virtual bool send(std::string message, ChatRoom* room) = 0;

    /**
     * @brief Receive a message from another user
     * @param message The message content
     * @param fromUser The user who sent the message
     * @param room The chat room the message was sent in
     */
    virtual void receive(std::string message, User* fromUser, ChatRoom* room);

    /**
     * @brief Add a command to the command queue
     * @param command Command to add
     */
    void addCommand(Command* command);

    /**
     * @brief Execute all queued commands
     */
    void executeAll();

    /**
     * @brief Add a chat room to the user's room list
     * @param room Chat room to add
     */
    void addChatRoom(ChatRoom* room);

    /**
     * @brief Remove a chat room from the user's room list
     * @param room Chat room to remove
     */
    void removeChatRoom(ChatRoom* room);

    /**
     * @brief Check if user is registered in a specific chat room
     * @param room Chat room to check
     * @return true if user is in the room, false otherwise
     */
    bool isInChatRoom(ChatRoom* room) const;

    /**
     * @brief Request an iterator for chat history (virtual - overridden by AdminUser)
     * @param room The chat room to get history for
     * @return Iterator pointer or nullptr if access denied
     */
    virtual Iterator* requestChatHistoryIterator(ChatRoom* room) { 
        (void)room; // Suppress unused parameter warning
        return nullptr;  // Non-admins get nullptr
    }
    
    /**
     * @brief Iterate through chat history (virtual - overridden by AdminUser)
     * @param room The chat room to iterate through
     */
    virtual void iterateChatHistory(ChatRoom* room) {
        (void)room; // Suppress unused parameter warning
        std::cout << "Access denied - admin privileges required" << std::endl;
    }
    
    /**
     * @brief Check if user has admin privileges
     * @return true if user is admin, false otherwise
     */
    virtual bool hasAdminPrivileges() const { 
        return getUserType() == UserType::ADMIN; 
    }
};

/**
 * @class FreeUser
 * @brief Free user with daily message limitations
 */
class FreeUser : public User {
private:
    static const int DAILY_MESSAGE_LIMIT = 5; ///< Maximum messages per day
    int dailyMessageCount;                     ///< Current daily message count

public:
    /**
     * @brief Constructor for FreeUser
     * @param userName The user's name
     */
    FreeUser(std::string userName);

    /**
     * @brief Generate debug information specific to free users
     * @return String containing free user debug information
     */
    virtual std::string toString() const override;

    /**
     * @brief Send a message (with daily limit check)
     * @param message The message to send
     * @param room The chat room to send to
     * @return true if message was sent successfully, false if limit reached
     */
    virtual bool send(std::string message, ChatRoom* room) override;

    /**
     * @brief Reset the daily message count
     */
    void resetDailyCount();

    /**
     * @brief Get current daily message count
     * @return Number of messages sent today
     */
    int getDailyMessageCount() const;

    /**
     * @brief Get daily message limit
     * @return Maximum messages allowed per day
     */
    int getDailyMessageLimit() const;
};

/**
 * @class PremiumUser
 * @brief Premium user with unlimited messaging
 */
class PremiumUser : public User {
public:
    /**
     * @brief Constructor for PremiumUser
     * @param userName The user's name
     */
    PremiumUser(std::string userName);

    /**
     * @brief Generate debug information specific to premium users
     * @return String containing premium user debug information
     */
    virtual std::string toString() const override;

    /**
     * @brief Send a message (unlimited)
     * @param message The message to send
     * @param room The chat room to send to
     * @return true if message was sent successfully
     */
    virtual bool send(std::string message, ChatRoom* room) override;
};

/**
 * @class AdminUser
 * @brief Admin user with administrative privileges
 */
class AdminUser : public User {
public:
    /**
     * @brief Constructor for AdminUser
     * @param userName The user's name
     */
    AdminUser(std::string userName);

    /**
     * @brief Generate debug information specific to admin users
     * @return String containing admin user debug information
     */
    virtual std::string toString() const override;

    /**
     * @brief Send a message with admin privileges
     * @param message The message to send
     * @param room The chat room to send to
     * @return true if message was sent successfully
     */
    virtual bool send(std::string message, ChatRoom* room) override;

    /**
     * @brief Receive a message with additional admin logging
     * @param message The message content
     * @param fromUser The user who sent the message
     * @param room The chat room the message was sent in
     */
    virtual void receive(std::string message, User* fromUser, ChatRoom* room) override;

    /**
     * @brief Override to provide admin access to chat history iterator
     * @param room The chat room to get history for
     * @return Iterator pointer for admin access
     */
    virtual Iterator* requestChatHistoryIterator(ChatRoom* room) override;

    /**
     * @brief Iterate through chat history (admin only feature)
     * @param room The chat room to review history for
     */
    virtual void iterateChatHistory(ChatRoom* room) override;
};

#endif // USERS_H