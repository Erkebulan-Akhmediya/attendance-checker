#include "user_service.h"
#include <string>

UserService::UserService()
    : m_repo()
{
}

std::string UserService::register_user(const std::string& code, const std::string& telegram_username)
{
    fprintf(stderr, "%s\n", std::string(telegram_username).c_str());
    std::string tmrw_username = decrypt_code(code);
    m_repo.add_telegram_username(tmrw_username, telegram_username);
    return tmrw_username;
}

std::string UserService::decrypt_code(const std::string& code)
{
    return std::string(code);
}
