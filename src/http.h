#ifndef HTTP_H
#define HTTP_H

#include <curl/curl.h>
#include <curl/easy.h>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

using EncodedString = std::unique_ptr<char, decltype(&curl_free)>;

class Http {
public:
    Http();

    json get_req(const std::string& url);

    EncodedString encode(const std::string& data);

private:
    std::unique_ptr<CURL, decltype(&curl_easy_cleanup)> m_curl;
};

#endif
