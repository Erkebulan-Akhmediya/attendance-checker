#ifndef CRYPTO_H
#define CRYPTO_H

#include <sodium/crypto_secretbox.h>
#include <sodium/utils.h>
#include <string>
#include <vector>

// TODO: use persistent keys
class Crypto {
public:
    static const Crypto& get_instance();

    std::string encrypt(const std::string& data) const;
    std::string decrypt(const std::string& data) const;

    Crypto(const Crypto&) = delete;

private:
    static constexpr int b64_variant = sodium_base64_VARIANT_URLSAFE_NO_PADDING;

    Crypto();

    const std::vector<unsigned char> m_key;

    std::string to_base64(const std::vector<unsigned char>& bin) const;
    std::vector<unsigned char> from_base64(const std::string& b64) const;
};

#endif
