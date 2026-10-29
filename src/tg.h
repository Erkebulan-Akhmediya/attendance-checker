#include <cstdlib>
#include <memory>
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
};

class Tg {
public:
    static const Tg& get_instance();

    std::vector<Message> get_messages() const;
    void consume_msg(const Message& msg);
    void send_message(unsigned long long chat_id, const std::string& text);

private:
    Tg();

    static std::unique_ptr<Tg> instance;

    unsigned long long m_offset;
    const std::string BOT_TOKEN;
    const std::string BASE_URL;
};

std::ostream& operator<<(std::ostream& os, const Message& msg);
