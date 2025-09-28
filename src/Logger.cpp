/**
 * @file Logger.cpp
 * @brief Implementation of static members for Logger class
 * @author Megan Azmanov & Kyle McCalgan  
 * @date 2025-09-28
 */

#include "Logger.h"

// Initialize static member - default to USER_ONLY for clean experience
LogLevel Logger::currentLevel = DEBUG;