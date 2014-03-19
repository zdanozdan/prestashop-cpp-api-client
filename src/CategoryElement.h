#ifndef CONFIGUREELEMENT_H
#define CONFIGUREELEMENT_H

#include "BaseElement.h"
#include <iostream>

class CategoryElement : public PrestaElement::BaseElement
{
public:
    CategoryElement(std::string user, std::string password);
    ~CategoryElement();

private:
    bool addElement(std::vector<std::string> path);
    bool deleteElement();
    bool init(std::string apiXmlString);
};

#endif // CONFIGUREELEMENT_H
