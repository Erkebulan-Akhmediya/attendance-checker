#ifndef DB_H
#define DB_H

#include <memory>
#include <sqlite3.h>
#include <string>

class Db {
public:
    Db();

    void insert_username(const std::string& tmrw_username, const std::string& telegram_username);

private:
    std::unique_ptr<sqlite3, decltype(&sqlite3_close)> m_db;

    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> make_stmt(const std::string& query);
};

#endif
