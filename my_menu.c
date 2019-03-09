#include <curses.h>
#include <stdlib.h>
#include <string.h>

// Define colors
const short MENU_GENERAL = 1;
const short ITEM_SELECTED = 2;

int eval_menu_width(char menu_name[], 
                    int menu_length,
                    char **menu_items)
{
    int max_item_width = 0;
    // Check menu name
    max_item_width = strlen(menu_name);
    // Check menu items
    for (int item_idx=0; item_idx<menu_length; item_idx++) {
        if (strlen(menu_items[item_idx]) > max_item_width)
            max_item_width = strlen(menu_items[item_idx]);
    }

    return max_item_width;
}
void draw_menu_bg(int menu_length,
                 int menu_width)
{
    start_color();
    init_pair(MENU_GENERAL, COLOR_WHITE, COLOR_BLACK);
    for (int idx = 0; idx <= menu_length; idx++) {
        attron(COLOR_PAIR(MENU_GENERAL));
        mvhline(idx, 0, ' ', menu_width);
        attroff(COLOR_PAIR(MENU_GENERAL));
    }
}

void draw_menu(char menu_name[], 
               int menu_length,
               char **menu_items,
               int menu_pos[2])
{
    const int MENU_WIDTH_OFFSET = 2; 
    int menu_width = MENU_WIDTH_OFFSET;
    int max_item_width = 0;
    int first_item_pos[2] = {menu_pos[0]+1, menu_pos[1]+1};
    // Print menu items
    for (int item_idx=0; item_idx<menu_length; item_idx++) {
        if (strlen(menu_items[item_idx]) > max_item_width)
            max_item_width = strlen(menu_items[item_idx]);
        int item_pos_y = first_item_pos[0]+item_idx;
        int item_pos_x = first_item_pos[1];
        mvaddstr(item_pos_y, item_pos_x, menu_items[item_idx]);
    }
    // Draw lines
    mvhline(menu_pos[0],
            menu_pos[1]+1, ACS_HLINE, max_item_width);
    mvhline(menu_pos[0]+menu_length+1,
            menu_pos[1]+1, ACS_HLINE, max_item_width);
    mvvline(menu_pos[0]+1, 
            menu_pos[1], ACS_VLINE, menu_length);
    mvvline(menu_pos[0]+1, 
            menu_pos[1]+max_item_width+1, ACS_VLINE, menu_length);
    // Draw corners
    mvaddch(menu_pos[0],
            menu_pos[1], ACS_ULCORNER);
    mvaddch(menu_pos[0],
            menu_pos[1]+max_item_width+1, ACS_URCORNER);
    mvaddch(menu_pos[0]+menu_length+1,
            menu_pos[1], ACS_LLCORNER);
    mvaddch(menu_pos[0]+menu_length+1,
            menu_pos[1]+max_item_width+1, ACS_LRCORNER);
}

void draw_menu_hl(char menu_name[], 
               int menu_length,
               char **menu_items,
               int menu_pos[2],
               int cursor_pos[2])
{
    const int MENU_WIDTH_OFFSET = 2; 
    int menu_width = MENU_WIDTH_OFFSET;
    int max_item_width = 0;
    int first_item_pos[2] = {menu_pos[0]+1, menu_pos[1]+1};
    start_color();
    init_pair(MENU_GENERAL, COLOR_WHITE, COLOR_BLACK);
    init_pair(ITEM_SELECTED, COLOR_CYAN, COLOR_BLACK);
    // Print menu items
    for (int item_idx=0; item_idx<menu_length; item_idx++) {
        if (strlen(menu_items[item_idx]) > max_item_width)
            max_item_width = strlen(menu_items[item_idx]);
        int item_pos_y = first_item_pos[0]+item_idx;
        int item_pos_x = first_item_pos[1];
        if ( (cursor_pos[0]==item_pos_y) &&
             (cursor_pos[1]>item_pos_x-1) && 
             (cursor_pos[1]<item_pos_x+max_item_width+1) ) {
            attron(A_BOLD);
            attron(COLOR_PAIR(ITEM_SELECTED));
        }
        else {
          attron(COLOR_PAIR(MENU_GENERAL));
        }
        mvaddstr(item_pos_y, item_pos_x, menu_items[item_idx]);
        if ( (cursor_pos[0]==item_pos_y) &&
             (cursor_pos[1]>item_pos_x-1) && 
             (cursor_pos[1]<item_pos_x+max_item_width+1) ) {
            attroff(A_BOLD);
            attroff(COLOR_PAIR(ITEM_SELECTED));
        }
        else {
          attroff(COLOR_PAIR(MENU_GENERAL));
        }
            
    }
    // Draw lines
    mvhline(menu_pos[0],
            menu_pos[1]+1, ACS_HLINE, max_item_width);
    mvhline(menu_pos[0]+menu_length+1,
            menu_pos[1]+1, ACS_HLINE, max_item_width);
    mvvline(menu_pos[0]+1, 
            menu_pos[1], ACS_VLINE, menu_length);
    mvvline(menu_pos[0]+1, 
            menu_pos[1]+max_item_width+1, ACS_VLINE, menu_length);
    // Draw corners
    mvaddch(menu_pos[0],
            menu_pos[1], ACS_ULCORNER);
    mvaddch(menu_pos[0],
            menu_pos[1]+max_item_width+1, ACS_URCORNER);
    mvaddch(menu_pos[0]+menu_length+1,
            menu_pos[1], ACS_LLCORNER);
    mvaddch(menu_pos[0]+menu_length+1,
            menu_pos[1]+max_item_width+1, ACS_LRCORNER);
}

int* nav_menu(const int cursor_move[2], int min_y, int min_x, int max_y, int max_x)
{
    static int cursor_pos[2] = {0, 0};
    // check if new position is allowed
    int dy = cursor_move[0];
    int dx = cursor_move[1];
    cursor_pos[0] = cursor_pos[0]+dy;
    if (cursor_pos[0]<min_y) cursor_pos[0]=min_y;
    else if (cursor_pos[0]>max_y) cursor_pos[0]=max_y;
    cursor_pos[1] = cursor_pos[1]+dx;
    if (cursor_pos[1]<min_x) cursor_pos[1]=min_x;
    else if (cursor_pos[1]>max_x) cursor_pos[1]=max_x;
    /*
    wmove(stdscr,
          cursor_pos[0],
          cursor_pos[1]);
          */

    // Debug
    char cursor_pos_str[11];
    snprintf(cursor_pos_str, 9, "%5d", cursor_pos[0]);
    mvaddstr(LINES-3, 10, cursor_pos_str);
    snprintf(cursor_pos_str, 9, "%5d", cursor_pos[1]);
    mvaddch(LINES-3, 16, ':');
    mvaddstr(LINES-3, 17, cursor_pos_str);

    return cursor_pos;
}

int main(void)
{
    // Enable colors
    start_color();
    //bkgd(COLOR_PAIR(MENU_GENERAL)); //Breaks item highlighting
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
    
    mvaddstr(MAXLINE, 0, "Press 'q' to quit");
    
    char *menu_name = "Main";
    int menu_length = 6;
    char *main_menu[] = {
        "Temperature",
        "Lights",
        "Doors & Windows",
        "Rooms",
        "Outside",
        "System"
    };
    int menu_pos[2] = {0,0};
    int menu_width = eval_menu_width(menu_name, menu_length, main_menu);

    draw_menu_bg(menu_length, menu_width);
    draw_menu(menu_name, menu_length, main_menu, menu_pos);
    
    int user_input = 112;
    while (user_input != 113) {
        //
        //timeout(-1);
        user_input = getch();
        char user_input_str[8] = {0};
        snprintf(user_input_str, 8, "%5d", user_input);
        mvaddstr(MAXLINE-2, 0, user_input_str);
        switch(user_input) {
            case KEY_UP:
            {
                const int delta[2] = {-1, 0};
                draw_menu_hl(menu_name, menu_length, main_menu, menu_pos, 
                             nav_menu(delta, 1, 1, menu_length, menu_width));
                break;
            }
            case KEY_DOWN:
            {
                const int delta[2] = {+1, 0};
                draw_menu_hl(menu_name, menu_length, main_menu, menu_pos, 
                             nav_menu(delta, 1, 1, menu_length, menu_width));
                break;
            }
            case KEY_LEFT:
            {
                const int delta[2] = { 0,-1};
                draw_menu_hl(menu_name, menu_length, main_menu, menu_pos, 
                             nav_menu(delta, 1, 1, menu_length, menu_width));
                break;
            }
            case KEY_RIGHT:
            {
                const int delta[2] = { 0,+1};
                draw_menu_hl(menu_name, menu_length, main_menu, menu_pos, 
                             nav_menu(delta, 1, 1, menu_length, menu_width));
                break;
            }
            default:
                break;
        }
        refresh();
    }
    endwin();
}
