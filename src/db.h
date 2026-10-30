#ifndef DB_H
#define DB_H

#include <memory>
#include <sqlite3.h>

class Db {
public:
    Db();

private:
    std::unique_ptr<sqlite3, decltype(&sqlite3_close)> m_db;
};

#endif
