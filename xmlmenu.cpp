#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <list>
#include <map>
#include "tinyxml2helper.h"
#include "xmlmenubackend.h"

// Define colors
const short MENU_GENERAL = 1;
const short ITEM_SELECTED = 2;

// Draw menu and active selected item
std::string draw_menu(std::list<menu_item> *mil, 
                         int mp[2], int cp[2])
{
    std::string menu_marker{"> "};
    std::string item_marker{"  "};

    std::string selected_item;
    unsigned int max_item_width = 0;
    int first_item_pos[2] = {mp[0]+1, mp[1]+1};
    int cursor_pos[2] = {cp[0] + 1, cp[1] + 1};
    start_color();
    init_pair(MENU_GENERAL, COLOR_WHITE, COLOR_BLACK);
    init_pair(ITEM_SELECTED, COLOR_CYAN, COLOR_BLACK);

    // Print menu items
    std::list<menu_item>::const_iterator mil_cit;
    int mil_idx = 0;
    int num_of_items = 0;
    for (mil_cit = mil->begin(); mil_cit != mil->end(); mil_cit++)
    {
        if (mil_cit->name.size() > max_item_width)
            max_item_width = mil_cit->name.size();
        int item_pos_y = first_item_pos[0] + mil_idx++;
        int item_pos_x = first_item_pos[1];
        if (cursor_pos[0]==item_pos_y) {
            attron(A_BOLD);
            attron(COLOR_PAIR(ITEM_SELECTED));
        }
        else {
          attron(COLOR_PAIR(MENU_GENERAL));
        }
        std::string entry;
        if (mil_cit->is_menu)
            entry.append(menu_marker);
        else
            entry.append(item_marker);
        entry.append(mil_cit->name);

        mvaddstr(item_pos_y, item_pos_x, entry.c_str());
        num_of_items++;
        if (cursor_pos[0]==item_pos_y){
            attroff(A_BOLD);
            attroff(COLOR_PAIR(ITEM_SELECTED));
            selected_item = mil_cit->name;
        }
        else {
          attroff(COLOR_PAIR(MENU_GENERAL));
        }         
    }

    // Add menu marker / item marker space to max_item_width
    if (menu_marker.size() > item_marker.size())
        max_item_width = max_item_width + menu_marker.size();
    else
        max_item_width = max_item_width + item_marker.size();
        

    // Draw lines
    mvhline(mp[0],
            mp[1]+1, ACS_HLINE, max_item_width);
    mvhline(mp[0]+num_of_items+1,
            mp[1]+1, ACS_HLINE, max_item_width);
    mvvline(mp[0]+1, 
            mp[1], ACS_VLINE, num_of_items);
    mvvline(mp[0]+1, 
            mp[1]+max_item_width+1, ACS_VLINE, num_of_items);
    // Draw corners
    mvaddch(mp[0],
            mp[1], ACS_ULCORNER);
    mvaddch(mp[0],
            mp[1]+max_item_width+1, ACS_URCORNER);
    mvaddch(mp[0]+num_of_items+1,
            mp[1], ACS_LLCORNER);
    mvaddch(mp[0]+num_of_items+1,
            mp[1]+max_item_width+1, ACS_LRCORNER);

    return selected_item;
}

int main(void)
{
    // Enable colors
    start_color();
    // Get terminal information
    initscr();
    // Disable line buffering
    // + one char at a time
    cbreak();
    // Do not echo input chars
    noecho();
    // Clear screen
    clear();
    // Define max line number
    // + max column number
    // (based on initscr())
    const int MAXLINE = LINES -1;
    const int MAXCOL = COLS - 1;
    // Allow keypad + F keys
    keypad(stdscr, TRUE);
    // Hide cursor
    curs_set(0);


    // Create menu
    xmlmenubackend mb("menu.xml", "menu");


    // Handle menu
    std::string menu_str{"> "+mb.getMenuPath()};
    int user_input = 112;
    while (user_input != 113) {
        clear();

        // Print menu path
        menu_str = " > ";
        menu_str.append(mb.getMenuPath());
        mvaddstr(0, 0, menu_str.c_str());
        
        // Print staus bar
        //char user_input_str[8] = {0};
        //snprintf(user_input_str, 8, "> %5d", user_input);
        //mvaddstr(MAXLINE-1, 0, user_input_str);
        mvaddstr(MAXLINE, 0, "Press 'q' to quit");

        // Draw menu
        std::list<menu_item> menu_items = mb.getMenuItems();
        int menu_offset_x = 0;
        int menu_offset_y = 2;
        int menu_pos[2] = {menu_offset_y, menu_offset_x};
        int cursor_pos[2] = {mb.getItemID() + menu_offset_y, menu_offset_x};
        draw_menu(&menu_items, menu_pos, cursor_pos);

        // Process user input
        user_input = getch();
        switch(user_input) {
            case KEY_UP:
            {
                mb.prevItem();
                break;
            }
            case KEY_DOWN:
            {
                mb.nextItem();
                break;
            }
            case KEY_LEFT:
            {
                // Previoius menu / Parent-menu
                mb.exitMenu();
                break;
            }
            case KEY_RIGHT:
            {
                // Next menu / Sub-menu
                mb.openMenu();
                break;
            }
            default:
                break;
        }

        refresh();
    }
    clear();
    endwin();
}