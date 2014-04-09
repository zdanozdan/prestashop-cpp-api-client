#include <vector>
#include "CategoryElement.h"
#include "PrestaXml.h"
#include "PrestaUrl.h"

using namespace std;
using namespace PrestaElement;

CategoryElement::CategoryElement(string usr, string pass)
    //: BaseElement(user, password)
{
    eType = ePrestaCategory;
    blankXmlString = "";
    serverUrl = "";
    displayFullUrl = "";
    user = usr;
    password = pass;
}

CategoryElement::~CategoryElement()
{
}

bool CategoryElement::init(string apiXmlString)
{
    bool retValue = false;
    PrestaXml apiXml;
    if(apiXml.loadXml(apiXmlString))
    {
        xmlNodePtr descNode = apiXml.getNodeByXPath("/prestashop/api/categories/description");
        if(NULL == descNode)
        {
            return false;
        }
        serverUrl = (char*)descNode->properties[0].children->content;
        displayFullUrl = serverUrl.append("?display=full");

        xmlNodePtr schemaNode = apiXml.getNodeByXPath("/prestashop/api/categories/schema[@type='blank']");
        string blankUrl = (char*)schemaNode->properties[0].children->content;
        CURLcode res = PrestaUrl::Get(blankUrl, blankXmlString, user, password);
        if(CURLE_OK == res && !blankXmlString.empty())
        {
            retValue = true;
        }
    }
    cout << "CategoryElement::init " << (retValue ? "success" : "failed") << endl;

    return retValue;
}

int CategoryElement::addElement(vector<string> categories)
{
   return 122;
    if(0 == categories.size())
    {
        cout << "Empty category list" << endl;
        return 1;
        //return false;
    }

    string baseCategoryUrl = "http://presta.mikran.pl/prestashop/api/categories/";
    string xpath = string("/prestashop/categories/category[name/language='").append(categories.at(0)).append("']");
    string xpathCategoryId = xpath + "/id";

    vector<string> urls;
    vector<string> urls2;

    //string categoryUrl;
    string parentCategoryUrl;
    string categoryId;
    string parentCategoryId;

    urls.push_back(displayFullUrl);

    for(unsigned int i=0; i < categories.size(); i++)
    {
        PrestaXml xml;
        xmlNodePtr categoryNode = NULL;
        //categoryNode = NULL;
        //categoryUrl = "";
        categoryId = "";
        for(unsigned int j=0; j < urls.size(); j++)
        {
            urls2.clear();
            string buffer;
            CURLcode res = PrestaUrl::Get(urls[j], buffer, user, password);

            if(CURLE_OK != res)
            {
                continue;
            }

            if(xml.loadXml(buffer))
            {
                categoryNode = xml.getNodeByXPath(xpath);
                if(NULL != categoryNode)
                {
                    //categoryUrl = urls[j];
                    categoryId = (char*)xml.getNodeByXPath(xpathCategoryId)->children->content;
                    xmlNodeSetPtr xmlNodes = xml.getNodesByXPath(xpath.append("/associations/categories/category"));
                    if(NULL == xmlNodes)
                    {
                        urls.clear();
                        break;
                    }
                    for(int k=0; k < xmlNodes->nodeNr; k++)
                    {
                        xmlNodePtr xmlNode = xmlNodes->nodeTab[k];
                        if(NULL == xmlNode)
                        {
                            continue;
                        }
                        if(NULL == xmlNode->properties)
                        {
                            continue;
                        }
                        urls2.push_back((char*)xmlNode->properties[0].children->content);
                    }
                    urls = urls2;
                    break;
                }
            }
        }

        if(NULL == categoryNode)
        {
            cout << "Create category..." << endl;
            PrestaXml categoryBlankXml;
            if(categoryBlankXml.loadXml(blankXmlString))
            {
                xmlNodePtr xmlNode = categoryBlankXml.getNodeByXPath("/prestashop/category/id_parent");
                if(parentCategoryUrl.empty())
                {
                    categoryBlankXml.addCDATA(xmlNode, "0");
                }
                else
                {
                    xmlNewProp(xmlNode, (xmlChar*)"xlink:href", (xmlChar*)parentCategoryUrl.c_str());
                    categoryBlankXml.addCDATA(xmlNode, parentCategoryId);
                }

                xmlNode = categoryBlankXml.getNodeByXPath("/prestashop/category/active");
                categoryBlankXml.addCDATA(xmlNode, "1");

                xmlNode = categoryBlankXml.getNodeByXPath("/prestashop/category/name/language");
                xmlNewProp(xmlNode, (xmlChar*)"xlink:href", (xmlChar*)"http://presta.mikran.pl/prestashop/api/languages/1");
                categoryBlankXml.addCDATA(xmlNode, categories.at(i));

                xmlNode = categoryBlankXml.getNodeByXPath("/prestashop/category/link_rewrite/language");
                xmlNewProp(xmlNode, (xmlChar*)"xlink:href", (xmlChar*)"http://presta.mikran.pl/prestashop/api/languages/1");
                categoryBlankXml.addCDATA(xmlNode, categories.at(i), true);


                string xmlString = categoryBlankXml.getXmlString();
                CURLcode res = PrestaUrl::Post(serverUrl, xmlString, user, password);
                if(CURLE_OK != res)
                {
                   return 2;
                   //return false;
                }
            }
            else
            {
               return 3;
               //return false;
            }
            if(i+1 < categories.size())
            {
                // new id
                // new url
                addElement(categories);
                break;
            }
        }
        else
        {
            //parentCategoryNode = categoryNode;
            //parentCategoryUrl = categoryUrl;
            parentCategoryUrl = baseCategoryUrl + categoryId;
            parentCategoryId = categoryId;
        }
        if(i+1 < categories.size())
        {
            xpath = string("/prestashop/category[name/language='").append(categories.at(i+1)).append("']");
            xpathCategoryId = xpath + "/id";
        }
    }

    return 0;
    //return true;
}

bool CategoryElement::deleteElement()
{
    PrestaUrl::Delete("http://presta.mikran.pl/prestashop/api/categories/?id=[,7]", user, password);
    return true;
}
