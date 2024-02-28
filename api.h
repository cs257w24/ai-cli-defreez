#ifndef API_H
#define API_H

#include <nlohmann/json.hpp>

nlohmann::json makeGetRequest(const std::string& ux);

nlohmann::json makePostRequest(const std::string& url,
                               const nlohmann::json& data,
                               const std::string& api_key);

#endif
