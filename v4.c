#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

typedef struct {
    char username[50];
    char password[50];
    char email[100];
    int  score;
} User;

typedef struct {
    int x,y,height,width;
    int door_x, door_y; 
}Room;

typedef struct{
    int x,y;
    char color[15];
}Player;

char current_user[50] = "";   
char current_email[50]="";
char current_pass[50]="";
int current_score = 0;        
int is_logged_in = 0;
Player player;        


void menu_sign_up();
void menu_log_in();
void menu_play_game();
void menu_profile();
void menu_scores();
void menu_exit();
void save_user(User *user);
int username_check(char *username);
int pass_check(char *password);
int email_check(char *email);
void start_new_game();
void continue_last_game();
void show_score_table();
void show_profile_status();
void settings_menu();
void select_difficulty(int *current_difficulty);
void change_character_color(int *current_color);
void create_new_map();
void Play_guest();
void upload_map();
int is_overlapping(Room r1, Room r2);
void draw_room(Room *room);
void draw_player(Player *player);
void move_player(Player *player, Room *room, int dx, int dy);
void main_menu();

int main() {
    initscr();        
    noecho();       
    cbreak();         
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    main_menu();
    endwin();         
    return 0;
}

void main_menu() {
    curs_set(0);
    char *menu_items[] = {
        "Sign up",
        "Log in",
        "Play game",
        "Profile",
        "Scores",
        "Exit"
    };
    int n_items = sizeof(menu_items) / sizeof(menu_items[0]);
    int choice = 0;
    int key;
    int rows, cols;

    while (1) {
        clear();
        getmaxyx(stdscr, rows, cols);
        attron(COLOR_PAIR(3));
        mvprintw(LINES/2,COLS/2 - 4,"ENJOY THE GAME :)");
        attroff(COLOR_PAIR(3));
        for (int i = 0; i < n_items; i++) {
            if (i == choice)
                attron(COLOR_PAIR(2));
            mvprintw(i + 1, 1, "%s", menu_items[i]);
            if (i == choice)
                attroff(COLOR_PAIR(2));
        }

        if (is_logged_in) {
            mvprintw(rows - 1, 1, "Logged in as: %s | Score: %d", current_user, current_score);
        } else {
            mvprintw(rows - 1, 1, "Not logged in. Please log in or sign up.");
        }

        key = getch();

        switch (key) {
            case KEY_UP:
                choice = (choice - 1 + n_items) % n_items;
                break;
            case KEY_DOWN:
                choice = (choice + 1) % n_items;
                break;
            case '\n':
                clear();
                switch (choice) {
                    case 0: menu_sign_up(); break;
                    case 1: menu_log_in(); break;
                    case 2: menu_play_game(); break;
                    case 3: menu_profile(); break;
                    case 4: menu_scores(); break;
                    case 5: menu_exit(); return;
                }
                break;
            default:
                break;
        }
    }
}
void menu_sign_up() {
    int sign = 0;
    char username[100],password[50],email[50];
    mvprintw(1, 1, "Sign up menu");
    
    while(1){
        mvprintw(2, 1, "Enter your username: ");
        clrtoeol();
        echo();
        getstr(username);
        noecho();

        if(username_check(username)){
            mvprintw(3, 1, "Username already exists. Try again.");
            clrtoeol();
        } else {
            mvprintw(3, 1, "Username is ok");
            clrtoeol();
            sign++;
            break;
        }
    }
    while(1){
        mvprintw(5, 1, "Enter your password: ");
        clrtoeol();
        echo();
        getstr(password);
        noecho();

        if(!pass_check(password)){
            mvprintw(6, 1, "Password does not meet requirements. Try again.");
            clrtoeol();
        } else {
            mvprintw(6, 1, "Password is ok");
            clrtoeol();
            sign++;
            break;
        }
    }
    while(1){
        mvprintw(8, 1, "Enter your email: ");
        clrtoeol();
        echo();
        getstr(email);
        noecho();

        if(!email_check(email)){
            mvprintw(9, 1, "Email format is incorrect. Try again.");
            clrtoeol();
        } else {
            mvprintw(9, 1, "Email is ok");
            clrtoeol();
            sign++;
            break;
        }
    }
    if(sign==3){
        User new_user;
        strcpy(new_user.username, username);
        strcpy(new_user.password, password);
        strcpy(new_user.email, email);
        save_user(&new_user);
        mvprintw(11,1,"ACCOUNT CREATED SUCCESSFULLY");
    }
}
void menu_log_in() {
    char username[50], password[50];
    int ch; 

    while (1) { 
        clear();
        mvprintw(1, 1, "Log in menu");

        mvprintw(3, 1, "Enter your username: ");
        echo();
        getstr(username);
        noecho();

        mvprintw(5, 1, "Enter your password: ");
        echo();
        getstr(password);
        noecho();

        FILE *fptr = fopen("users.txt", "r");
        if (fptr == NULL) {
            mvprintw(7, 1, "Error: Unable to open users file.");
            getch();
            return;
        }

        User user;
        int valid = 0;
        while (fscanf(fptr, "%s %s %s %d", user.username, user.password, user.email, &user.score) != EOF) {
            if (strcmp(user.username, username) == 0 && strcmp(user.password, password) == 0) {
                valid = 1;
                strcpy(current_email, user.email);
                strcpy(current_pass, user.password);
                current_score = user.score;
                break;
            }
        }

        fclose(fptr);

        if (valid) {
            mvprintw(7, 1, "Login successful! Press any key to return to the main menu.");
            strcpy(current_user,username);
            is_logged_in = 1;
            getch();
            return;
        } else {
            mvprintw(7, 1, "Invalid username or password!");
            mvprintw(9, 1, "Press 'r' to retry or any other key to return to the main menu.");
            ch = getch();

            if (ch != 'r' && ch != 'R') {
                return;
            }
        }
    }
}
void menu_play_game() {
    char *play_menu_items[] = {
        "New Game",
        "Continue Your Last Game",
        "Play as a guest",
        "Score Table",
        "Profile Status",
        "Settings",
        "Back to Main Menu"
    };
    int n_items = sizeof(play_menu_items) / sizeof(play_menu_items[0]);
    int choice = 0;
    int key;

    while (1) {
        clear();

        mvprintw(0, 1, "Play Game Menu");

        for (int i = 0; i < n_items; i++) {
            if (i == choice)
                attron(A_REVERSE);
            mvprintw(i + 2, 1, "%s", play_menu_items[i]);
            if (i == choice)
                attroff(A_REVERSE);
        }

        key = getch();

        switch (key) {
            case KEY_UP:
                choice = (choice - 1 + n_items) % n_items;
                break;
            case KEY_DOWN:
                choice = (choice + 1) % n_items;
                break;
            case '\n':
                switch (choice) {
                    case 0: start_new_game(); break;
                    case 1: continue_last_game(); break;
                    case 2: Play_guest(); break;
                    case 3: show_score_table(); break;
                    case 4: show_profile_status(); break;
                    case 5: settings_menu(); break;
                    case 6: return;
                }
                break;
            default:
                break;
        }
    }
}
void start_new_game() {
    clear();
    initscr();
    create_new_map();
}
void Play_guest(){
    clear();
    initscr();
    create_new_map();
}
void continue_last_game() {
    clear();
    mvprintw(1, 1, "Continuing your last game...");
    mvprintw(3, 1, "Press any key to return.");
    getch();
}
void show_score_table() {
    clear();
    mvprintw(1, 1, "Score Table:");

    FILE *file = fopen("users.txt", "r");
    if (!file) {
        mvprintw(3, 1, "Error: Unable to open users file.");
        getch();
        return;
    }

    User users[100];
    int user_count = 0;

    while (fscanf(file, "%s %s %s %d", users[user_count].username, users[user_count].password, users[user_count].email, &users[user_count].score) != EOF) {
        user_count++;
    }
    fclose(file);

    for (int i = 0; i < user_count - 1; i++) {
        for (int j = 0; j < user_count - i - 1; j++) {
            if (users[j].score < users[j + 1].score) {
                User temp = users[j];
                users[j] = users[j + 1];
                users[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < user_count; i++) {
        if (i < 3) {
            attron(A_BOLD);
        }
        mvprintw(3 + i, 1, "%d. %s - Score: %d", i + 1, users[i].username, users[i].score);
        if (i < 3) {
            attroff(A_BOLD);
        }
    }

    mvprintw(5 + user_count, 1, "Press any key to return.");
    getch();
}
void show_profile_status() {
    clear();
    if (!is_logged_in) {
        mvprintw(1, 1, "You are not logged in!");
    } else {
        mvprintw(1, 1, "Profile Status:");
        mvprintw(3, 1, "Username: %s", current_user);
        mvprintw(4, 1, "Email: %s",current_email);
        mvprintw(5, 1, "password: %s",current_pass);
        mvprintw(6, 1, "Score: %d", current_score);
    }
    mvprintw(7, 1, "Press any key to return.");
    getch();
}
void settings_menu() {
    char *settings_items[] = {
        "Select Difficulty Level",
        "Change Character Color",
        "Back to Play Menu"
    };
    int n_items = sizeof(settings_items) / sizeof(settings_items[0]);
    int choice = 0;
    int key;

    static int current_difficulty = 0;  
    static int current_color = 1;       

    while (1) {
        clear();
        mvprintw(0, 1, "Settings Menu");

        for (int i = 0; i < n_items; i++) {
            if (i == choice)
                attron(A_REVERSE);
            mvprintw(i + 2, 1, "%s", settings_items[i]);
            if (i == choice)
                attroff(A_REVERSE);
        }

        key = getch();

        switch (key) {
            case KEY_UP:
                choice = (choice - 1 + n_items) % n_items;
                break;
            case KEY_DOWN:
                choice = (choice + 1) % n_items;
                break;
            case '\n':
                switch (choice) {
                    case 0:  
                        select_difficulty(&current_difficulty);
                        break;
                    case 1:  
                        change_character_color(&current_color);
                        break;
                    case 2:  
                        return;
                }
                break;
            default:
                break;
        }
    }
}
void select_difficulty(int *current_difficulty) {
    char *difficulty_levels[] = {
        "Easy",
        "Medium",
        "Hard"
    };
    int n_levels = sizeof(difficulty_levels) / sizeof(difficulty_levels[0]);
    int choice = *current_difficulty;
    int key;

    while (1) {
        clear();
        mvprintw(0, 1, "Select Difficulty Level");

        for (int i = 0; i < n_levels; i++) {
            if (i == choice)
                attron(A_REVERSE);
            mvprintw(i + 2, 1, "%s%s", difficulty_levels[i], (i == *current_difficulty) ? " (Selected)" : "");
            if (i == choice)
                attroff(A_REVERSE);
        }

        mvprintw(n_levels + 4, 1, "Press ENTER to confirm, or ESC to cancel.");

        key = getch();

        switch (key) {
            case KEY_UP:
                choice = (choice - 1 + n_levels) % n_levels;
                break;
            case KEY_DOWN:
                choice = (choice + 1) % n_levels;
                break;
            case '\n':
                *current_difficulty = choice;
                mvprintw(n_levels + 6, 1, "Difficulty set to %s!", difficulty_levels[choice]);
                getch();
                return;
            case 27:  
                return;
        }
    }
}
void change_character_color(int *current_color) {
    char *colors[] = {
        "White",
        "Red",
        "Green",
        "Blue",
        "Yellow"
    };
    int n_colors = sizeof(colors) / sizeof(colors[0]);
    int choice = *current_color;
    int key;

    while (1) {
        clear();
        mvprintw(0, 1, "Change Character Color");

        for (int i = 0; i < n_colors; i++) {
            if (i == choice)
                attron(A_REVERSE);
            mvprintw(i + 2, 1, "%s%s", colors[i], (i == *current_color) ? " (Selected)" : "");
            if (i == choice)
                attroff(A_REVERSE);
        }

        mvprintw(n_colors + 4, 1, "Press ENTER to confirm, or ESC to cancel.");

        key = getch();

        switch (key) {
            case KEY_UP:
                choice = (choice - 1 + n_colors) % n_colors;
                break;
            case KEY_DOWN:
                choice = (choice + 1) % n_colors;
                break;
            case '\n':
                *current_color = choice;
                strcpy(player.color,colors[choice]);
                mvprintw(n_colors + 6, 1, "Color set to %s!", colors[choice]);
                getch();
                return;
            case 27:  
                return;
        }
    }
}
void menu_profile() {
    mvprintw(1, 1, "Profile menu");
    mvprintw(3, 1, "Press any key to return to the main menu...");
    getch();
}
void menu_scores() {
    mvprintw(1, 1, "Scores menu");
    mvprintw(3, 1, "Press any key to return to the main menu...");
    getch();
}
void menu_exit() {
    mvprintw(1, 1, "Exiting the program...");
    getch();
}
int username_check(char *username){
    FILE *fptr = fopen("users.txt","r");
    if (fptr == NULL) {
        return 0;
    }
    User user;
    while (fscanf(fptr, "%s %s %s", user.username, user.password, user.email) != EOF) {
        if (strcmp(user.username, username) == 0) {
            fclose(fptr);
            return 1;
        }
    }
    fclose(fptr);
    return 0;
}
int pass_check(char *password){
    int sign1=0;
    int sign2=0;
    int sign3=0;
    int sign4=0;
    int index = 0;
    while(password[index]!='\0'){
        if(password[index]>='A'&&password[index]<='Z'){
            sign1=1;
        }
        if(strlen(password)>=7){
            sign2=1;
        }
        if(password[index]>='0'&&password[index]<='9'){
            sign3=1;
        }
        if(password[index]>='a'&&password[index]<='z'){
            sign4=1;
        }
        index++;
    }
    return sign1 && sign2 && sign3 && sign4;
}
int email_check(char *email){
    int sign1=0;
    int sign2=0;
    int sign3=0;
    int index = 0;
    char *at = strchr(email, '@');
    char *dot = strrchr(email, '.');
    if(at == NULL || dot == NULL || at > dot){
        return 0;
    }
    for(int i = 0 ; i < at - email ; i++){
        if((email[i]>='A'&&email[i]<='Z')||(email[i]>='a'&&email[i]<='z')){
             sign1=1;
             break;
        }
    }
    for(int i = at - email + 1 ; i < dot - email; i++){
        if((email[i]>='A'&&email[i]<='Z')||(email[i]>='a'&&email[i]<='z')){
             sign2=1;
             break;
        }
    }
    for(int i = dot - email + 1; email[i]!='\0'; i++){
        if((email[i]>='A'&&email[i]<='Z')||(email[i]>='a'&&email[i]<='z')){
             sign3=1;
             break;
        }
    }
    return sign1 && sign2 && sign3;
}
void save_user(User *user) {
    FILE *file = fopen("users.txt", "a");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    fprintf(file, "%s %s %s 0\n", user->username, user->password, user->email);
    fclose(file);
}
void create_new_map(){
    initscr();
    noecho();
    curs_set(0);

    srand(time(NULL));

    Room rooms[6];
    int room_count = 0;

     
    while (room_count < 6) {
        int width = (rand() % 6) + 4;   
        int height = (rand() % 6) + 4;  
        int x = rand() % (LINES - height - 3) + 3;
        int y = rand() % (COLS - width - 3) + 3;

        Room new_room = {x, y, width, height, -1, -1};
        int overlap = 0;

        for (int i = 0; i < room_count; i++) {
            if (is_overlapping(rooms[i], new_room)) {
                overlap = 1;
                break;
            }
        }

        if (!overlap) {
            rooms[room_count++] = new_room;
            draw_room(&new_room);
        }
    }

     
    int room_index = rand() % room_count;
    Player player = {rooms[room_index].x + 1, rooms[room_index].y + 1};
    draw_player(&player);

    refresh();

     
    int ch;
    while ((ch = getch()) != 'q') {
        mvprintw(player.x, player.y, ".");  
        switch (ch) {
            case '2': move_player(&player, &rooms[room_index], 1, 0); break;   
            case '8': move_player(&player, &rooms[room_index], -1, 0); break;  
            case '4': move_player(&player, &rooms[room_index], 0, -1); break;  
            case '6': move_player(&player, &rooms[room_index], 0, 1); break;   
            case '7': move_player(&player, &rooms[room_index], -1, -1); break;  
            case '9': move_player(&player, &rooms[room_index], -1, 1); break;   
            case '1': move_player(&player, &rooms[room_index], 1, -1); break;   
            case '3': move_player(&player, &rooms[room_index], 1, 1); break;    
        }
        draw_player(&player);  
        refresh();
    }
}
int is_overlapping(Room r1, Room r2) {
    return !((r1.x + r1.height + 3 <= r2.x) ||
             (r2.x + r2.height + 3 <= r1.x) ||
             (r1.y + r1.width + 3 <= r2.y)  ||
             (r2.y + r2.width + 3 <= r1.y));  
}
void draw_room(Room *room) {
     
    for (int i = room->x; i < room->x + room->height; i++) {
        mvprintw(i, room->y, "|");
        mvprintw(i, room->y + room->width, "|");
    }

     
    for (int i = room->y; i < room->y + room->width; i++) {
        mvprintw(room->x - 1, i, "--");
        mvprintw(room->x + room->height, i, "--");
    }

     
    for (int i = room->x; i < room->x + room->height; i++) {
        for (int j = room->y + 1; j < room->y + room->width; j++) {
            mvprintw(i, j, ".");
        }
    }

     
    int column_count = (rand() % 2);  
    for (int i = 0; i < column_count; i++) {
        int col_x = rand() % (room->height - 1) + room->x;
        int col_y = rand() % (room->width - 1) + room->y + 1;
        mvprintw(col_x, col_y, "O");
    }

     
    int window_count = (rand() % 3) + 2;  
    for (int i = 0; i < window_count; i++) {
        int wall = rand() % 4;
        if (wall == 0) {  
            int wx = room->x - 1;
            int wy = rand() % (room->width - 1) + room->y;
            mvprintw(wx, wy, "=");
        } else if (wall == 1) {  
            int wx = room->x + room->height;
            int wy = rand() % (room->width - 1) + room->y;
            mvprintw(wx, wy, "=");
        } else if (wall == 2) {  
            int wx = rand() % (room->height - 1) + room->x;
            int wy = room->y;
            mvprintw(wx, wy, "=");
        } else {  
            int wx = rand() % (room->height - 1) + room->x;
            int wy = room->y + room->width;
            mvprintw(wx, wy, "=");
        }
    }

     
    int door_wall = rand() % 4;  
    if (door_wall == 0) {  
        room->door_y = rand() % (room->width - 1) + room->y;
        room->door_x = room->x - 1;
        mvprintw(room->door_x, room->door_y, "+");
    } else if (door_wall == 1) {  
        room->door_y = rand() % (room->width - 1) + room->y;
        room->door_x = room->x + room->height;
        mvprintw(room->door_x, room->door_y, "+");
    } else if (door_wall == 2) {  
        room->door_x = rand() % (room->height) + room->x;
        room->door_y = room->y;
        mvprintw(room->door_x, room->door_y, "+");
    } else if (door_wall == 3) {  
        room->door_x = rand() % (room->height) + room->x;
        room->door_y = room->y + room->width;
        mvprintw(room->door_x, room->door_y, "+");
    }
}
void draw_player(Player *player) {
    mvprintw(player->x, player->y, "@");
}
void move_player(Player *player, Room *room, int dx, int dy) {
    int new_x = player->x + dx;
    int new_y = player->y + dy;

     
    char next_char = mvinch(new_x, new_y) & A_CHARTEXT;

     
    if (next_char == '.' || next_char == '+') {
        player->x = new_x;
        player->y = new_y;
    } else if (next_char == '|') {
         
        if ((new_x == room->door_x && new_y == room->door_y) ||
            (abs(new_x - room->door_x) + abs(new_y - room->door_y) == 1)) {
            player->x = new_x;
            player->y = new_y;
        }
    }
}