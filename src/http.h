#include <nlohmann/json.hpp>
#include <string>
#include <utility>
#include <vector>

using json = nlohmann::json;

typedef std::vector<std::pair<std::string, std::string>> Params;

json get_request(const std::string& url);

std::string url_encode(const std::string& data);

json get_request(
    std::string& url,
    const std::vector<std::pair<std::string, std::string>>& params,
    const std::vector<std::pair<std::string, std::string>>& encoded_params);
