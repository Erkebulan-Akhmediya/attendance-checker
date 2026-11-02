#ifndef USER_REPO_H
#define USER_REPO_H

#include "../db.h"
#include <string>

class UserRepo {
public:
    UserRepo();

    void add_telegram_username(const std::string& tmrw_username, const std::string& telegram_username);

private:
    Db& m_db;
};

#endif
