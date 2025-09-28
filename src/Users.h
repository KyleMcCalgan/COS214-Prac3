/**
 * @file Users.h
 * @brief User class hierarchy with Strategy pattern for validation
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-28
 */

#ifndef USERS_H
#define USERS_H

#include <string>
#include <vector>

// Forward declarations
class ChatRoom;
class Command;
class Iterator;
class ValidationStrategy;

enum class UserType {
    FREE,
    PREMIUM,
    ADMIN
};

/**
 * @brief Base User class - Abstract base for all user types
 * Participates in: Mediator (Colleague), Command (Invoker), Strategy (Context)
 */
class User {
protected:
    std::string name;
    UserType userType;
    std::vector<ChatRoom*> chatRooms;
    std::vector<Command*> commandQueue;
    ValidationStrategy* validationStrategy; ///< Strategy pattern for message validation

public:
    /**
     * @brief Constructor for User
     * @param userName Name of the user
     * @param type Type of user (FREE, PREMIUM, ADMIN)
     */
    User(std::string userName, UserType type);
    
    /**
     * @brief Virtual destructor
     */
    virtual ~User();
    
    // Getters
    std::string getName() const;
    UserType getUserType() const;
    std::string getUserTypeString() const;
    std::string toString() const;
    
    // Mediator pattern methods (Colleague role)
    /**
     * @brief Receive a message from another user via mediator
     * @param message The message content
     * @param fromUser The user who sent the message
     * @param room The chat room where the message was sent
     */
    virtual void receive(std::string message, User* fromUser, ChatRoom* room);
    
    /**
     * @brief Send a message to a chat room (pure virtual - implemented by subclasses)
     * @param message The message to send
     * @param room The chat room to send to
     * @return true if message was sent successfully, false otherwise
     */
    virtual bool send(std::string message, ChatRoom* room) = 0;
    
    // Command pattern methods (Invoker role)
    /**
     * @brief Add a command to the queue
     * @param command Command to add
     */
    void addCommand(Command* command);
    
    /**
     * @brief Execute all queued commands
     */
    void executeAll();
    
    // Chat room management
    void addChatRoom(ChatRoom* room);
    void removeChatRoom(ChatRoom* room);
    bool isInChatRoom(ChatRoom* room) const;
    
    // Strategy pattern methods (Context role)
    /**
     * @brief Set the validation strategy
     * @param strategy New validation strategy
     */
    void setValidationStrategy(ValidationStrategy* strategy);
    
    /**
     * @brief Get current validation strategy
     * @return Current validation strategy
     */
    ValidationStrategy* getValidationStrategy() const;
    
    // Iterator pattern methods (default implementation - overridden by AdminUser)
    /**
     * @brief Request iterator for chat history (only admins get access)
     * @param room Chat room to get iterator for
     * @return Iterator pointer or nullptr if access denied
     */
    virtual Iterator* requestChatHistoryIterator(ChatRoom* room) { return nullptr; }
    
    /**
     * @brief Iterate through chat history (only admins can do this)
     * @param room Chat room to iterate through
     */
    virtual void iterateChatHistory(ChatRoom* room) {}

protected:
    /**
     * @brief Perform the actual send operation (common implementation)
     * @param message Message to send
     * @param room Chat room to send to
     */
    void performSend(std::string message, ChatRoom* room);
    
    /**
     * @brief Validate message using current strategy
     * @param message Message to validate
     * @return true if valid, false if blocked
     */
    bool validateMessage(const std::string& message);
};

/**
 * @brief Free User - Basic functionality with restrictions
 * Strategy: FreeUserValidationStrategy (short messages, no profanity, strict rules)
 */
class FreeUser : public User {
private:
    static const int DAILY_MESSAGE_LIMIT = 10;
    int dailyMessageCount;

public:
    /**
     * @brief Constructor for FreeUser
     * @param userName Name of the free user
     */
    FreeUser(std::string userName);
    
    /**
     * @brief Destructor
     */
    ~FreeUser() = default;
    
    std::string toString() const;
    
    /**
     * @brief Send message with free user restrictions
     * @param message Message to send
     * @param room Chat room to send to
     * @return true if sent successfully, false if blocked
     */
    bool send(std::string message, ChatRoom* room) override;
    
    // Free user specific methods
    void resetDailyCount();
    int getDailyMessageCount() const;
    int getDailyMessageLimit() const;
};

/**
 * @brief Premium User - Enhanced functionality with fewer restrictions
 * Strategy: PremiumUserValidationStrategy (unlimited length, mild language OK)
 */
class PremiumUser : public User {
public:
    /**
     * @brief Constructor for PremiumUser
     * @param userName Name of the premium user
     */
    PremiumUser(std::string userName);
    
    /**
     * @brief Destructor
     */
    ~PremiumUser() = default;
    
    /**
     * @brief Send message with premium user privileges
     * @param message Message to send
     * @param room Chat room to send to
     * @return true if sent successfully, false if blocked
     */
    bool send(std::string message, ChatRoom* room) override;

    std::string toString() const;
};

/**
 * @brief Admin User - Full privileges and moderation capabilities
 * Strategy: AdminUserValidationStrategy (minimal restrictions, moderation needs)
 */
class AdminUser : public User {
public:
    /**
     * @brief Constructor for AdminUser
     * @param userName Name of the admin user
     */
    AdminUser(std::string userName);
    
    /**
     * @brief Destructor
     */
    ~AdminUser() = default;
    
    
    /**
     * @brief Send message with admin privileges
     * @param message Message to send
     * @param room Chat room to send to
     * @return true if sent successfully, false if blocked
     */
    bool send(std::string message, ChatRoom* room) override;
    
    /**
     * @brief Receive message with admin monitoring
     * @param message The message content
     * @param fromUser The user who sent the message
     * @param room The chat room where the message was sent
     */
    void receive(std::string message, User* fromUser, ChatRoom* room) override;
    
    // Iterator pattern methods (admin-only access)
    /**
     * @brief Request iterator for chat history (admin privilege)
     * @param room Chat room to get iterator for
     * @return Iterator pointer for admin use
     */
    Iterator* requestChatHistoryIterator(ChatRoom* room) override;
    
    /**
     * @brief Iterate through and display chat history (admin privilege)
     * @param room Chat room to iterate through
     */
    void iterateChatHistory(ChatRoom* room) override;

    std::string toString() const;
};

#endif // USERS_H