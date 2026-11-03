#ifndef CRYPTO_H
#define CRYPTO_H

#include <sodium/crypto_secretbox.h>
#include <sodium/utils.h>
#include <string>
#include <vector>

// TODO: make it singleton
// TODO: use persistent keys
class Crypto {
public:
    Crypto();

    std::string encrypt(const std::string& data);
    std::string decrypt(const std::string& data);

private:
    static constexpr int b64_variant = sodium_base64_VARIANT_URLSAFE_NO_PADDING;

    static const Crypto instance;

    const std::vector<unsigned char> m_key;

    std::string to_base64(const std::vector<unsigned char>& bin);
    std::vector<unsigned char> from_base64(const std::string& b64);
};

#endif
