#ifndef USER_REPO_H
#define USER_REPO_H

#include "../db.h"
#include <string>
#include <vector>

class UserRepo {
public:
    UserRepo();

    void add_telegram_username(const std::string& tmrw_username, const std::string& telegram_username);

    std::vector<std::string> get_unregistered();

private:
    Db& m_db;
};

#endif
