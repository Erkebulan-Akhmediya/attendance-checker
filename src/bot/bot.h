#ifndef PROC_H
#define PROC_H

#include "tg.h"

class Bot {
public:
    static void run();

private:
    static void process_msg(const Message& msg);
};

#endif
