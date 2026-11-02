#ifndef TG_H
#define TG_H

#include <cstdlib>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

struct Message {

    struct Exception {
        unsigned long long m_update_id;

        Exception(unsigned long long update_id)
            : m_update_id(update_id)
        {
        }
    };

    unsigned long long m_update_id;
    unsigned long long m_chat_id;
    std::string m_from;
    std::string m_text;

    Message(
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

    Message(const nlohmann::json& json)
    {
        try {
            m_update_id = json.at("update_id");
            const auto& msg = json.at("message");
            m_chat_id = msg.at("chat").at("id");
            const std::string from = msg.at("from").at("username");
            const std::string text = msg.at("text");
        } catch (nlohmann::json::exception& e) {
            std::cout << "invalid json schema\n";
            throw Message::Exception(m_update_id);
        }
    }

    void reply(const std::string& text) const;
};

class Tg {
public:
    static std::vector<Message> get_messages();
    static void consume_msg(const Message& msg);
    static void skip(unsigned long long update_id);
    static void send_message(unsigned long long chat_id, const std::string& text);

private:
    static unsigned long long m_offset;
    static const std::string BOT_TOKEN;
    static const std::string BASE_URL;
};

std::ostream& operator<<(std::ostream& os, const Message& msg);

#endif
