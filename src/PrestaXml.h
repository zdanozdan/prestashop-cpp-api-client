#ifndef PRESTAXML_H
#define PRESTAXML_H

#include <string>
#include <iostream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlwriter.h>
#include <libxml/xpath.h>

class PrestaXml
{
public:
    PrestaXml();
    ~PrestaXml();
    bool loadXml(std::string xmlContent);
    bool addCDATA(xmlNodePtr xmlNode, std::string cdata, bool slug = false);
    xmlNodePtr getNodeByXPath(std::string xpath);
    xmlNodeSetPtr getNodesByXPath(std::string xpath);
    std::string getXmlString();
private:
    xmlDocPtr doc;
    std::string toUtf8(std::string text);
    std::string slugify(std::string text);
};

#endif // PRESTAXML_H
