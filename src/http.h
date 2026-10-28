#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

json get_request(const std::string& url);
