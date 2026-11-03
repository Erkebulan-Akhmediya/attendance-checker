#ifndef USER_SERVICE_H
#define USER_SERVICE_H

#include "../crypto.h"
#include "user_repo.h"
#include <string>

// TODO: make it singleton
class UserService {
public:
    UserService();

    std::string register_user(const std::string& code, const std::string& telegram_username);

    // returns tmrw usernames
    std::vector<std::string> get_unregistered();

private:
    UserRepo m_repo;
    Crypto m_crypto_service;
};

#endif
