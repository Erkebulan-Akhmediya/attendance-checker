#include "user_service.h"
#include <string>
#include <vector>

UserService::UserService()
    : m_repo()
    , m_crypto_service(Crypto::get_instance())
{
}

std::string UserService::register_user(const std::string& code, const std::string& telegram_username)
{
    std::string tmrw_username = m_crypto_service.decrypt(code);
    m_repo.add_telegram_username(tmrw_username, telegram_username);
    return tmrw_username;
}

std::vector<std::string> UserService::get_unregistered()
{
    return m_repo.get_unregistered();
}
