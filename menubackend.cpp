#include "menubackend.h"


// Public
// =====================================================

menubackend::menubackend(std::string xmlf) : xml_file(xmlf), main_menu_name("menu"),
                                                   menu_name(""), menu_id(""), menu_length(0),
                                                   menu_stack(), menu_map(),
                                                   item(0), item_stack()
{ }

menubackend::menubackend(std::string xmlf, std::string mn) : xml_file(xmlf), main_menu_name(mn),
                                                                   menu_name(""), menu_id(""), menu_length(0),
                                                                   menu_stack(), menu_map(),
                                                                   item(0), item_stack()
{ }


void menubackend::reloadMenu() {
    loadMenu();
}

bool menubackend::openMenu() {
    std::map<std::string, std::list<menu_item> *>::const_iterator cit;

    // Get new menu's name
    std::string new_menu_name;
    cit = menu_map.find(menu_id);
    std::list<menu_item>::const_iterator cit_items;
    cit_items = cit->second->begin();
    std::advance(cit_items, item);
    if (cit_items == cit->second->end()) {
        return false;
    }
    new_menu_name = cit_items->name;

    // Open new menu
    cit = menu_map.find(menu_id+new_menu_name);
    if (cit == menu_map.end()) {        // New menu id does not exists
        return false;
    }
    if (cit->second == nullptr) {       // New menu id has no menu
        return false;
    }
    // Update menu ID to new menu
    menu_id.append(new_menu_name);
    // Put current menu name to stack
    menu_stack.push_back(menu_name);
    // Update menu name to new menu
    menu_name = new_menu_name;
    // Update menu length
    menu_length = cit->second->size();
    // Put current item number to stack
    item_stack.push_back(item);
    // Update select item
    item = 0;

    return true;
}

bool menubackend::exitMenu() {
    // Go to previous menu
    if (menu_stack.empty()) {
        return false;
    }
    // Update menu ID to previous menu
    menu_id = menu_id.substr(0, menu_id.size() - menu_name.size());
    // Update menu name to previous menu
    menu_name = menu_stack.back();
    // Pop current menu name from stack
    menu_stack.pop_back();
    // Update menu length
    std::map<std::string, std::list<menu_item> *>::const_iterator cit;
    cit = menu_map.find(menu_id);
    if (cit == menu_map.end()) {        // New menu id does not exists
        return false;
    }
    if (cit->second == nullptr) {       // New menu id has no menu
        return false;
    }
    menu_length = cit->second->size();
    // Update select item
    item = item_stack.back();
    // Pop current item number from stack
    item_stack.pop_back();

    return true;
}
std::string menubackend::getMenuName() {
    return menu_name;
}
std::string menubackend::getMenuID() {
    return menu_id;
}

std::string menubackend::getMenuPath() {
    std::string menu_path{""};
    std::list<std::string> menu_stack_cp = menu_stack;
    while (!menu_stack_cp.empty())
    {
        menu_path.append(menu_stack_cp.front());
        menu_path.append("/");
        menu_stack_cp.pop_front();
    }
    menu_path.append(menu_name);

    return menu_path;
}

std::list<menu_item> menubackend::getMenuItems() {
    std::map<std::string, std::list<menu_item> *>::const_iterator cit;
    cit = menu_map.find(menu_id);
    if (cit == menu_map.end()) {        // New menu id does not exists
        return {};   
    }
    if (cit->second == nullptr) {       // New menu id has no menu
        return {};  
    }

    return *(cit->second);
}

bool menubackend::nextItem() {
    item++;
    if (item > menu_length-1) {
        item = menu_length - 1;
        return false;
    }
    else
    {
        return true;
    }
}

bool menubackend::prevItem() {
    item--;
    if (item < 0) {
        item = 0;
        return false;
    }
    else
    {
        return true;
    }
}

int menubackend::getItemID() {
    return item;
}

menu_item menubackend::getItem() {
    std::list<menu_item> tmpmenu = getMenuItems();
    std::list<menu_item>::const_iterator cit_items;
    cit_items = tmpmenu.begin();
    std::advance(cit_items, item);
    if (cit_items == tmpmenu.end()) {
        return {"", nullptr, false};
    } 
    else {
        return *cit_items;
    }
}

