#include "tg.h"

#include <cstdio>
#include <exception>
#include <format>
#include <iostream>
#include <string>
#include <unistd.h>

const int POLLS_PER_SEC = 1;
const int SLEEP_DURATION = 1000000 / POLLS_PER_SEC;

std::string decrypt_code(std::string_view code)
{
    return std::string(code);
}

std::string reg_user(const std::string& code, const std::string& telegram_username)
{
    fprintf(stderr, "%s\n", std::string(telegram_username).c_str());
    std::string tmrw_username = decrypt_code(code);
    return tmrw_username;
}

void process_msg(const Message& msg)
{
    try {
	   	if (!msg.m_text.starts_with("/start") || msg.m_text.size() == 6)
	        return;

	    const std::string payload = msg.m_text.substr(7);

	    if (payload[0] == 'r') {
	        const std::string code = payload.substr(1);
			if (code.size() == 0)
				return;
	        const std::string tmrw_username = reg_user(code, msg.m_from);

	        constexpr auto reply_template = "Telegram аккаунт {} был успешно привязан к аккаунту Tomorrow School {}";
	        const std::string reply = std::format(reply_template, msg.m_from, tmrw_username);
	        msg.reply(reply);
	    } else {
	        msg.reply(std::format("received \"{}\" from {}", msg.m_text, msg.m_from));
	    }
    } catch(std::exception& e) {
    	std::cout << "failed to process a Telegram message: " << e.what() << '\n';
    }
}

int main()
{
    for (; true; usleep(SLEEP_DURATION)) {
        for (const auto& msg : Tg::get_messages()) {
            std::cout << msg << '\n';
            process_msg(msg);
            Tg::consume_msg(msg);
        }
        std::cout << "waiting...\n";
    }
}
