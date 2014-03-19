
#include <sstream>
#include "PrestaUrl.h"

using namespace std;

//#define CURL_DOES_CONVERSIONS

CURLcode PrestaUrl::Get(const string url, const string &buffer, const string user, const string pass)
{
    string login = user;
    login.append(":");
    login.append(pass);

    CURL *curl = 0;
    CURLcode res(CURLE_OK);

    curl = curl_easy_init();

    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_USERPWD, login.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        //curl_easy_setopt(curl, CURL_DOES_CONVERSIONS, 0);
        //struct curl_slist *headers=NULL;
        //headers = curl_slist_append(headers, "Content-Type: text/html; charset=utf-8");
        //curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        //curl_easy_setopt(curl, CURLOPT_HEADER, 0);
        /*curl_easy_setopt(curl, CURLOPT_CONV_FROM_NETWORK_FUNCTION, convertCallback);
        curl_easy_setopt(curl, CURLOPT_CONV_TO_NETWORK_FUNCTION, convertCallback);
        curl_easy_setopt(curl, CURLOPT_CONV_FROM_UTF8_FUNCTION, convertCallback);*/

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if(res != CURLE_OK)
        {
            cout << "curl_easy_perform() failed - " << curl_easy_strerror(res) << endl;
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    else
    {
        res = CURLE_FAILED_INIT;
        cout << "PrestaParser::Get - wrong curl_easy_init()" << endl;
    }

    return res;
}

CURLcode PrestaUrl::Post(const string url, const string &buffer, const string user, const string pass)
{
    string login = user;
    login.append(":");
    login.append(pass);

    CURL *curl = 0;
    CURLcode res(CURLE_OK);

    curl = curl_easy_init();

    if(curl)
    {
        /*PostData data;
        data.data = buffer.c_str();
        data.size = buffer.length();
        data.pos = 0;*/

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_USERPWD, login.c_str());
        //curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        //curl_easy_setopt(curl, CURLOPT_READFUNCTION, readFunction); // PUT
        //curl_easy_setopt(curl, CURLOPT_READDATA, &data); // PUT
        //curl_easy_setopt(curl, CURLOPT_INFILESIZE, buffer.length());
        //curl_easy_setopt(curl,CURLOPT_UPLOAD, 1); // PUT

        //curl_easy_setopt(curl, CURLOPT_POST, 1);
        //curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, buffer.length());
        //curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
        //const char *data = buffer.c_str();
        //cout << data << endl;
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buffer.c_str());
        //curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");

        /*struct curl_slist *headers=NULL;
        headers = curl_slist_append(headers, "Content-Type: text/html; charset=utf-8");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_HEADER, 1);*/

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if(res != CURLE_OK)
        {
            cout << "curl_easy_perform() failed - " << curl_easy_strerror(res) << endl;
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    else
    {
        res = CURLE_FAILED_INIT;
        cout << "PrestaParser::Get - wrong curl_easy_init()" << endl;
    }

    return res;
}

CURLcode PrestaUrl::Delete(const string url, const string user, const string pass)
{
    string login = user;
    login.append(":");
    login.append(pass);

    CURL *curl = 0;
    CURLcode res(CURLE_OK);

    curl = curl_easy_init();

    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_USERPWD, login.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if(res != CURLE_OK)
        {
            cout << "curl_easy_perform() failed - " << curl_easy_strerror(res) << endl;
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    else
    {
        res = CURLE_FAILED_INIT;
        cout << "PrestaParser::Get - wrong curl_easy_init()" << endl;
    }
    return res;
}

size_t PrestaUrl::writeFunction(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    string &buffer = *static_cast<string*>(userdata);
    streamsize len = size * nmemb;
    buffer.append(ptr, len);

    return len;
}

size_t PrestaUrl::readFunction(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    size_t written = 0;
    if(userdata)
    {
        PostData *data = (PostData*)userdata;
        int available = data->size - data->pos;
        if(available > 0)
        {
            written = min(available, (int)(size * nmemb));
            memcpy(ptr, (char*)data->data + data->pos, written);
            data->pos += written;
        }
    }
    return written;
}
