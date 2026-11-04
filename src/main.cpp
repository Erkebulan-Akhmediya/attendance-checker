#include "bot/bot.h"
#include "server/server.h"
#include <thread>

int main()
{
    std::thread bot_thread(Bot::run);

    Server::run();

    bot_thread.join();
}
