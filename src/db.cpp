#include "db.h"
#include <cstdlib>
#include <stdexcept>

Db::Db()
    : m_db(nullptr, &sqlite3_close)
{
    sqlite3* db;
    const int res = sqlite3_open(std::getenv("DB_FILE"), &db);
    if (res != SQLITE_OK)
        throw std::runtime_error("failed to open database connection");
    m_db.reset(db);
}
