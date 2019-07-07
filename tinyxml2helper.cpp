#include "tinyxml2helper.h"
#include <sstream>
#include <iostream>
#include <curses.h>         // Debug output

static bool debug_output = false;
void enable_debug_output() {
    debug_output = true;
}
void disable_debug_output() {
    debug_output = false;
}

tinyxml2::XMLNode * XmlGetChild(std::string name, tinyxml2::XMLNode *node) {
    // ToDo: XmlGetChildREN (multiple)?
    // +: We would need to iterate over the structure once
    // -: We would need to compare every node's name to the searched childrens names
    // --> Trade off?
    tinyxml2::XMLNode *ret_val{nullptr};
    tinyxml2::XMLNode *child_ptr{node->FirstChild()};
    if (child_ptr != nullptr) {
        do
        {
            std::string child_name = child_ptr->Value();
            if (child_name == name)
            {
                ret_val = child_ptr;
            }
            child_ptr = child_ptr->NextSibling();
        } while (child_ptr != nullptr && (ret_val == nullptr));
    }

    return ret_val;
}

tinyxml2::XMLNode * XmlGetSibling(std::string name, tinyxml2::XMLNode *node) {
    // ToDo: Special case of XmlGetChild --> merge?
    tinyxml2::XMLNode *ret_val{nullptr};
    tinyxml2::XMLNode *sibling_ptr = node;
    if (sibling_ptr != nullptr)
    {
        do
        {
            std::string sibling_name = sibling_ptr->Value();
            if (sibling_name == name)
            {
                ret_val = sibling_ptr;
            }
            sibling_ptr = sibling_ptr->NextSibling();
        } while (sibling_ptr != nullptr && (ret_val == nullptr));
    }

    return ret_val;
}

std::string XmlGetValue(std::string name, tinyxml2::XMLNode *node) {
    std::string ret_val{""};
    if (node != nullptr) {
        do
        {
            std::string node_name = node->Value();
            if (node_name == name)
            {
                tinyxml2::XMLElement* element = node->ToElement();
                if (element != nullptr)
                    ret_val = element->GetText();
            }
            node = node->NextSibling();
        } while (node != nullptr && (ret_val.size() == 0));
    }

    return ret_val;
}

std::map<std::string, std::list<menu_item> *> Xml2Map(tinyxml2::XMLNode * node) {
    // Debugging
    bool debug_output = false;
    std::string prepend{" "};
    // Map of all menus
    std::map<std::string, std::list<menu_item> *> menus;
    // List of bookmarked parent menus
    std::list<tinyxml2::XMLNode *> prev_menu_nodes;
    // Line index
    int i = 0;

    // Create top-menu
    std::string menu_id = node->Value();
    menus[menu_id] = new std::list<menu_item>;
    node = node->FirstChild();

    // Create sub-menus
    do
    {
        // Process node type
        //   ENTRY:     if there are NO children  
        //              then continue with sibling
        //   SUB-MENU:  if there are children
        //              then continue with child(ren) before continue with sibling
        if (node->NoChildren()) {   // ENTRY
            // Debug output
            if (debug_output) {
                std::stringstream text;
                text <<prepend <<"  " <<node->Value() <<"\n";
                mvaddstr(i++, 0, text.str().c_str());
            }
            // Save menu's entry
            menus[menu_id]->push_back(menu_item(node->Value(), nullptr, false));
            // Next node is sibling
            node = node->NextSibling();
        }
        else {                      // SUB-MENU
            // Debug output
            if (debug_output) {
                std::stringstream text;
                text <<prepend <<"> " <<node->Value() <<"\n";
                mvaddstr(i++, 0, text.str().c_str());
            }
            // Save menu's sub-menu
            menus[menu_id]->push_back(menu_item(node->Value(), static_cast<void *>(node->FirstChild()), true));
            // Bookmark current menu before continuing with child
            prev_menu_nodes.push_back(node);
            // New active menu name:
            //   previous menu name appended with current node's name
            menu_id.append(std::string(node->Value()));
            // Next node is child
            node = node->FirstChild();
            // Debug output
            if (debug_output) {
                std::stringstream text;
                text <<prepend <<"  " <<menu_id <<"\n";
                mvaddstr(i++, 0, text.str().c_str());
            }
            // Create new active menu
            menus[menu_id] = new std::list<menu_item>;
            // Intend by one additional space
            prepend.append(" ");
        }

        // Check if end of sub-menu (XML hierarchy branch)
        //   END:   if current node does not exists (nullptr)
        //          then previous node was last node in hierarchy
        if (node == nullptr){
            // Debug output
            if (debug_output) {
                std::stringstream text;
                text <<prepend <<"  " <<menu_id <<"\n";
                mvaddstr(i++, 0, text.str().c_str());
            }
            // Next node
            //   Load node of previous (parent) menu and load its next sibiling
            //   If previous menu node was itself last in a menu
            //      then load the previous menu node's previous menu node
            do {
                // Go back to parent menu (new node is current sub-menu)
                if (!prev_menu_nodes.empty())  {
                    node = prev_menu_nodes.back();
                    prev_menu_nodes.pop_back();
                }
                // New active menu name:
                //   remove parenet's node name from menu name
                menu_id = menu_id.substr(0, menu_id.size()-std::string(node->Value()).size());
                // Go to parent sub-menu's next sibling
                if (node != nullptr) {
                    node = node->NextSibling();
                }
            } while ((node == nullptr) && !(prev_menu_nodes.empty()));
            // Remove one space of intend
            prepend.erase(prepend.end() - 1);
        }
    } while (!((node == nullptr) && (prev_menu_nodes.empty())));

    return menus;
}