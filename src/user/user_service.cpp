#include "user_service.h"
#include <string>

UserService::UserService()
    : m_repo()
    , m_crypto_service()
{
}

std::string UserService::register_user(const std::string& code, const std::string& telegram_username)
{
    std::string tmrw_username = m_crypto_service.decrypt(code);
    m_repo.add_telegram_username(tmrw_username, telegram_username);
    return tmrw_username;
}
