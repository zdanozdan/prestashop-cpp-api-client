#ifndef BASEELEMENT_H
#define BASEELEMENT_H

#include <string>
#include <iostream>
#include <vector>

namespace PrestaElement
{

typedef enum
{
    eNone,
    ePrestaCategory,
    ePrestaProduct,
    eMax
} PrestaTypeElements;

class BaseElement
{
public:
    virtual ~BaseElement()
    {
    }
    virtual bool init(std::string apiXmlString) = 0;
    virtual int addElement(std::vector<std::string> path) = 0;
    virtual bool deleteElement() = 0;
    PrestaTypeElements getType()
    {
        return eType;
    }

protected:
    PrestaTypeElements eType;
    std::string blankXmlString;
    std::string serverUrl;
    std::string displayFullUrl;
    std::string user;
    std::string password;
};

}

#endif // BASEELEMENT_H
