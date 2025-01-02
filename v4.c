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
}Room;

char current_user[50] = "";   
char current_email[50]="";
char current_pass[50]="";
int current_score = 0;        
int is_logged_in = 0;         


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
void draw_room(Room room);

void main_menu();

int main() {
    initscr();        
    noecho();       
    cbreak();         
    keypad(stdscr, TRUE); 
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

        for (int i = 0; i < n_items; i++) {
            if (i == choice)
                attron(A_REVERSE);
            mvprintw(i + 1, 1, "%s", menu_items[i]);
            if (i == choice)
                attroff(A_REVERSE);
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
        curs_set(0);

    srand(time(NULL));

    Room rooms[6];
    int room_count = 0;

    while (room_count < 6) {
        int width = (rand() % 5) + 5;
        int height = (rand() % 5) + 5;
        int x = rand() % (LINES - height - 3) + 3;
        int y = rand() % (COLS - width - 3) + 3;
        if (width > COLS - 2) {
            width = COLS - 2;
        }
        if (height > LINES - 2) {
            height = LINES - 2;
        }

        Room new_room = {x, y, width, height};
        int overlap = 0;
        for (int i = 0; i < room_count; i++) {
            if (is_overlapping(rooms[i], new_room)) {
                overlap = 1;
                break;
            }
        }

        if (!overlap) {
            rooms[room_count++] = new_room;
            draw_room(new_room);
        }
    }
    getch();
}
int is_overlapping(Room r1, Room r2) {
    return !((r1.x + r1.height + 3 <= r2.x) ||
             (r2.x + r2.height + 3 <= r1.x) ||
             (r1.y + r1.width + 3 <= r2.y)  ||
             (r2.y + r2.width + 3 <= r1.y));  
}
void draw_room(Room room) {
    for (int i = room.x; i < room.x + room.height; i++) {
        mvprintw(i, room.y -1, "|");
        mvprintw(i, room.y + room.width, "|");
    }
    for (int i = room.x; i < room.x + room.height; i++) {
        for (int j = room.y + 1; j < room.y + room.width; j++) {
            mvprintw(i, j, ".");
        }
    }
    for (int i = room.y; i < room.y + room.width; i++) {
        mvprintw(room.x - 1, i, "_");
        mvprintw(room.x + room.height, i, "_");
    }
    int num_columns = (rand() % 3) + 1;   
    for (int i = 0; i < num_columns; i++) {
        int col_x = rand() % room.height + room.x;
        int col_y = rand() % (room.width - 1) + room.y + 1;
        mvprintw(col_x, col_y, "O");
    }

      
int num_windows = (rand() % 3);
for (int i = 0; i < num_windows; i++) {
    int wall = rand() % 4; 
    if (wall == 0) {
        int win_y = rand() % (room.width - 1) + room.y;
        mvprintw(room.x - 1, win_y, "=");
    } else if (wall == 1) {
        int win_y = rand() % (room.width - 1) + room.y;
        mvprintw(room.x + room.height, win_y, "=");
    } else if (wall == 2) {
        int win_x = rand() % (room.height) + room.x;
        mvprintw(win_x, room.y-1, "=");
    } else if (wall == 3) {
        int win_x = rand() % (room.height) + room.x;
        mvprintw(win_x, room.y + room.width, "=");
    }
}


      
    int door_wall = rand() % 3;   
    if (door_wall == 0) {   
        int door_y = rand() % (room.width - 1) + room.y + 1;
        mvprintw(room.x - 1, door_y, "+");
    } else if (door_wall == 1) {   
        int door_y = rand() % (room.width - 1) + room.y + 1;
        mvprintw(room.x + room.height, door_y, "+");
    } else if (door_wall == 2) {   
        int door_x = rand() % room.height + room.x;
        mvprintw(door_x, room.y-1, "+");
    } else if (door_wall == 3) {   
        int door_x = rand() % room.height + room.x;
        mvprintw(door_x, room.y + room.width, "+");
    }
}