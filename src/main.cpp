#include "tg.h"

#include <cstdio>
#include <exception>
#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include <unistd.h>

const int POLLS_PER_SEC = 1;
const int SLEEP_DURATION = 1000000 / POLLS_PER_SEC;

std::string decrypt_code(std::string_view code)
{
    return std::string(code);
}

std::string reg_user(std::string_view code, std::string_view telegram_username)
{
    fprintf(stderr, "%s\n", std::string(telegram_username).c_str());
    std::string tmrw_username = decrypt_code(code);
    return tmrw_username;
}

void process_msg(const Message& msg)
{
    if (!msg.m_text.starts_with("/start"))
        return;

    const auto payload = std::string_view(msg.m_text).substr(7);

    if (payload[0] == 'r') {
        auto code = payload.substr(1);
        const auto tmrw_username = reg_user(code, msg.m_from);
        constexpr auto reply_template = "Telegram аккаунт {} был успешно привязан к аккаунту Tomorrow School {}";
        const auto reply = std::format(reply_template, msg.m_from, tmrw_username);
        Tg::get_instance().send_message(msg.m_chat_id, reply);
    } else {
        Tg::get_instance().send_message(msg.m_chat_id, std::format("received \"{}\" from {}", msg.m_text, msg.m_from));
    }
}

int main()
{
    auto tg = Tg::get_instance();
    for (; true; usleep(SLEEP_DURATION)) {
        for (const auto& msg : tg.get_messages()) {
            std::cout << msg << '\n';
            try {
                process_msg(msg);
                tg.consume_msg(msg);
            } catch (std::exception& e) {
                std::cout << "failed to process a message: " << e.what() << '\n';
            }
        }
        std::cout << "waiting...\n";
    }
}
