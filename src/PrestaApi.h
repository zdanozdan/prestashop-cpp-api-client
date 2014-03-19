#ifndef PRESTAAPI_H
#define PRESTAAPI_H

#include <string>
#include <vector>

class PrestaApi
{
public:
    PrestaApi();
    ~PrestaApi();
    bool initApi(std::string user, std::string password, std::string serverUrl);
    bool addCategory(std::vector<std::string> path);
private:
    class PrestaApiHandle;
    PrestaApiHandle *apiHandle;
};

#endif // PRESTAAPI_H
