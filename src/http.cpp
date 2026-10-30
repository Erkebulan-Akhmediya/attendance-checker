#include "http.h"
#include <cstddef>
#include <curl/curl.h>
#include <curl/easy.h>
#include <memory>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

using json = nlohmann::json;

// size is always 1
size_t write_function(char* input, size_t size, size_t input_len, void* output)
{
    static_cast<std::string*>(output)->append(input, input_len * size);
    return input_len;
}

Http::Http()
    : m_curl(curl_easy_init(), &curl_easy_cleanup)
{
    if (!m_curl)
        throw std::runtime_error("curl init failed");

    curl_easy_setopt(m_curl.get(), CURLOPT_TIMEOUT, 5l);
    curl_easy_setopt(
        m_curl.get(),
        CURLOPT_WRITEFUNCTION,
        &write_function);
}

json Http::get_req(const std::string& url)
{
    curl_easy_setopt(m_curl.get(), CURLOPT_URL, url.c_str());
    std::string res;
    curl_easy_setopt(m_curl.get(), CURLOPT_WRITEDATA, &res);

    CURLcode code = curl_easy_perform(m_curl.get());
    if (code != CURLE_OK)
        throw std::runtime_error("request failed: " + url);

    return json::parse(res);
}

EncodedString Http::encode(const std::string& data)
{
    return { curl_easy_escape(m_curl.get(), data.c_str(), data.size()), &curl_free };
}
