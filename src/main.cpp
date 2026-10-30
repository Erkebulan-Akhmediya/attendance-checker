#include "proc.h"

#include <cstdio>
#include <iostream>
#include <unistd.h>

const int POLLS_PER_SEC = 1;
const int SLEEP_DURATION = 1000000 / POLLS_PER_SEC;

int main()
{
    for (; true; usleep(SLEEP_DURATION)) {
        for (const auto& msg : Tg::get_messages()) {
            std::cout << msg << '\n';
            process_msg(msg);
            Tg::consume_msg(msg);
        }
        std::cout << "waiting...\n";
    }
}
