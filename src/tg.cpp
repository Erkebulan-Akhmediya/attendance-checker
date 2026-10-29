#include "tg.h"
#include "http.h"
#include <curl/curl.h>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <ostream>
#include <string>
#include <vector>

std::unique_ptr<Tg> Tg::instance;

const Tg& Tg::get_instance()
{
    if (!instance)
        instance.reset(new Tg());
    return *instance.get();
}

Tg::Tg()
    : m_offset(0)
    , BOT_TOKEN(std::getenv("BOT_TOKEN"))
    , BASE_URL("https://api.telegram.org/bot" + BOT_TOKEN)
{
}

std::vector<Message> Tg::get_messages() const
{
    const std::string url = BASE_URL + "/getUpdates?offset=" + std::to_string(m_offset);
    const auto updates = get_request(url);

    std::vector<Message> messages;
    for (const auto& update : updates["result"]) {
        const unsigned long long update_id = update["update_id"];
        const auto& msg = update["message"];
        const unsigned long long chat_id = msg["chat"]["id"];
        const std::string from = msg["from"]["username"];
        const std::string text = msg["text"];
        messages.emplace_back(update_id, chat_id, from, text);
    }
    return messages;
}

void Tg::consume_msg(const Message& msg)
{
    m_offset = msg.m_update_id + 1;
}

void Tg::send_message(unsigned long long chat_id, const std::string& text)
{
    std::string url = BASE_URL + "/sendMessage";
    std::vector<std::pair<std::string, std::string>> params = {
        { "chat_id", std::to_string(chat_id) },
    };
    std::vector<std::pair<std::string, std::string>> encoded_params = {
        { "text", text }
    };
    get_request(url, params, encoded_params);
}

std::ostream& operator<<(std::ostream& os, const Message& msg)
{
    os << "{ update_id: " << msg.m_update_id;
    os << ", from: " << msg.m_from;
    os << ", text: " << msg.m_text;
    return os << " }";
}
