#include <iostream>
#include <curl/curl.h>
#include <string>
#include "nlohmann/json.hpp"
#include <fstream>






// Callback function for writing data received by libcurl
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to fetch the manifest file
std::string FetchManifest(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string response_data;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return response_data;
}

int main() {
    std::string manifestUrl = "https://siyargame.de/manifest/manifest.json"; // Replace with the actual URL
    std::string manifest = FetchManifest(manifestUrl);

    std::cout << "Manifest data:" << std::endl;
    std::cout << manifest << std::endl;

    return 0;
}
