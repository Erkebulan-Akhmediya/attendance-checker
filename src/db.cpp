#include "db.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

Db* Db::m_instance;

Db& Db::get_instance()
{
    if (!m_instance)
        m_instance = new Db();
    return *m_instance;
}

Db::Db()
    : m_db(nullptr, &sqlite3_close)
{
    sqlite3* db = nullptr;
    const int res = sqlite3_open(std::getenv("DB_FILE"), &db);
    if (res != SQLITE_OK)
        throw std::runtime_error("failed to open database connection");
    std::cout << "successfully connected to database (" << std::getenv("DB_FILE") << ")\n";
    m_db.reset(db);
}

std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> Db::make_stmt(const std::string& query)
{
    sqlite3_stmt* raw_stmt = nullptr;
    sqlite3_prepare_v2(m_db.get(), query.c_str(), -1, &raw_stmt, nullptr);
    return { raw_stmt, &sqlite3_finalize };
}
