#include "prestaxml.h"
#include <string.h>

using namespace std;


PrestaXml::PrestaXml()
    : doc(NULL)
{
}

/*PrestaXml::PrestaXml(const PrestaXml &xml)
{
    xmlChar *xmlbuff;
    xmlDocDumpFormatMemory(doc, &xmlbuff, NULL, 1);
}*/

PrestaXml::~PrestaXml()
{
    xmlFreeDoc(doc);

    /*
     * Cleanup function for the XML library.
     */
    xmlCleanupParser();
    /*
     * this is to debug memory for regression tests
     */
    //xmlMemoryDump();
}

bool PrestaXml::loadXml(string xmlContent)
{
    //doc = xmlReadMemory(xmlContent.c_str(), xmlContent.length(), "", NULL, 0);
    doc = xmlParseDoc((xmlChar*)xmlContent.c_str());
    //doc = xmlReadDoc((xmlChar*)xmlContent.c_str(), NULL, NULL, 0);
    if (doc == NULL)
    {
        cout << "PrestaXml::loadXml: Failed to parse document" << endl;
        return false;
    }

    //xmlNode *root_element = xmlDocGetRootElement(doc);

    return true;
}

bool PrestaXml::addCDATA(xmlNodePtr xmlNode, string cdata, bool slug)
{
    if(NULL == xmlNode)
    {
        return false;
    }
    if(slug)
    {
        cdata = slugify(cdata);
    }
    else
    {
        cdata = toUtf8(cdata);
    }
    xmlNodePtr cdataNode = xmlNewCDataBlock(doc, (const xmlChar*)cdata.c_str(), cdata.length());
    cdataNode = xmlAddChild(xmlNode, cdataNode);
    if(NULL == cdataNode)
    {
        return false;
    }

    return true;
}

xmlNodeSetPtr PrestaXml::getNodesByXPath(string xpath)
{
    xpath = toUtf8(xpath);
    xmlXPathContextPtr xpathCtx;
    xmlXPathObjectPtr xpathObj;

    xpathCtx = xmlXPathNewContext(doc);
    if(xpathCtx == NULL)
    {
        cout << "Error: unable to create new XPath context" << endl;
        return NULL;
    }

    /* Evaluate xpath expression */
    xpathObj = xmlXPathEvalExpression((xmlChar*)xpath.c_str(), xpathCtx);
    if(xpathObj == NULL)
    {
        cout << "Error: unable to evaluate xpath expression" << xpath << endl;
        xmlXPathFreeContext(xpathCtx);
        return NULL;
    }

    return xpathObj->nodesetval;
}

xmlNodePtr PrestaXml::getNodeByXPath(string xpath)
{
    xmlNodeSetPtr nodes = getNodesByXPath(xpath);
    if(NULL == nodes)
    {
        return NULL;
    }
    else
    {
        return nodes->nodeTab[0];
    }
}

string PrestaXml::getXmlString()
{
    xmlChar *xmlbuff;
    int buffersize;
    xmlDocDumpFormatMemory(doc, &xmlbuff, &buffersize, 0);

    return (char*)xmlbuff;
}

string PrestaXml::toUtf8(string text)
{
    const char* src = text.c_str();
    size_t srclen = strlen(src);
    size_t dstlen = srclen*2;

    char dst[dstlen];
    memset(dst, 0, sizeof(dst));

    const char * pIn = src;
    char * pOut = ( char*)dst;

    iconv_t conv = iconv_open("UTF-8", "char");
    iconv(conv, &pIn, &srclen, &pOut, &dstlen);
    iconv_close(conv);

    string retValue = dst;

    return retValue;
}

string PrestaXml::slugify(string title)
{
    if (title.empty())
        return "";

    const int maxlen = 80;
    int len = title.length();
    bool prevdash = false;
    string sb = "";
    char c;

    for (int i = 0; i < len; i++)
    {
        c = title.at(i);
        if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))
        {
            sb.append(string(1, c));
            prevdash = false;
        }
        else if (c >= 'A' && c <= 'Z')
        {
            // tricky way to convert to lowercase
            sb.append(1, ((char)(c | 32)));
            prevdash = false;
        }
        else if (c == ' ' || c == ',' || c == '.' || c == '/' ||
            c == '\\' || c == '-' || c == '_' || c == '=')
        {
            if (!prevdash && sb.length() > 0)
            {
                sb.append("-");
                prevdash = true;
            }
        }
        else if ((int)c >= 128)
        {
            //int prevlen = sb.length();
            //sb.Append(RemapInternationalCharToAscii(c));
            //if (prevlen != sb.Length) prevdash = false;
        }
        if (i == maxlen)
            break;
    }

    if (prevdash)
        return sb.substr(0, sb.length() - 1);
    else
        return sb;
}

/*string PrestaXml::slugify(string text)
{
    int len = text.length();
    char dst[len+1];
    memset(dst, 0, sizeof(dst));
    int j = 0;
    string exc =  "¹œ³¿Ÿæñêó";
    string slug = "aslzzcneo";
    for (int i=0; i < len; i++)
    {
        char c = text.at(i);
        int found = exc.find(c);
        if(found+1)
        {
            dst[i] = slug.at(found);
        }
        else
        {
            dst[i] = c;
        }
    }
    return string(dst);
}*/
