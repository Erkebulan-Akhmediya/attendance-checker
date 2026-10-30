#ifndef TG_H
#define TG_H

#include <cstdlib>
#include <string>
#include <vector>

struct Message {
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

    void reply(const std::string& text) const;
};

class Tg {
public:
    static std::vector<Message> get_messages();
    static void consume_msg(const Message& msg);
    static void send_message(unsigned long long chat_id, const std::string& text);

private:
    static unsigned long long m_offset;
    static const std::string BOT_TOKEN;
    static const std::string BASE_URL;
};

std::ostream& operator<<(std::ostream& os, const Message& msg);

#endif
