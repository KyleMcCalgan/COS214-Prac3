/**
 * @file Logger.h
 * @brief Simple logging system to control debug output
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-28
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>

enum LogLevel {
    NONE = 0,      // No logging
    USER_ONLY = 1, // Only user-facing messages (clean chat experience)
    BASIC = 2,     // Basic system operations
    DEBUG = 3      // Full debugging info
};

class Logger {
private:
    static LogLevel currentLevel;

public:
    static void setLevel(LogLevel level) {
        currentLevel = level;
    }
    
    static LogLevel getLevel() {
        return currentLevel;
    }
    
    // Log only essential user messages (clean chat experience)
    static void user(const std::string& message) {
        if (currentLevel >= USER_ONLY) {
            std::cout << message << std::endl;
        }
    }
    
    // Log basic system operations (joins, leaves, etc.)
    static void info(const std::string& message) {
        if (currentLevel >= BASIC) {
            std::cout << message << std::endl;
        }
    }
    
    // Log detailed debugging info (pattern operations, memory management)
    static void debug(const std::string& message) {
        if (currentLevel >= DEBUG) {
            std::cout << message << std::endl;
        }
    }
    
    // Utility methods for common patterns
    static void chatMessage(const std::string& username, const std::string& message) {
        user(username + ": " + message);
    }
    
    static void systemMessage(const std::string& message) {
        info("[SYSTEM] " + message);
    }
};

#endif