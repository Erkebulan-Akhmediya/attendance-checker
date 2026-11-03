#ifndef USER_SERVICE_H
#define USER_SERVICE_H

#include "../crypto.h"
#include "user_repo.h"
#include <string>

class UserService {
public:
    UserService();

    std::string register_user(const std::string& code, const std::string& telegram_username);

private:
    UserRepo m_repo;
    Crypto m_crypto_service;
};

#endif
