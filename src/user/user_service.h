#ifndef USER_SERVICE_H
#define USER_SERVICE_H

#include "user_repo.h"
#include <string>

class UserService {
public:
    UserService();

    std::string register_user(const std::string& code, const std::string& telegram_username);

private:
    UserRepo m_repo;

    std::string decrypt_code(const std::string& code);
};

#endif
