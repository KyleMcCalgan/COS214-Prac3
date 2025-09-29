/**
 * @file ValidationStrategy.cpp
 * @brief User-type-specific validation strategies implementation
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-28
 */

#include "ValidationStrategy.h"
#include "Logger.h"
#include <algorithm>
#include <vector>
#include <cctype>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ================== FreeUserValidationStrategy ==================
//Free users: Short messages, no profanity, no caps abuse
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool FreeUserValidationStrategy::validateMessage(const std::string& message, const std::string& userName) {
    Logger::debug("[FreeUserValidation] Validating message from " + userName);
    
    if (message.empty()) {
        Logger::user(userName + ": Cannot send empty messages");
        return false;
    }

    if (message.length() > MAX_FREE_MESSAGE_LENGTH) {
        Logger::user(userName + ": Message too long! Free users limited to " + 
                    std::to_string(MAX_FREE_MESSAGE_LENGTH) + " characters. Upgrade to Premium for longer messages!");
        return false;
    }

    if (containsAnyProfanity(message)) {
        Logger::user(userName + ": Language not appropriate! Free users must keep messages family-friendly. Upgrade to Premium for more flexibility!");
        return false;
    }

    if (hasExcessiveCaps(message)) {
        Logger::user(userName + ": Please don't use excessive CAPS! Free users must follow basic etiquette rules.");
        return false;
    }
    
    Logger::debug("[FreeUserValidation] Message approved for free user " + userName);
    return true;
}

bool FreeUserValidationStrategy::containsAnyProfanity(const std::string& message) const {
    std::vector<std::string> blockedWords = {
        "stupid", "dumb", "hate", "sucks", "crap", "damn", "hell", 
        "shut", "idiot", "loser", "weird", "ugly", "fat", "shit", "fuck", "bitch", "poes"
    };
    
    std::string lowerMessage = message;
    std::transform(lowerMessage.begin(), lowerMessage.end(), lowerMessage.begin(), ::tolower);
    
    for (const std::string& word : blockedWords) {
        size_t pos = 0;
        while ((pos = lowerMessage.find(word, pos)) != std::string::npos) {
            // Check if it's a whole word (not part of another word)
            bool isWordStart = (pos == 0 || !isalnum(static_cast<unsigned char>(lowerMessage[pos - 1])));
            bool isWordEnd = (pos + word.length() == lowerMessage.length() || 
                             !isalnum(static_cast<unsigned char>(lowerMessage[pos + word.length()])));
            
            if (isWordStart && isWordEnd) {
                Logger::debug("[FreeUserValidation] Blocked word found: " + word);
                return true;
            }
            pos++;
        }
    }
    return false;
}

bool FreeUserValidationStrategy::hasExcessiveCaps(const std::string& message) const {
    if (message.length() < 5) return false;
    
    int capsCount = 0;
    for (char c : message) {
        if (isupper(static_cast<unsigned char>(c))) capsCount++;
    }

    bool excessive = (capsCount > message.length() * 0.3);
    if (excessive) {
        Logger::debug("[FreeUserValidation] Excessive caps detected: " + 
                     std::to_string(capsCount) + "/" + std::to_string(message.length()));
    }
    return excessive;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ================== PremiumUserValidationStrategy ==================
// Premium users: No length limit, but still no severe profanity
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool PremiumUserValidationStrategy::validateMessage(const std::string& message, const std::string& userName) {
    Logger::debug("[PremiumUserValidation] Validating message from premium user " + userName);
    
    if (message.empty()) {
        Logger::user(userName + ": Cannot send empty messages");
        return false;
    }

    Logger::debug("[PremiumUserValidation] Premium user - no length restrictions (" + 
                  std::to_string(message.length()) + " characters)");

    if (containsSevereProfanity(message)) {
        Logger::user(userName + ": That language is too severe! Even Premium users must avoid extreme profanity.");
        return false;
    }

    if (isExcessiveSpam(message)) {
        Logger::user(userName + ": Message appears to be spam. Please send meaningful content!");
        return false;
    }
    
    Logger::debug("[PremiumUserValidation] Message approved for premium user " + userName);
    return true;
}

bool PremiumUserValidationStrategy::containsSevereProfanity(const std::string& message) const {
    std::vector<std::string> severeWords = {
        "fuck", "shit", "bitch", "asshole", "bastard", "whore", "slut"
    };
    
    std::string lowerMessage = message;
    std::transform(lowerMessage.begin(), lowerMessage.end(), lowerMessage.begin(), ::tolower);
    
    for (const std::string& word : severeWords) {
        size_t pos = 0;
        while ((pos = lowerMessage.find(word, pos)) != std::string::npos) {
            // Check if it's a whole word (not part of another word)
            bool isWordStart = (pos == 0 || !isalnum(static_cast<unsigned char>(lowerMessage[pos - 1])));
            bool isWordEnd = (pos + word.length() == lowerMessage.length() || 
                             !isalnum(static_cast<unsigned char>(lowerMessage[pos + word.length()])));
            
            if (isWordStart && isWordEnd) {
                Logger::debug("[PremiumUserValidation] Severe profanity detected: " + word);
                return true;
            }
            pos++;
        }
    }
    return false;
}

bool PremiumUserValidationStrategy::isExcessiveSpam(const std::string& message) const {
    if (message.length() < 10) return false;

    int maxRepeat = 0;
    int currentRepeat = 1;
    
    for (size_t i = 1; i < message.length(); ++i) {
        if (message[i] == message[i-1]) {
            currentRepeat++;
        } else {
            maxRepeat = std::max(maxRepeat, currentRepeat);
            currentRepeat = 1;
        }
    }
    maxRepeat = std::max(maxRepeat, currentRepeat);

    if (maxRepeat > 15) {
        Logger::debug("[PremiumUserValidation] Excessive character repetition: " + std::to_string(maxRepeat));
        return true;
    }

    int capsCount = 0;
    for (char c : message) {
        if (isupper(static_cast<unsigned char>(c))) capsCount++;
    }

    if (capsCount > message.length() * 0.8) {
        Logger::debug("[PremiumUserValidation] All caps spam detected");
        return true;
    }
    
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ================== AdminUserValidationStrategy ==================
// Admin users: Can say almost anything, very high limits
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool AdminUserValidationStrategy::validateMessage(const std::string& message, const std::string& userName) {
    Logger::debug("[AdminUserValidation] Validating message from admin " + userName);
    
    if (message.empty()) {
        Logger::user(userName + ": Cannot send empty messages");
        return false;
    }

    if (message.length() > MAX_ADMIN_MESSAGE_LENGTH) {
        Logger::user(userName + ": Even admin messages have limits! Max " + 
                    std::to_string(MAX_ADMIN_MESSAGE_LENGTH) + " characters for system stability.");
        return false;
    }

    if (containsSystemThreats(message)) {
        Logger::user(userName + ": Admin message blocked - contains potential system threats!");
        return false;
    }
    
    Logger::debug("[AdminUserValidation] Admin message approved - full privileges (" + 
                  std::to_string(message.length()) + " characters)");
    return true;
}

bool AdminUserValidationStrategy::containsSystemThreats(const std::string& message) const {
    std::vector<std::string> threatWords = {
        "DELETE FROM", "DROP TABLE", "rm -rf", "format c:", 
        "shutdown", "reboot", "kill -9", "sudo rm", "del /s"
    };
    
    std::string upperMessage = message;
    std::transform(upperMessage.begin(), upperMessage.end(), upperMessage.begin(), ::toupper);
    
    for (const std::string& threat : threatWords) {
        std::string upperThreat = threat;
        std::transform(upperThreat.begin(), upperThreat.end(), upperThreat.begin(), ::toupper);
        
        if (upperMessage.find(upperThreat) != std::string::npos) {
            Logger::debug("[AdminUserValidation] System threat detected: " + threat);
            return true;
        }
    }
    
    return false;
}