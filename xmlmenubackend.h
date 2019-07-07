#ifndef __DEFINITION_OF_XMLMENUBACKEND__H__
#define __DEFINITION_OF_XMLMENUBACKEND__H__

#include "menubackend.h"
#include "tinyxml2.h"
#include "tinyxml2helper.h"


class xmlmenubackend : public menubackend
{
public:
    xmlmenubackend(std::string xmlf, std::string mn) : menubackend(xmlf, mn) 
    {
        if (!loadMenu())
            throw;
    };
    xmlmenubackend(std::string xmlf) : menubackend(xmlf, "menu") 
    {
        if (!loadMenu())
            throw;
    };
    bool loadMenu() override;
};

#endif