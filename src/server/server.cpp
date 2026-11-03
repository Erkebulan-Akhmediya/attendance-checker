#include "server.h"
#include "../user/user_service.h"
#include <format>
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

void Server::run()
{
    httplib::Server server;

    server.Get("/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("I am healthy!", "text/plain");
    });

    server.Get("/users/unregistered", [](const httplib::Request&, httplib::Response& res) {
        using json = nlohmann::json;
        auto unregistered = UserService().get_unregistered();
        json j = unregistered;
        res.set_content(j.dump(), "application/json");
    });

    server.Get("/users/unregistered/links", [](const httplib::Request&, httplib::Response& res) {
        using json = nlohmann::json;
        std::vector<std::string> links;
        for (const auto& username : UserService().get_unregistered()) {
            const auto link = std::format("https://t.me/ts_pvl_attendance_bot?start={}", Crypto().encrypt(username));
            links.push_back(link);
        }
        auto unregistered = UserService().get_unregistered();
        json j = links;
        res.set_content(j.dump(), "application/json");
    });

    server.listen("0.0.0.0", 8080);
}
