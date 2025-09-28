/**
 * @file User.h
 * @brief Defines the User base class and derived user types
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
 * @enum UserType
 * @brief Enumeration for different user types
 */
enum class UserType {
    FREE,
    PREMIUM,
    ADMIN
};

/**
 * @class User
 * @brief Abstract base class for users in the chat system
 * 
 * User acts as both a Colleague in the Mediator pattern, communicates through chatroom mediator
 * and an Invoker in the Command pattern, creates and executes commands
 */
class User {
protected:
    std::string name;
    std::vector<ChatRoom*> chatRooms;
    std::vector<Command*> commandQueue;
    UserType userType;

public:
    /**
     * @brief Constructor
     * @param userName The user's name
     * @param type The type of user
     */
    User(std::string userName, UserType type);
    
    /**
     * @brief Virtual destructor for proper cleanup
     */
    virtual ~User();
    
    /**
     * @brief Get user's name
     * @return The user's name
     */
    std::string getName() const;
    
    /**
     * @brief Get user's type
     * @return The user's type
     */
    UserType getUserType() const;
    
    /**
     * @brief Get user type as string for display
     * @return String representation of user type
     */
    std::string getUserTypeString() const;

    // Mediator pattern functions ----------------------------------

    /**
     * @brief Send a message to a chat room (virtual for different user behavior)
     * @param message The message to send
     * @param room The chat room to send to
     * @return true if message was sent successfully, false if blocked by limits
     */
    virtual bool send(std::string message, ChatRoom* room) = 0;
    
    /**
     * @brief Receive a message from another user
     * @param message The message content
     * @param fromUser The user who sent it
     * @param room The chat room it came from
     */
    virtual void receive(std::string message, User* fromUser, ChatRoom* room);
    
    // Command pattern methods ---------------------------------
    
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

protected:
    /**
     * @brief Helper method to actually send the message (used by derived classes)
     * @param message The message to send
     * @param room The chat room to send to
     */
    void performSend(std::string message, ChatRoom* room);
};

/**
 * @class FreeUser
 * @brief Free user with daily message limits
 */
class FreeUser : public User {
private:
    static const int DAILY_MESSAGE_LIMIT = 10;
    int dailyMessageCount;

public:
    /**
     * @brief Constructor for free user
     * @param userName The user's name
     */
    FreeUser(std::string userName);
    
    /**
     * @brief Send message with daily limit check
     * @param message The message to send
     * @param room The chat room to send to
     * @return true if sent, false if daily limit exceeded
     */
    bool send(std::string message, ChatRoom* room) override;
    
    /**
     * @brief Reset daily message count (would be called daily in real system)
     */
    void resetDailyCount();
    
    /**
     * @brief Get current daily message count
     * @return Number of messages sent today
     */
    int getDailyMessageCount() const;
    
    /**
     * @brief Get daily message limit
     * @return Daily message limit
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
     * @brief Constructor for premium user
     * @param userName The user's name
     */
    PremiumUser(std::string userName);
    
    /**
     * @brief Send message without limits
     * @param message The message to send
     * @param room The chat room to send to
     * @return Always true for premium users
     */
    bool send(std::string message, ChatRoom* room) override;
};

/**
 * @class AdminUser
 * @brief Admin user with special privileges
 */
class AdminUser : public User {
public:
    /**
     * @brief Constructor for admin user
     * @param userName The user's name
     */
    AdminUser(std::string userName);
    
    /**
     * @brief Send message without limits (admin privilege)
     * @param message The message to send
     * @param room The chat room to send to
     * @return Always true for admin users
     */
    bool send(std::string message, ChatRoom* room) override;
    
    /**
     * @brief Admin-specific receive method with additional logging
     * @param message The message content
     * @param fromUser The user who sent it
     * @param room The chat room it came from
     */
    void receive(std::string message, User* fromUser, ChatRoom* room) override;
};

#endif