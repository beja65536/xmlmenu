#ifndef __DEFINITION_OF_TINYXMLHELPER__H__
#define __DEFINITION_OF_TINYXMLHELPER__H__

#include <list>
#include <map>
#include <string>
#include "tinyxml2.h"


struct menu_item 
{
    menu_item(std::string n, void*c, bool m) : name(n), context(c), is_menu(m){};
    std::string name;
    void *context;
    bool is_menu;
};

void enable_debug_output();
void disable_debug_output();

std::string XmlGetValue(std::string name, tinyxml2::XMLNode *node);
tinyxml2::XMLNode *XmlGetChild(std::string name, tinyxml2::XMLNode *node);
tinyxml2::XMLNode *XmlGetSibling(std::string name, tinyxml2::XMLNode *node);
std::map<std::string, std::list<menu_item> *> Xml2Map(tinyxml2::XMLNode *node);

#endif