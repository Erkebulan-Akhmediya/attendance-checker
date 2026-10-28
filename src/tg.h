#include <memory>
#include <string>
#include <vector>

struct Message {
    unsigned long long m_update_id;
    std::string m_from;
    std::string m_text;

    Message(unsigned long long update_id, std::string from, std::string text)
        : m_update_id(update_id)
        , m_from(from)
        , m_text(text)
    {
    }
};

class Tg {
public:
    static Tg& get_instance();

    std::vector<Message> get_messages();
    void consume_msg(const Message& msg);

private:
    Tg()
        : m_offset(0)
    {
    }

    static inline std::unique_ptr<Tg> instance;

    unsigned long long m_offset;
};

std::ostream& operator<<(std::ostream& os, const Message& msg);
