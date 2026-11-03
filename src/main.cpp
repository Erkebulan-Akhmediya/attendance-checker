#include "bot/bot.h"
#include <thread>
#include <unistd.h>

int main()
{
    std::thread bot_thread(Bot::run);
    bot_thread.join();
}
