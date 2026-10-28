#include <cstddef>
#include <curl/curl.h>
#include <memory>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

using json = nlohmann::json;

size_t write_function(char* input, size_t size, size_t input_len, void* output)
{
    static_cast<std::string*>(output)->append(input, input_len);
    return input_len;
}

json get_request(const std::string& url)
{
    std::unique_ptr<CURL, decltype(&curl_easy_cleanup)> curl(curl_easy_init(), &curl_easy_cleanup);
    if (!curl)
        throw std::runtime_error("curl_easy_init failed");

    curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());
    curl_easy_setopt(
        curl.get(),
        CURLOPT_WRITEFUNCTION,
        &write_function);
    std::string res;
    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &res);

    CURLcode code = curl_easy_perform(curl.get());
    if (code != CURLE_OK)
        throw std::runtime_error("request failed");

    return json::parse(res);
}
