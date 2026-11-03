#include "bot/bot.h"
#include "crypto.h"
#include "server/server.h"
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>

int main()
{
    std::string og = "yakhmediy";
    std::string enc = Crypto().encrypt(og);
    std::string dec = Crypto().decrypt(enc);
    std::cout << og << ' ' << enc << ' ' << dec << '\n';
    std::thread bot_thread(Bot::run);

    Server::run();

    bot_thread.join();
}
