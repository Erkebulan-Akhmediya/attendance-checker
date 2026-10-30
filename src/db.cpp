#include "db.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

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

void Db::insert_username(const std::string& tmrw_username, const std::string& telegram_username)
{
    auto stmt = make_stmt("update users set telegram_username = ? where tmrw_username = ?");
    sqlite3_bind_text(stmt.get(), 1, telegram_username.c_str(), telegram_username.size(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt.get(), 2, tmrw_username.c_str(), tmrw_username.size(), SQLITE_TRANSIENT);
    const int res = sqlite3_step(stmt.get());
    if (res != SQLITE_DONE)
        throw std::runtime_error("failed to update telegram username");
}

std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> Db::make_stmt(const std::string& query)
{
    sqlite3_stmt* raw_stmt = nullptr;
    sqlite3_prepare_v2(m_db.get(), query.c_str(), -1, &raw_stmt, nullptr);
    return { raw_stmt, &sqlite3_finalize };
}
