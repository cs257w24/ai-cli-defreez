#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <iostream>

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <fstream>

#include "api.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <system prompt filename>" << endl;
        return 1;
    }

    string system_prompt_path = argv[1];
    string system_message;
    ifstream system_prompt_file(system_prompt_path);
    string line;
    while (std::getline(system_prompt_file, line)) {
        system_message += line;
    }

    string message;
    while (std::getline(cin, line)) {
        message += line;  
    }

    string endpoint = "https://api.openai.com/v1/chat/completions";
    string api_key = getenv("OPENAI_API_KEY");

    nlohmann::json data;
    data["model"] = "gpt-3.5-turbo";

    nlohmann::json json_messages = nlohmann::json::array();
    nlohmann::json system_prompt;
    system_prompt["role"] = "system";
    system_prompt["content"] = system_message;

    nlohmann::json user_message;
    user_message["role"] = "user";
    user_message["content"] = message;

    json_messages.push_back(system_prompt);
    json_messages.push_back(user_message);

    data["messages"] = json_messages;
    auto response = makePostRequest(endpoint, data, api_key);

    cout << response["choices"][0]["message"]["content"] << endl;

    return 0;
}
