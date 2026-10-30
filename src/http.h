#include <curl/curl.h>
#include <curl/easy.h>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <utility>
#include <vector>

using json = nlohmann::json;

class Http {
public:
    Http();

    json get_req(const std::string& url);
    json get_req(
        std::string& url,
        const std::vector<std::pair<std::string, std::string>>& params,
        const std::vector<std::pair<std::string, std::string>>& encoded_params);

    std::unique_ptr<char, decltype(&curl_free)> encode(const std::string& data);

private:
    std::unique_ptr<CURL, decltype(&curl_easy_cleanup)> m_curl;
};
