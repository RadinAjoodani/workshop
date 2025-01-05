#include<ncurses.h>
#include<string.h>
#include<stdlib.h>

typedef struct{
    char password[50];
    char email[50];
    char username[50];
    int score;
    int game;
    int gold;
    int health;
    char weapon;
    char spell;
    char difficulty[200];
    char color[200];
}User;

typedef struct{
    int x,y;
    int color;
    int difficulty;
}Player;

Player l_player;
User l_user;
int is_logged_in=0;

void main_menu();
void log_in();
void sign_up();
int username_check(char *username);
int pass_check(char *password);
int email_check(char *email);
void play_game();
void show_table();
void profile();
void eexit();
void save_user(User *user);
void play_as_guest();
void start_new_game();
void continue_last_game();
void show_score_table();
void show_profile_status();
void settings_menu();
void start_new_game();
void continue_last_game();
void show_profile_status();
void select_difficulty(int *current_difficulty);
void change_character_color(int *current_color);
void save_information(User user);

int main(){
    initscr();        
    noecho();       
    cbreak();         
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_BLACK);
    main_menu();
    getch();
    endwin();
}


void main_menu(){
    curs_set(0);
    char *menu_items[] = {
        "Sign up",
        "Log in",
        "Play game",
        "Play as a guest",
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
            mvprintw(rows - 1, 1, "Logged in as: %s | Score: %d", l_user.username, l_user.score);
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
                    case 0: sign_up(); break;
                    case 1: log_in(); break;
                    case 2: play_game(); break;
                    case 3: play_as_guest(); break;
                    case 4: profile(); break;
                    case 5: show_table(); break;
                    case 6: eexit();return;
                }
                break;
            default:
                break;
        }
    }
}
void sign_up(){
    User s_user;
    int sign=0;
    attron(COLOR_PAIR(3));
    mvprintw(1, 1, "Sign up menu");
    attroff(COLOR_PAIR(3));
    attron(COLOR_PAIR(1));
        while(1){
            mvprintw(2, 1, "Enter your username: ");
            clrtoeol();
            echo();
            getstr(s_user.username);
            noecho();

            if(username_check(s_user.username)){
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
            getstr(s_user.password);
            noecho();

            if(!pass_check(s_user.password)){
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
            getstr(s_user.email);
            noecho();

            if(!email_check(s_user.email)){
                mvprintw(9, 1, "Email format is incorrect. Try again.");
                clrtoeol();
            } else {
                mvprintw(9, 1, "Email is ok");
                clrtoeol();
                sign++;
                break;
            }
            strcpy(s_user.difficulty,"Easy");
            strcpy(s_user.color,"White");
        }
        if(sign==3){
            save_user(&s_user);
            mvprintw(11,1,"ACCOUNT CREATED SUCCESSFULLY");
            mvprintw(12,1,"press Enter to return to menu");
            while(1){
                if(getch()=='\n'){
                    break;
                }
            }
        }
    
}
void log_in(){
    User user;
    int ch; 

    while (1) { 
        clear();
        mvprintw(1, 1, "Log in menu");

        mvprintw(3, 1, "Enter your username: ");
        echo();
        getstr(l_user.username);
        noecho();

        mvprintw(5, 1, "Enter your password: ");
        echo();
        getstr(l_user.password);
        noecho();

        FILE *fptr = fopen("users.txt", "r");
        if (fptr == NULL) {
            mvprintw(7, 1, "Error: Unable to open users file.");
            getch();
            return;
        }

        int valid = 0;
        while (fscanf(fptr, "%s %s %s %d %d %d %s %s", user.username, user.password, user.email, &user.score,&user.gold,&user.game,user.color,user.difficulty) != EOF) {
            if (strcmp(l_user.username, user.username) == 0 && strcmp(l_user.password, user.password) == 0) {
                valid = 1;
                strcpy(l_user.username,user.username);
                strcpy(l_user.password,user.password);
                strcpy(l_user.email,user.email);
                l_user.score=user.score;
                l_user.gold=user.gold;
                strcpy(l_user.color,user.color);                
                strcpy(l_user.difficulty,user.difficulty);
                break;
            }
        }

        fclose(fptr);

        if (valid) {
            mvprintw(7, 1, "Login successful! Press any key to return to the main menu.");
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
void play_game(){
    char *play_menu_items[] = {
        "New Game",
        "Continue Your Last Game",
        "Score Table",
        "Profile Status",
        "Settings",
        "Back to Main Menu"
    };
    int n_items = sizeof(play_menu_items) / sizeof(play_menu_items[0]);
    int choice = 0;
    int key;
    int rows,cols;
    getmaxyx(stdscr, rows, cols);
    while (1) {
        clear();

         if (is_logged_in) {
            mvprintw(0, 1, "Play Game Menu");
            mvprintw(rows - 1, 1, "Logged in as: %s | Score: %d", l_user.username, l_user.score);
        } else {
            mvprintw(LINES/2, COLS/2 -20, "Not logged in. Please log in or sign up.");
            getch();
            return;
        }
        
        for (int i = 0; i < n_items; i++) {
            if (i == choice)
                attron(COLOR_PAIR(2));
            mvprintw(i + 2, 1, "%s", play_menu_items[i]);
            if (i == choice)
                attroff(COLOR_PAIR(2));
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
                    start_new_game();
                    break;
                    case 1: continue_last_game(); break;
                    case 2: show_table(); break;
                    case 3: show_profile_status(); break;
                    case 4: settings_menu(); break;
                    case 5: return;
                }
                break;
            default:
                break;
        }
    }
}
void play_as_guest(){

}
void show_table(){
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

    while (fscanf(file, "%s %s %s %d %d %d", users[user_count].username, users[user_count].password, users[user_count].email, &users[user_count].score,&users[user_count].gold,&users[user_count].game) != EOF) {
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
        if(!strcmp(users[i].username,l_user.username)){
            attron(COLOR_PAIR(2));
        }
        if(i==0){
            attron(A_BOLD);
            mvprintw(3 + i, 1, "%d. %s - Score: %d gold %d: game-played: %d(THE GOAT)", i + 1, users[i].username, users[i].score,users[i].gold,users[i].game);
            attroff(A_BOLD);
        }
        else if(i==1){
            attron(A_BOLD);
            mvprintw(3 + i, 1, "%d. %s - Score: %d gold %d: game-played: %d(ALMOST GOAT)", i + 1, users[i].username, users[i].score,users[i].gold,users[i].game);
            attroff(A_BOLD);
        }
        else if(i==2){
            attron(A_BOLD);
            mvprintw(3 + i, 1, "%d. %s - Score: %d gold %d: game-played: %d(SEMI GOAT)", i + 1, users[i].username, users[i].score,users[i].gold,users[i].game);
            attroff(A_BOLD);
        }
        else {
            
            mvprintw(3 + i, 1, "%d. %s - Score: %d gold %d: game-played: %d", i + 1, users[i].username, users[i].score,users[i].gold,users[i].game);
        }
        if(!strcmp(users[i].username,l_user.username)){
            attroff(COLOR_PAIR(2));
        }
    }

    mvprintw(5 + user_count, 1, "Press any key to return.");
    getch();
}
void profile(){
    if(is_logged_in){
        while(1){
            attron(COLOR_PAIR(4));
            mvprintw(1,1,"Here is your information:");
            mvprintw(3,1,"Name: %s",l_user.username);
            mvprintw(4,1,"Password: %s",l_user.password);
            mvprintw(5,1,"Email: %s",l_user.email);
            mvprintw(6,1,"Score: %d",l_user.score);
            mvprintw(7,1,"Gold: %d",l_user.gold);
            mvprintw(8,1,"Game-played numbers: %d",l_user.game);
            mvprintw(9,1,"Difficulty level: %s",l_user.difficulty);
            mvprintw(10,1,"Player color: %s",l_user.color);

            attroff(COLOR_PAIR(4));
            attron(COLOR_PAIR(2));
            mvprintw(11,1,"press Enter to exit");
            attroff(COLOR_PAIR(2));
            int ch = getch();
            if(ch=='\n'){
                return;
            }
        }
    }
    else{
        mvprintw(1,1,"you are not logged in,press Enter to exit!");
        int ch = getch();
        if(ch=='\n'){
                return;
        }
    }
}
void eexit(){
    mvprintw(1,1,"%s",l_user.username);
    mvprintw(2,1,"%s",l_user.password);
    mvprintw(3,1,"%s",l_user.email);
    mvprintw(4,1,"%d",l_user.score);
    mvprintw(5,1,"%d",l_user.gold);
    mvprintw(6,1,"%s",l_user.difficulty);
    mvprintw(7,1,"%s",l_user.color);
    save_information(l_user);
    mvprintw(22,90,"GOODBYE!");
}
void save_user(User *user) {
    FILE *file = fopen("users.txt", "a");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    fprintf(file, "%s %s %s %d %d %d %s %s\n", user->username, user->password, user->email, user->score,user->gold,user->game,user->color,user->difficulty);
    fclose(file);
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
                attron(COLOR_PAIR(2));
            mvprintw(i + 2, 1, "%s", settings_items[i]);
            if (i == choice)
                attroff(COLOR_PAIR(2));
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
                attron(COLOR_PAIR(2));
            mvprintw(i + 2, 1, "%s%s", difficulty_levels[i], (i == *current_difficulty) ? " (Selected)" : "");
            if (i == choice)
                attroff(COLOR_PAIR(2));
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
                mvprintw(n_levels + 6, 1, "Difficulty set to %s!%d", difficulty_levels[choice],choice);
                strcpy(l_user.difficulty,difficulty_levels[choice]);
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
                strcpy(l_user.color,colors[choice]);
                getch();
                return;
            case 27:  
                return;
        }
    }
}
void start_new_game(){

}
void continue_last_game(){

}
void show_profile_status(){

}
void save_information(User user){
    FILE *reed=fopen("users.txt","r");
    FILE *write=fopen("temp.txt", "w");
    char buffer[1024];
    int found = 0;

    while (fgets(buffer, sizeof(buffer), reed) != NULL) {
        char currentUsername[256], currentPassword[256], currentEmail[256],currentcolor[256],currentdifficulty[256];
        int field1, field2, field3;

         
        if (sscanf(buffer, "%s %s %s %d %d %d %s %s", currentUsername, currentPassword, currentEmail, &field1, &field2, &field3, currentcolor, currentdifficulty) == 8) {
            if (strcmp(currentUsername, l_user.username) == 0) {
                 
                fprintf(write, "%s %s %s %d %d %d %s %s\n", l_user.username, l_user.password, l_user.email, l_user.score, l_user.gold, l_user.game,l_user.color, l_user.difficulty);
                found = 1;
            } else {
                 
                fprintf(write, "%s", buffer);
            }
        } else {
             
            fprintf(write, "%s", buffer);
        }
    }

    fclose(reed);
    fclose(write);
    if (!found) {
    } else {
        if (remove("users.txt") != 0 || rename("temp.txt", "users.txt") != 0) {
            exit(EXIT_FAILURE);
        }
    }
}