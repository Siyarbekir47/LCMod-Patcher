#include <iostream>
#include <curl/curl.h>
#include <string>
#include "nlohmann/json.hpp"
#include <fstream>


const char* red = "\033[31m";
const char* reset = "\033[0m";
const char* green = "\033[92m";

// Callback function for writing data received by libcurl
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* data) {

    data->append(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;

}



// Function to fetch the manifest file
std::string FetchManifest(const std::string& url) {
    std::string response_data;
    CURLcode result = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (result != CURLE_OK) {
        std::cerr << red << "curl_global_init() failed: " << curl_easy_strerror(result) << reset <<std::endl;
    }
    else
    {
        std::cout << green << "curl_global_init() succeeded with status: " << curl_easy_strerror(result) << reset << std::endl;
    }
   
    CURL* curl = curl_easy_init();
    if (curl == NULL) {
        std::cerr << red << "curl = curl_easy_init() failed" << reset << std::endl;
    }
    else
    {
		std::cout << green <<"curl = curl_easy_init() succeeded" << reset <<std::endl;
	}


    if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

		result = curl_easy_perform(curl);
        if (result != CURLE_OK) {
   			std::cerr << red << "curl_easy_perform() failed: " << curl_easy_strerror(result) << reset << std::endl;
		}
        else
        {
			std::cout << green << "curl_easy_perform() succeeded with status: " << curl_easy_strerror(result) << reset << std::endl;
		}
	}

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return response_data;
}   


void saveFile(std::string data, std::string filename)
{

    std::ofstream outFile("manifest.json");
    if (outFile)
    {
		outFile << data;
		outFile.close();
        std::cout << green << "\nManifest.json saved successfully" << reset << std::endl;
	}
    else
    {
		std::cout << red << "Error opening file" << reset << std::endl;
	}

}



int main()
{

    std::string manifestUrl = "https://siyargame.de/manifest/manifest.json"; // url to check for updates
    std::string manifest = FetchManifest(manifestUrl);

    std::cout << "Manifest data: " << manifest << std::endl;

    saveFile(manifest, "manifest.json");

    return 0;
}


































































/*

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
    std::string manifestUrl = "https://siyargame.de/manifest/manifest.json"; // url to check for updates
    std::string manifest = FetchManifest(manifestUrl);

    std::cout << "Manifest data:" << std::endl;
    std::cout << manifest << std::endl;

    return 0;
}




*/
