#include "crypto.h"
#include <cstddef>
#include <iostream>
#include <sodium.h>
#include <sodium/core.h>
#include <sodium/crypto_secretbox.h>
#include <sodium/randombytes.h>
#include <sodium/utils.h>
#include <stdexcept>
#include <string>
#include <vector>

const Crypto& Crypto::instance = *(new Crypto());

const Crypto& Crypto::get_instance()
{
    return instance;
}

Crypto::Crypto()
    : m_key(crypto_secretbox_KEYBYTES)
{
    if (sodium_init() < 0)
        throw std::runtime_error("failed to init sodium lib");

    randombytes_buf(const_cast<unsigned char*>(m_key.data()), m_key.size());

    std::cout << "crypto obj created\n";
}

std::string Crypto::encrypt(const std::string& message) const
{
    std::vector<unsigned char> out(crypto_secretbox_NONCEBYTES + crypto_secretbox_MACBYTES + message.size());
    randombytes_buf(out.data(), crypto_secretbox_NONCEBYTES);
    crypto_secretbox_easy(
        out.data() + crypto_secretbox_NONCEBYTES,
        reinterpret_cast<const unsigned char*>(message.data()),
        message.size(),
        out.data(),
        m_key.data());
    return to_base64(out);
}

std::string Crypto::decrypt(const std::string& data) const
{
    std::vector<unsigned char> blob = from_base64(data);
    if (blob.size() < crypto_secretbox_NONCEBYTES + crypto_secretbox_MACBYTES)
        throw std::runtime_error("encrypted message is too short");

    const unsigned char* nonce = blob.data();
    const unsigned char* ciphertext = blob.data() + crypto_secretbox_NONCEBYTES;
    size_t ciphertext_len = blob.size() - crypto_secretbox_NONCEBYTES;

    std::string out(ciphertext_len - crypto_secretbox_MACBYTES, '\0');
    const int res = crypto_secretbox_open_easy(
        reinterpret_cast<unsigned char*>(out.data()),
        ciphertext,
        ciphertext_len,
        nonce,
        m_key.data());
    if (res != 0)
        throw std::runtime_error("decryption failed");
    return out;
}

std::string Crypto::to_base64(const std::vector<unsigned char>& bin) const
{
    size_t len = sodium_base64_encoded_len(bin.size(), Crypto::b64_variant);
    std::string out(len, '\0');
    sodium_bin2base64(out.data(), len, bin.data(), bin.size(), Crypto::b64_variant);
    out.resize(len - 1);
    return out;
}

std::vector<unsigned char> Crypto::from_base64(const std::string& b64) const
{
    std::vector<unsigned char> bin(b64.size());
    size_t bin_len;
    const int res = sodium_base642bin(
        bin.data(),
        bin.size(),
        b64.data(),
        b64.size(),
        nullptr,
        &bin_len,
        nullptr,
        Crypto::b64_variant);
    if (res != 0)
        throw std::runtime_error("invalid base 64 string");
    bin.resize(bin_len);
    return bin;
}
