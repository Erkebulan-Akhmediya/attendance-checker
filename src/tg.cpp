#include "tg.h"
#include "http.h"
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <ostream>
#include <string>
#include <vector>

const std::string BOT_TOKEN = getenv("BOT_TOKEN");

const Tg& Tg::get_instance()
{
    if (!instance)
        instance.reset(new Tg());
    return *instance.get();
}

std::vector<Message> Tg::get_messages() const
{
    const std::string url = "https://api.telegram.org/bot" + BOT_TOKEN + "/getUpdates?offset=" + std::to_string(m_offset);
    const auto updates = get_request(url);

    std::vector<Message> messages;
    for (const auto& update : updates["result"]) {
        const unsigned long long update_id = update["update_id"];
        const auto& msg = update["message"];
        const std::string from = msg["from"]["username"];
        const std::string text = msg["text"];
        messages.emplace_back(update_id, from, text);
    }
    return messages;
}

void Tg::consume_msg(const Message& msg)
{
    m_offset = msg.m_update_id + 1;
}

std::ostream& operator<<(std::ostream& os, const Message& msg)
{
    os << "{ update_id: " << msg.m_update_id;
    os << ", from: " << msg.m_from;
    os << ", text: " << msg.m_text;
    return os << " }";
}
