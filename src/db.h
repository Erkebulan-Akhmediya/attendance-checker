#ifndef DB_H
#define DB_H

#include <memory>
#include <sqlite3.h>
#include <string>
#include <vector>

class Db {
public:
    static Db& get_instance();

    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> make_stmt(const std::string& query);

    int rows_changed();

    std::vector<std::vector<std::string>> exec(const std::string&& query);

private:
    static Db* m_instance;

    Db();

    std::unique_ptr<sqlite3, decltype(&sqlite3_close)> m_db;
};

#endif
