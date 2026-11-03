#include "user_repo.h"
#include "user_exceptions.h"
#include <stdexcept>
#include <string>
#include <vector>

UserRepo::UserRepo()
    : m_db(Db::get_instance())
{
}

void UserRepo::add_telegram_username(const std::string& tmrw_username, const std::string& telegram_username)
{
    constexpr auto query = "update users set telegram_username = ? where tmrw_username = ? and telegram_username is null";
    auto stmt = m_db.make_stmt(query);
    sqlite3_bind_text(stmt.get(), 1, telegram_username.c_str(), telegram_username.size(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt.get(), 2, tmrw_username.c_str(), tmrw_username.size(), SQLITE_TRANSIENT);
    const int res = sqlite3_step(stmt.get());
    if (res != SQLITE_DONE)
        throw std::runtime_error("failed to update telegram username");
    if (m_db.rows_changed() == 0)
        throw User::Exceptions::NoUserChanged();
}

int write_username(void* vector, int, char** col, char**)
{
    auto ptr = reinterpret_cast<std::string*>(vector);
    if (!col)
        return 0;
    *ptr = *col;
    ++ptr;
    return 0;
}

std::vector<std::string> UserRepo::get_unregistered()
{
    constexpr auto query = "select tmrw_username from users where telegram_username is null";
    auto rows = m_db.exec(query);

    std::vector<std::string> usernames;
    for (const auto& row : rows)
        usernames.push_back(std::move(row[0]));
    return usernames;
}
