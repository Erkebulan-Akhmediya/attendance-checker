#include "user_repo.h"
#include "user_exceptions.h"
#include <stdexcept>
#include <string>

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
