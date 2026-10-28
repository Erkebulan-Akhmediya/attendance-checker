#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <curl/easy.h>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

using json = nlohmann::json;

const std::string BOT_TOKEN = getenv("BOT_TOKEN");

size_t write_function(char* input, size_t size, size_t input_len, void* output)
{
    static_cast<std::string*>(output)->append(input, input_len);
    return input_len;
}

json get_request(std::string url)
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

void tg_get_updates()
{
    const std::string url = "https://api.telegram.org/bot" + BOT_TOKEN + "/getUpdates";
    auto updates = get_request(url);
    for (const auto& update : updates["result"]) {
        std::cout << update.dump(2) << '\n';
        std::cout << "------------------------------------\n";
    }
}

int main()
{
    tg_get_updates();
}
