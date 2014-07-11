#include "curl.h"
#include <iostream>

int main (int argc, char * argv []) {
    std::string url = "https://zkillboard.com/api/losses/solarSystemID/30000141/";

    std::cout << curlFetch(url);

    return 0;
}