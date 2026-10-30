#include "tg.h"
#include "http.h"
#include <cstdlib>
#include <curl/curl.h>
#include <format>
#include <iostream>
#include <nlohmann/json.hpp>
#include <ostream>
#include <string>
#include <vector>

void Message::reply(const std::string& text) const
{
    Tg::send_message(m_chat_id, text);
}

unsigned long long Tg::m_offset = 0;
const std::string Tg::BOT_TOKEN = std::getenv("BOT_TOKEN");
const std::string Tg::BASE_URL = "https://api.telegram.org/bot" + BOT_TOKEN;

std::vector<Message> Tg::get_messages()
{
    try {
        const std::string url = BASE_URL + "/getUpdates?offset=" + std::to_string(m_offset);
        const auto updates = Http().get_req(url);

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
    } catch (std::exception& e) {
        std::cout << "failed to get Telegram messages: " << e.what() << '\n';
    }
    return {};
}

void Tg::consume_msg(const Message& msg)
{
    m_offset = msg.m_update_id + 1;
}

void Tg::send_message(unsigned long long chat_id, const std::string& text)
{
    Http http;
    const std::string url = std::format("{}/sendMessage?chat_id={}&text={}", BASE_URL, chat_id, http.encode(text).get());
    http.get_req(url);
}

std::ostream& operator<<(std::ostream& os, const Message& msg)
{
    os << "{ update_id: " << msg.m_update_id;
    os << ", from: " << msg.m_from;
    os << ", text: " << msg.m_text;
    return os << " }";
}
