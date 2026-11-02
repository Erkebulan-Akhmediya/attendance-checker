#include "crypto.h"
#include <sodium.h>
#include <sodium/crypto_secretbox.h>
#include <sodium/randombytes.h>
#include <stdexcept>
#include <string>

std::string KEY;

Crypto::Crypto()
{
    if (sodium_init() < 0)
        throw std::runtime_error("failed to init sodium lib");
}
