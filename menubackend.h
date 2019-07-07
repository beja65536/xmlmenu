#ifndef __DEFINITION_OF_MENUBACKEND__H__
#define __DEFINITION_OF_MENUBACKEND__H__

#include <list>
#include <map>
#include <string>
#include "tinyxml2helper.h"


class menubackend 
{
public:
    menubackend(std::string xmlf, std::string mn);
    menubackend(std::string xmlf);
    ~menubackend(){
        // FIXME: required
    };
    // Menus
    void reloadMenu();
    bool openMenu();
    bool exitMenu();
    std::string getMenuName();
    std::string getMenuID();
    std::string getMenuPath();
    std::list<menu_item> getMenuItems();
    // Items
    bool nextItem();
    bool prevItem();
    int getItemID();
    menu_item getItem();

protected:
    // Menus
    virtual bool loadMenu() = 0;
    std::string xml_file;
    std::string main_menu_name;
    //   Current menu
    std::string menu_name;
    std::string menu_id;
    int menu_length;
    std::list<std::string> menu_stack;
    std::map<std::string, std::list<menu_item> *> menu_map;
    // Items
    int item;
    std::list<int> item_stack;
};

#endif