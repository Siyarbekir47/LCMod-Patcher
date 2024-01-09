#include <iostream>
#include <curl/curl.h>
#include <string>
#include "nlohmann/json.hpp"
#include <fstream>
#include <sstream>

#include <filesystem>



namespace fs = std::filesystem;

const char* red = "\033[31m";
const char* reset = "\033[0m";
const char* green = "\033[92m";

// Callback function for writing data received by libcurl
size_t WriteData(void* ptr, size_t size, size_t nmemb, std::ofstream* outFile) {
    outFile->write((char*)ptr, size * nmemb);
    return size * nmemb;
}




bool DownloadFile(const std::string& url, const std::string& localPath) {
    CURL* curl;
    std::ofstream file;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        file.open(localPath, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << localPath << std::endl;
            curl_easy_cleanup(curl);
            return false;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            file.close();
            curl_easy_cleanup(curl);
            return false;
        }

        file.close();
        curl_easy_cleanup(curl);
    }
    else {
        return false;
    }

    return true;
}

bool UpdateFile(const std::string& oldFilePath, const std::string& newFilePath)
{
    //Check if the new file exists
    std::ifstream newFile(newFilePath);
    if (!newFile.good())
    {
		std::cerr << red << "New file not found" << reset << std::endl;
		return false;
    }
    //delete the old file
    if (std::remove(oldFilePath.c_str()) != 0)
    {
        std::cerr << red << "Error deleting file" << oldFilePath << reset << std::endl;
        return false;
    }
    if (std::rename(newFilePath.c_str(), oldFilePath.c_str()) != 0)
    {
		std::cerr << red << "Error renaming file" << reset << std::endl;
		return false;
	}   
	return true;

}



bool createFolderIfNotExist(const std::string& folderPath)
{
    try
    {
        
        if (fs::exists(folderPath))
        {
            std::cout << green << "Folder already exists." << reset << std::endl;
            return true;
        }
        
        else
        {
			std::cerr << red << "Folder dont exists" << reset << std::endl;
            fs::create_directories(folderPath);
            if (fs::exists(folderPath))
            {
				std::cout << green << "Folder created successfully" << reset << std::endl;
				return true;
			}
            else
            {
				std::cerr << red << "Error creating folder" << reset << std::endl;
				return false;
			}
		}
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return false;
}
 

int main()
{

    std::string manifestUrl = "https://siyargame.de/manifest/manifest.json"; // url to check for updates

    std::string folderPath = "update/";

    createFolderIfNotExist(folderPath);
    DownloadFile(manifestUrl, "update/manifest.json");




    std::ifstream file("manifest.json");
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string manifestContent = buffer.str();

    return 0;
}






