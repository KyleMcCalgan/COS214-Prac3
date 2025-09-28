/**
 * @file ValidationStrategy.h
 * @brief Strategy pattern for user-type-specific message validation
 * @author Your Name
 * @date 2025-09-28
 */

#ifndef VALIDATIONSTRATEGY_H
#define VALIDATIONSTRATEGY_H

#include <string>

// Strategy interface
class ValidationStrategy {
public:
    virtual ~ValidationStrategy() = default;
    virtual bool validateMessage(const std::string& message, const std::string& userName) = 0;
    virtual std::string getStrategyName() const = 0;
    virtual int getMaxMessageLength() const = 0;
};

// Concrete Strategy 1: Free User Strategy - Very restrictive
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

// Concrete Strategy 2: Premium User Strategy - Moderate restrictions  
class PremiumUserValidationStrategy : public ValidationStrategy {
public:
    bool validateMessage(const std::string& message, const std::string& userName) override;
    std::string getStrategyName() const override { return "Premium User"; }
    int getMaxMessageLength() const override { return -1; } // No length limit

private:
    bool containsSevereProfanity(const std::string& message) const;
    bool isExcessiveSpam(const std::string& message) const;
};

// Concrete Strategy 3: Admin User Strategy - Minimal restrictions
class AdminUserValidationStrategy : public ValidationStrategy {
public:
    bool validateMessage(const std::string& message, const std::string& userName) override;
    std::string getStrategyName() const override { return "Admin User"; }
    int getMaxMessageLength() const override { return 2000; } // Very high limit

private:
    static const int MAX_ADMIN_MESSAGE_LENGTH = 2000;
    bool containsSystemThreats(const std::string& message) const;
};

#endif // VALIDATIONSTRATEGY_H