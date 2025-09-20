/**
 * @file CtrlCat.h
 * @brief Defines the CtrlCat chat room
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-19
 */

#ifndef CTRLCAT_H
#define CTRLCAT_H

#include "ChatRoom.h"

/**
 * @class CtrlCat
 * @brief Concrete mediator, chat room for cat people
 * It inherits the mediation behavior from ChatRoom and implements
 * the abstract methods.
 */
class CtrlCat : public ChatRoom {
public:
    /**
     * @brief Register a user with CtrlCat room, adds users to user vector
     * @param user Pointer to user to register
     */
    void registerUser(User* user) override;
    
    /**
     * @brief Remove a user from CtrlCat room
     * @param user Pointer to user to remove
     */
    void removeUser(User* user) override;
};

#endif