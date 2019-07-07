#include "xmlmenubackend.h"

bool xmlmenubackend::loadMenu() {
    // FIXME: Move to tinyxmlhelper --> menubackend will not depend on tinyxml directly

    // Get start of XML document
    tinyxml2::XMLDocument xml_doc;
    tinyxml2::XMLError tinyret = xml_doc.LoadFile(xml_file.c_str());
    if (tinyret != tinyxml2::XML_SUCCESS) {
        // Error
        // FIXME
        return false;
    }
    // Get XML reference to menu
    tinyxml2::XMLNode *node = xml_doc.FirstChild();
    std::string tmp_menu_name = xml_doc.FirstChild()->Value();
    if (xml_doc.FirstChild()->Value() != main_menu_name) {
        node = XmlGetSibling(tmp_menu_name, node);
    }
    if (node == nullptr) {
        // Error
        // FIXME
        return false;
    }

    // Extract menu structure
    menu_map = Xml2Map(node);

    // Initialize menu parameters
    menu_id = main_menu_name;
    menu_name = main_menu_name;
    item = 0;
    std::map<std::string, std::list<menu_item> *>::const_iterator cit;
    cit = menu_map.find(menu_id);
    if (cit == menu_map.end()) {        // Main menu id does not exists
        return false;
    }
    if (cit->second == nullptr) {       // Main menu id has no menu
        return false;
    }
    menu_length = cit->second->size();

    return true;
}