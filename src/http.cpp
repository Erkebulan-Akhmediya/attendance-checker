#include "http.h"
#include <cstddef>
#include <curl/curl.h>
#include <curl/easy.h>
#include <memory>
#include <nlohmann/json.hpp>
#include <ranges>
#include <stdexcept>
#include <string>
#include <vector>

using json = nlohmann::json;

// size is always 1
size_t write_function(char* input, size_t size, size_t input_len, void* output)
{
    static_cast<std::string*>(output)->append(input, input_len * size);
    return input_len;
}

json generic_get_request(CURL* curl, const std::string& url)
{
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5l);
    curl_easy_setopt(
        curl,
        CURLOPT_WRITEFUNCTION,
        &write_function);
    std::string res;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);

    CURLcode code = curl_easy_perform(curl);
    if (code != CURLE_OK)
        throw std::runtime_error("request failed: " + url);

    return json::parse(res);
}

json get_request(const std::string& url)
{
    std::unique_ptr<CURL, decltype(&curl_easy_cleanup)> curl(curl_easy_init(), &curl_easy_cleanup);
    if (!curl)
        throw std::runtime_error("curl_easy_init failed");

    return generic_get_request(curl.get(), url);
}

json get_request(std::string& url,
    const std::vector<std::pair<std::string, std::string>>& params,
    const std::vector<std::pair<std::string, std::string>>& encoded_params)
{
    std::unique_ptr<CURL, decltype(&curl_easy_cleanup)> curl(curl_easy_init(), &curl_easy_cleanup);
    if (!curl)
        throw std::runtime_error("curl_easy_init failed");

    if (params.size() > 0 || encoded_params.size() > 0)
        url += '?';

    bool first = true;
    for (const auto& [key, value] : params) {
        if (!first)
            url += '&';
        url += key + '=' + value;
        first = false;
    }

    if (encoded_params.size() > 0)
        url += '&';

    first = true;
    for (const auto& [key, value] : encoded_params) {
        auto encoded = std::unique_ptr<char, decltype(&curl_free)>(
            curl_easy_escape(
                curl.get(),
                value.c_str(),
                value.size()),
            &curl_free);

        if (!first)
            url += '&';
        url += key + '=' + encoded.get();
        first = false;
    }
    return generic_get_request(curl.get(), url);
}

std::string url_encode(const std::string& data)
{
    std::unique_ptr<CURL, decltype(&curl_easy_cleanup)> curl(curl_easy_init(), &curl_easy_cleanup);
    if (!curl)
        throw std::runtime_error("curl_easy_init failed");

    auto encoded = std::unique_ptr<char, decltype(&curl_free)>(curl_easy_escape(curl.get(), data.c_str(), data.size()), &curl_free);
    std::string encoded_str = encoded.get();
    return encoded_str;
}
