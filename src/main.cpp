#include <curl/curl.h>
#include <exception>
#include <format>
#include <nlohmann/json.hpp>

#include "http.h"
#include "tg.h"
#include <curl/easy.h>
#include <iostream>
#include <string>
#include <unistd.h>

using json = nlohmann::json;

const std::string BOT_TOKEN = getenv("BOT_TOKEN");
const int POLLS_PER_SEC = 1;
const int SLEEP_DURATION = 1000000 / POLLS_PER_SEC;

int main()
{
    auto tg = Tg::get_instance();
    for (; true; usleep(SLEEP_DURATION)) {
        for (const auto& msg : tg.get_messages()) {
            try {
                std::cout << msg << '\n';
                tg.send_message(msg.m_chat_id, std::format("received \"{}\" from {}", msg.m_text, msg.m_from));
                tg.consume_msg(msg);
            } catch (std::exception& e) {
                std::cout << "failed to process a message: " << e.what() << '\n';
            }
        }
        std::cout << "waiting...\n";
    }
}
