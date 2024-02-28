#include <curl/curl.h>

#include <iostream>
#include <string>

#include "api.h"

#include "nlohmann/json.hpp"

// Callback function writes data to a std::string, and returns the size of the
// data written.
static size_t WriteCallback(void* contents, size_t size, size_t nmemb,
                            std::string* data) {
  size_t newLength = size * nmemb;
  try {
    data->append((char*)contents, newLength);
    return newLength;
  } catch (std::bad_alloc& e) {
    // handle memory problem
    return 0;
  }
}

nlohmann::json makeGetRequest(const std::string& ux) {
  nlohmann::json response_json;

  CURL* curl;
  CURLcode res;
  std::string readBuffer;

  curl = curl_easy_init();

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, ux.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    else {
      response_json = nlohmann::json::parse(readBuffer);
    }

    curl_easy_cleanup(curl);
  }

  return response_json;
}

nlohmann::json makePostRequest(const std::string& url,
                               const nlohmann::json& data,
                               const std::string& api_key) {
  CURL* curl;
  CURLcode res;
  std::string readBuffer;

  nlohmann::json response_json;

  std::string data_str = data.dump();

  // Initialize a list for custom headers
  struct curl_slist* headers = NULL;
  headers = curl_slist_append(headers, "Content-Type: application/json");

  // Prepare the Authorization header with the API key
  std::string authHeader = "Authorization: Bearer " + api_key;
  headers = curl_slist_append(headers, authHeader.c_str());

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);  // Specify the request is a POST
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS,
                     data_str.c_str());                   // Set the POST data
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);  // Set custom headers
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    else {
      // Parse JSON and use it
      response_json = nlohmann::json::parse(readBuffer);
    }

    // Cleanup
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);  // Free the list of headers
  }

  return response_json;
}
