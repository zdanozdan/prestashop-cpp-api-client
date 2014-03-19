#ifndef PRESTAURL_H
#define PRESTAURL_H

#include <string>
#include <iostream>
#include <curl.h>

typedef struct
{
    const void *data;
    int size;
    int pos;
} PostData;

class PrestaUrl
{
public:
    static CURLcode Get(const std::string url, const std::string &buffer, const std::string user="", const std::string pass="");
    static CURLcode Post(const std::string url, const std::string &buffer, const std::string user="", const std::string pass="");
    static CURLcode Delete(const std::string url, const std::string user="", const std::string pass="");
private:
    static size_t writeFunction(char *ptr, size_t size, size_t nmemb, void *userdata);
    static size_t readFunction(char *ptr, size_t size, size_t nmemb, void *userdata);
};

#endif // PRESTAURL_H
