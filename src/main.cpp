#include <curl/curl.h>
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
            std::cout << msg << '\n';
            tg.consume_msg(msg);
        }
        std::cout << "waiting...\n";
    }
}
