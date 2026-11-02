#ifndef CRYPTO_H
#define CRYPTO_H

#include <string>
class Crypto {
public:
    Crypto();

    std::string encrypt(const std::string& data);

private:
    const std::string KEY;
};

#endif
