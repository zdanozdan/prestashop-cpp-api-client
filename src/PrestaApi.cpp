#include <iostream>

#include "PrestaApi.h"
#include "PrestaUrl.h"

#include "PrestaXml.h"
#include "CategoryElement.h"

using namespace std;
using namespace PrestaElement;

class PrestaApi::PrestaApiHandle
{
public:
    PrestaApiHandle();
    ~PrestaApiHandle()
    {
        for(unsigned int i; i < elems.size(); i++)
        {
            delete elems.at(i);
        }
    }

    bool initApiHandle(std::string usr, std::string pass, std::string serverUrl);
    int addCategory(std::vector<std::string> path);
    bool initElement(PrestaElement::PrestaTypeElements eType);
    int addElement(PrestaElement::PrestaTypeElements eType, std::vector<std::string> path);
private:
    std::string user;
    std::string password;
    std::string apiXmlString;
    bool isInitialized;
    std::vector<PrestaElement::BaseElement *> elems;
    bool deleteElement(PrestaElement::PrestaTypeElements eType);
};

PrestaApi::PrestaApiHandle::PrestaApiHandle() :
    user(""),
    password(""),
    apiXmlString(""),
    isInitialized(false),
    elems(NULL)
{
}

PrestaApi::~PrestaApi()
{
    if(apiHandle)
    {
        delete apiHandle;
    }
}

bool PrestaApi::PrestaApiHandle::initApiHandle(string usr, string pass, string serverUrl)
{
    user = usr;
    password = pass;

    CURLcode res = PrestaUrl::Get(serverUrl, apiXmlString, user, password);
    if(CURLE_OK == res && !apiXmlString.empty())
    {
        isInitialized = true;
        isInitialized = initElement(PrestaElement::ePrestaCategory);
    }

    return isInitialized;
}

bool PrestaApi::PrestaApiHandle::initElement(PrestaTypeElements eType)
{
    bool retValue = false;
    BaseElement *element = NULL;

    if(!isInitialized)
    {
        cout << "API is not initialized." << endl;
        return false;
    }

    switch(eType)
    {
        case ePrestaCategory:
        {
            element = new CategoryElement(user, password);
        }
        break;

        default:
        break;
    }

    if(NULL != element)
    {
        if(element->init(apiXmlString))
        {
            elems.push_back(element);
        }
    }

    return retValue;
}

int PrestaApi::PrestaApiHandle::addElement(PrestaTypeElements eType, vector<string> path)
{
    int retValue = 999;
    for(unsigned int i=0; i < elems.size(); i++)
    {
       if(eType == elems.at(i)->getType())
       {
          retValue = elems.at(i)->addElement(path);
       }
    }
    cout << "PrestaApiHandle::addElement retValue = " << retValue << endl;
    cout << "PrestaApi::addElement " << (0==retValue ? "success" : "failed") << endl;

    return retValue;
}

bool PrestaApi::PrestaApiHandle::deleteElement(PrestaTypeElements eType)
{
    bool retValue = false;
    for(unsigned int i=0; i < elems.size(); i++)
    {
        if(eType == elems.at(i)->getType())
        {
            retValue = elems.at(i)->deleteElement();
            break;
        }
    }

    return retValue;
}



PrestaApi::PrestaApi()
{
    apiHandle = new PrestaApiHandle();
}

bool PrestaApi::initApi(string usr, string pass, string serverUrl)
{
    return apiHandle->initApiHandle(usr, pass, serverUrl);
}

/*bool PrestaApi::initElement(PrestaTypeElements eType)
{
    return apiHandler->initElement(eType);
}*/

int PrestaApi::addCategory(vector<string> path)
{
   return apiHandle->addElement(PrestaElement::ePrestaCategory, path);
}
