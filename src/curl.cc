#include "curl.h"

int writer (char * data, size_t size, size_t nmemb, std::string * buffer_in) {
    if (buffer_in != NULL) {
        buffer_in->append(data, size * nmemb);
        return size * nmemb;
    }
    return 0;
}


std::string curlFetch (std::string url) {
    CURL * curl;
    std::string buffer;

    struct curl_slist * headers = NULL;

    curl_slist_append(headers, "Accept: application/json");
    curl_slist_append(headers, "Content-Type: application/json");
    curl_slist_append(headers, "charsets: utf-8");

    curl = curl_easy_init();
    if (curl == NULL)
        return "";

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &buffer);
        curl_easy_cleanup(curl);
        if (CURLE_OK == res)
            return buffer;
        return "";
    }

    curl_easy_cleanup(curl);
    return buffer;
}