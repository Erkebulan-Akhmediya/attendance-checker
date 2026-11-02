#include "tg.h"
#include "../http.h"
#include <cstdlib>
#include <curl/curl.h>
#include <format>
#include <iostream>
#include <nlohmann/json.hpp>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

Message::Message(
    unsigned long long update_id,
    unsigned long long chat_id,
    std::string from,
    std::string text)
    : m_update_id(update_id)
    , m_chat_id(chat_id)
    , m_from(from)
    , m_text(text)
{
}

Message::Message(const nlohmann::json& json)
{
    try {
        m_update_id = json.at("update_id");
        const auto& msg = json.at("message");
        m_chat_id = msg.at("chat").at("id");
        m_from = msg.at("from").at("username");
        m_text = msg.at("text");
    } catch (nlohmann::json::exception& e) {
        std::cout << "invalid json schema\n";
        throw Message::Exception(m_update_id);
    }
}

Message::Exception::Exception(unsigned long long update_id)
    : std::runtime_error("message exception")
    , m_update_id(update_id)
{
}

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
            try {
                messages.emplace_back(update);
            } catch (Message::Exception& e) {
                Tg::skip(e.m_update_id);
            }
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

void Tg::skip(unsigned long long update_id)
{
    m_offset = update_id + 1;
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
