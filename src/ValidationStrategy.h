/**
 * @file ValidationStrategy.h
 * @brief Strategy pattern for user-type-specific message validation
 * @author Kyle McCalgan
 * @date 2025-09-28
 */

#ifndef VALIDATIONSTRATEGY_H
#define VALIDATIONSTRATEGY_H

#include <string>

/**
 * @class ValidationStrategy
 * @brief Abstract base class for message validation strategies
 */
class ValidationStrategy {
public:
    virtual ~ValidationStrategy() = default;
    
    /**
     * @brief Validates a message according to strategy rules
     * @param message The message to validate
     * @param userName The user sending the message
     * @return true if valid, false otherwise
     */
    virtual bool validateMessage(const std::string& message, const std::string& userName) = 0;
    
    /**
     * @brief Gets the strategy name
     * @return Strategy identifier string
     */
    virtual std::string getStrategyName() const = 0;
    
    /**
     * @brief Gets maximum message length
     * @return Max length in characters, -1 for unlimited
     */
    virtual int getMaxMessageLength() const = 0;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @class FreeUserValidationStrategy
 * @brief Strict validation for free users (100 char limit, no profanity)
 */
class FreeUserValidationStrategy : public ValidationStrategy {
public:
    bool validateMessage(const std::string& message, const std::string& userName) override;
    std::string getStrategyName() const override { return "Free User"; }
    int getMaxMessageLength() const override { return 100; }

private:
    static const int MAX_FREE_MESSAGE_LENGTH = 100;
    bool containsAnyProfanity(const std::string& message) const;
    bool hasExcessiveCaps(const std::string& message) const;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @class PremiumUserValidationStrategy
 * @brief Moderate validation for premium users (unlimited length, mild profanity allowed)
 */
class PremiumUserValidationStrategy : public ValidationStrategy {
public:
    bool validateMessage(const std::string& message, const std::string& userName) override;
    std::string getStrategyName() const override { return "Premium User"; }
    int getMaxMessageLength() const override { return -1; }

private:
    bool containsSevereProfanity(const std::string& message) const;
    bool isExcessiveSpam(const std::string& message) const;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @class AdminUserValidationStrategy
 * @brief Minimal validation for admins (2000 char limit, only blocks system threats)
 */
class AdminUserValidationStrategy : public ValidationStrategy {
public:
    bool validateMessage(const std::string& message, const std::string& userName) override;
    std::string getStrategyName() const override { return "Admin User"; }
    int getMaxMessageLength() const override { return 2000; }

private:
    static const int MAX_ADMIN_MESSAGE_LENGTH = 2000;
    bool containsSystemThreats(const std::string& message) const;
};

#endif // VALIDATIONSTRATEGY_H