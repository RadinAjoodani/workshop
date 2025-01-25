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
    int difficulty;
    int color;
}User;

typedef struct{
    int x,y;
    int color;
    int difficulty;
    char prev_char;
}Player;
int fmsign=0;
Player l_player;
User l_user;
User s_user;
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
void save_user();
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
int create_level1();
int create_level2();
int create_level3();
int create_level4();
int is_wall(int x, int y);
void draw_path(int x1, int y1, int x2, int y2);
void draw_player(Player *player);
int handle_input(Player *player);
void clear_player(Player *player);
int is_valid_move(int x, int y);
// void save_information(User user);

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
    init_pair(5, COLOR_BLUE, COLOR_BLACK);
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
        for(int i = LINES/2-4 ; i < LINES/2+5 ; i++ ){
            mvprintw(i,COLS/2-17,"||");
            mvprintw(i,COLS/2+12,"||");
        }
        mvprintw(LINES/2+5,COLS/2-14,"=========================");
        mvprintw(LINES/2-5,COLS/2-14,"=========================");
        attroff(COLOR_PAIR(3));
        attron(COLOR_PAIR(2));
        mvprintw(LINES/2,COLS/2-10,"ENJOY THE GAME :)");
        attroff(COLOR_PAIR(2));
        
        // mvprintw(LINES/2,1,"%d %d",LINES,COLS);
        
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
                mvprintw(4, 1, "press q to exit");
                if(getch()=='q'){
                    return;
                }
                else{
                    move(3, 1);
                    clrtoeol();
                    move(4, 1);
                    clrtoeol();
                }
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
                mvprintw(7, 1, "press q to exit");
                if(getch()=='q'){
                    return;
                }
                else{
                    move(6, 1);
                    clrtoeol();
                    move(7, 1);
                    clrtoeol();
                }
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
                mvprintw(10, 1, "press q to exit");
                if(getch()=='q'){
                    return;
                }
                else{
                    move(9, 1);
                    clrtoeol();
                    move(10, 1);
                    clrtoeol();
                }
            } else {
                mvprintw(9, 1, "Email is ok");
                clrtoeol();
                sign++;
                break;
            }
            s_user.game=0;
            s_user.gold=0;
            s_user.score=0;
            s_user.difficulty=0;
            s_user.color=2;
        }
        if(sign==3){
            save_user();
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
    if(is_logged_in==1){
        mvprintw(1,1,"You are logged in!press any key to exit.");
        getch();
        return;
    }
    else{
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
            while (fscanf(fptr, "%s %s %s %d %d %d %d %d", user.username, user.password, user.email, &user.score,&user.gold,&user.game,&user.color,&user.difficulty) != EOF) {
                if (strcmp(l_user.username, user.username) == 0 && strcmp(l_user.password, user.password) == 0) {
                    valid = 1;
                    strcpy(l_user.username,user.username);
                    strcpy(l_user.password,user.password);
                    strcpy(l_user.email,user.email);
                    l_user.score=user.score;
                    l_user.gold=user.gold;
                    l_user.game=user.game;
                    l_user.color=user.color;                
                    l_user.difficulty=user.difficulty;
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
}
int username_check(char *username){
    FILE *fptr = fopen("users.txt","r");
    if (fptr == NULL) {
        return 0;
    }
    User user;
    while (fscanf(fptr, "%s %s %s %d %d %d %d %d", user.username, user.password, user.email, &user.score,&user.gold,&user.game,&user.color,&user.difficulty) != EOF) {
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
                    case 3: profile(); break;
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
    while(1){
        create_level1();
        if(getch()=='q'){
        return;
        }
    }
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

    while (fscanf(file, "%s %s %s %d %d %d %d %d", users[user_count].username, users[user_count].password, users[user_count].email, &users[user_count].score,&users[user_count].gold,&users[user_count].game,&users[user_count].color,&users[user_count].difficulty) != EOF) {
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
    mvprintw(22,90,"GOODBYE!");
    getch();
    return;
}
void save_user() {
    FILE *file = fopen("users.txt", "a");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    fprintf(file, "%s %s %s %d %d %d %d %d\n", s_user.username, s_user.password, s_user.email, s_user.score,s_user.gold,s_user.game,s_user.color,s_user.difficulty);
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
                mvprintw(n_levels + 6, 1, "Difficulty set to %s!", difficulty_levels[choice]);
                getch();
                if(strcmp(difficulty_levels[choice],"Easy")==0){
                    l_user.difficulty=0;
                }
                else if(strcmp(difficulty_levels[choice],"Medium")==0){
                    l_user.difficulty=1;
                }
                else if(strcmp(difficulty_levels[choice],"Hard")==0){
                    l_user.difficulty=2;
                }
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
                attron(COLOR_PAIR(2));
            mvprintw(i + 2, 1, "%s%s", colors[i], (i == *current_color) ? " (Selected)" : "");
            if (i == choice)
                attroff(COLOR_PAIR(2));
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
                if(strcmp(colors[choice],"White")==0){
                    l_user.color=4;
                }
                else if(strcmp(colors[choice],"Red")==0){
                    l_user.color=2;
                }
                else if(strcmp(colors[choice],"Green")==0){
                    l_user.color=3;
                }
                else if(strcmp(colors[choice],"Blue")==0){
                    l_user.color=5;
                }
                else if(strcmp(colors[choice],"Yellow")==0){
                    l_user.color=1;
                }
                return;
            case 27:  
                return;
        }
    }
}
void start_new_game(){
    clear();
    while(1){
        create_level1();
        if(getch()=='q'){
        return;
        }
    }
}
void continue_last_game(){

}
int create_level1(){
    clear();
        mvprintw(1,1,"LEVEL1");
        //room1
        for(int i = 3 ; i < 10 ; i++){
            mvprintw(i,3,"|");
            mvprintw(i,10,"|");
        }
        for(int i = 3 ; i < 10 ; i++){
            mvprintw(3,i,"--");
            mvprintw(9,i,"--");
        }
        for(int i = 4 ; i < 9 ; i++){
            for(int j = 4 ; j < 10 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(6,10,"+");
        mvprintw(7,5,"o");
        mvprintw(4,8,"o");
        //room2
        for(int i = 4 ; i < 11 ; i++){
            mvprintw(i,40,"|");
            mvprintw(i,48,"|");
        }
        for(int i = 40 ; i < 48 ; i++){
            mvprintw(4,i,"--");
            mvprintw(10,i,"--");
        }
        for(int i = 5 ; i < 10 ; i++){
            for(int j = 41 ; j < 48 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(9,40,"+");
        mvprintw(7,48,"+");
        mvprintw(6,45,"o");
        mvprintw(8,41,"o");
        //room3
        for(int i = 6 ; i < 17 ; i++){
            mvprintw(i,100,"|");
            mvprintw(i,111,"|");
        }
        for(int i = 100 ; i < 111 ; i++){
            mvprintw(6,i,"--");
            mvprintw(16,i,"--");
        }
        for(int i = 7 ; i < 16 ; i++){
            for(int j = 101 ; j < 111 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(9,100,"+");
        mvprintw(14,111,"+");
        mvprintw(10,105,"o");
        mvprintw(14,102,"o");
        mvprintw(15,109,"o");
        //room4
        for(int i = 20 ; i < 30 ; i++){
            mvprintw(i,150,"|");
            mvprintw(i,159,"|");
        }
        for(int i = 150 ; i < 159 ; i++){
            mvprintw(20,i,"--");
            mvprintw(29,i,"--");
        }
        for(int i = 21 ; i < 29 ; i++){
            for(int j = 151 ; j < 159 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(20,152,"+");
        mvprintw(25,150,"+");
        mvprintw(22,152,"o");
        mvprintw(28,156,"o");
        //room5
        for(int i = 30 ; i < 36 ; i++){
            mvprintw(i,94,"|");
            mvprintw(i,110,"|");
        }
        for(int i = 94 ; i < 110 ; i++){
            mvprintw(30,i,"--");
            mvprintw(35,i,"--");
        }
        for(int i = 31 ; i < 35 ; i++){
            for(int j = 95 ; j < 110 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(32,94,"+");
        mvprintw(34,110,"+");
        mvprintw(31,99,"o");
        mvprintw(33,107,"o");
        //room6
        for(int i = 28 ; i < 38 ; i++){
            mvprintw(i,10,"|");
            mvprintw(i,20,"|");
        }
        for(int i = 10 ; i < 20 ; i++){
            mvprintw(28,i,"--");
            mvprintw(37,i,"--");
        }
        for(int i = 29 ; i < 37 ; i++){
            for(int j = 11 ; j < 20 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(30,20,"+");
        mvprintw(30,12,"o");
        mvprintw(36,17,"o");
        
        draw_path(11, 6, 40, 9);
        draw_path(49, 7, 100, 9);
        draw_path(112, 14, 153, 19);
        draw_path(149, 25, 110, 34);
        draw_path(93, 32, 20, 30);
        Player player = {5, 4, '.'};
        draw_player(&player);

        refresh();

        while (1) {
            handle_input(&player);
            refresh();
        }
}
int create_level2(){
    clear();
    mvprintw(1,1,"LEVEL2");
        //room1
        for(int i = 6 ; i < 15 ; i++){
            mvprintw(i,10,"|");
            mvprintw(i,18,"|");
        }
        for(int i = 10 ; i < 18 ; i++){
            mvprintw(6,i,"--");
            mvprintw(14,i,"--");
        }
        for(int i = 7 ; i < 14 ; i++){
            for(int j = 11 ; j < 18 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(7,18,"+");
        mvprintw(10,12,"o");
        mvprintw(8,16,"o");
        //room2
        for(int i = 5 ; i < 14 ; i++){
            mvprintw(i,35,"|");
            mvprintw(i,48,"|");
        }
        for(int i = 35 ; i < 48 ; i++){
            mvprintw(5,i,"--");
            mvprintw(13,i,"--");
        }
        for(int i = 6 ; i < 13 ; i++){
            for(int j = 36 ; j < 48 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(12,35,"+");
        mvprintw(9,48,"+");
        mvprintw(11,45,"o");
        mvprintw(9,37,"o");
        //room3
        for(int i = 3 ; i < 13 ; i++){
            mvprintw(i,120,"|");
            mvprintw(i,135,"|");
        }
        for(int i = 120 ; i < 135 ; i++){
            mvprintw(3,i,"--");
            mvprintw(12,i,"--");
        }
        for(int i = 4 ; i < 12 ; i++){
            for(int j = 121 ; j < 135 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(7,120,"+");
        mvprintw(12,127,"+");
        mvprintw(5,126,"o");
        mvprintw(6,131,"o");
        mvprintw(11,122,"o");
        //room4
        for(int i = 30 ; i < 42 ; i++){
            mvprintw(i,160,"|");
            mvprintw(i,175,"|");
        }
        for(int i = 160 ; i < 175 ; i++){
            mvprintw(30,i,"--");
            mvprintw(41,i,"--");
        }
        for(int i = 31 ; i < 41 ; i++){
            for(int j = 161 ; j < 175 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(30,167,"+");
        mvprintw(39,160,"+");
        mvprintw(35,169,"o");
        mvprintw(40,163,"o");
        //room5
        for(int i = 30 ; i < 36 ; i++){
            mvprintw(i,94,"|");
            mvprintw(i,110,"|");
        }
        for(int i = 94 ; i < 110 ; i++){
            mvprintw(30,i,"--");
            mvprintw(35,i,"--");
        }
        for(int i = 31 ; i < 35 ; i++){
            for(int j = 95 ; j < 110 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(32,94,"+");
        mvprintw(34,110,"+");
        mvprintw(31,99,"o");
        mvprintw(33,107,"o");
        //room6
        for(int i = 15 ; i < 23 ; i++){
            mvprintw(i,50,"|");
            mvprintw(i,60,"|");
        }
        for(int i = 50 ; i < 60 ; i++){
            mvprintw(15,i,"--");
            mvprintw(22,i,"--");
        }
        for(int i = 16 ; i < 22 ; i++){
            for(int j = 51 ; j < 60 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(17,60,"+");
        mvprintw(21,50,"+");
        mvprintw(20,53,"o");
        mvprintw(19,57,"o");
        //room7
        for(int i = 28 ; i < 33 ; i++){
            mvprintw(i,11,"|");
            mvprintw(i,26,"|");
        }
        for(int i = 11 ; i < 26 ; i++){
            mvprintw(28,i,"--");
            mvprintw(32,i,"--");
        }
        for(int i = 29 ; i < 32 ; i++){
            for(int j = 12 ; j < 26 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(28,20,"+");
        mvprintw(30,12,"o");
        mvprintw(29,17,"o");
        draw_path(19, 7, 34, 12);
        draw_path(49, 9, 120, 7);
        draw_path(127, 13, 168, 29);
        draw_path(159, 39, 110, 34);
        draw_path(93, 32, 60, 17);
        draw_path(49, 21, 19, 27);
        if(getch()=='q'){
            return 0;
        }
        else{
            create_level3();
        }
}
int create_level3(){
    clear();
    mvprintw(1,1,"LEVEL3");
        //room1
        for(int i = 6 ; i < 16 ; i++){
            mvprintw(i,35,"|");
            mvprintw(i,41,"|");
        }
        for(int i = 35 ; i < 41 ; i++){
            mvprintw(6,i,"--");
            mvprintw(15,i,"--");
        }
        for(int i = 7 ; i < 15 ; i++){
            for(int j = 36 ; j < 41 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(12,41,"+");
        mvprintw(11,40,"o");
        mvprintw(9,37,"o");
        //room2
        for(int i = 12 ; i < 17 ; i++){
            mvprintw(i,70,"|");
            mvprintw(i,84,"|");
        }
        for(int i = 70 ; i < 84 ; i++){
            mvprintw(12,i,"--");
            mvprintw(16,i,"--");
        }
        for(int i = 13 ; i < 16 ; i++){
            for(int j = 71 ; j < 84 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(13,70,"+");
        mvprintw(14,84,"+");
        mvprintw(13,75,"o");
        mvprintw(15,80,"o");
        //room3
        for(int i = 2 ; i < 9 ; i++){
            mvprintw(i,150,"|");
            mvprintw(i,160,"|");
        }
        for(int i = 150 ; i < 160; i++){
            mvprintw(2,i,"--");
            mvprintw(8,i,"--");
        }
        for(int i = 3 ; i < 8 ; i++){
            for(int j = 151 ; j < 160 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(6,150,"+");
        mvprintw(8,158,"+");
        mvprintw(4,155,"o");
        mvprintw(7,159,"o");
        mvprintw(4,151,"o");
        //room4
        for(int i = 30 ; i < 42 ; i++){
            mvprintw(i,160,"|");
            mvprintw(i,175,"|");
        }
        for(int i = 160 ; i < 175 ; i++){
            mvprintw(30,i,"--");
            mvprintw(41,i,"--");
        }
        for(int i = 31 ; i < 41 ; i++){
            for(int j = 161 ; j < 175 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(30,167,"+");
        mvprintw(39,160,"+");
        mvprintw(35,169,"o");
        mvprintw(40,163,"o");
        //room5
        for(int i = 30 ; i < 36 ; i++){
            mvprintw(i,84,"|");
            mvprintw(i,100,"|");
        }
        for(int i = 84 ; i < 100 ; i++){
            mvprintw(30,i,"--");
            mvprintw(35,i,"--");
        }
        for(int i = 31 ; i < 35 ; i++){
            for(int j = 85 ; j < 100 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(32,84,"+");
        mvprintw(34,100,"+");
        mvprintw(31,99,"o");
        mvprintw(33,86,"o");
        //room6
        for(int i = 35 ; i < 40 ; i++){
            mvprintw(i,17,"|");
            mvprintw(i,26,"|");
        }
        for(int i = 17 ; i < 26 ; i++){
            mvprintw(35,i,"--");
            mvprintw(39,i,"--");
        }
        for(int i = 36 ; i < 39 ; i++){
            for(int j = 18 ; j < 26 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(37,26,"+");
        mvprintw(37,22,"o");
        draw_path(27, 37, 84, 32);
        draw_path(101, 34, 160, 39);
        draw_path(167, 29, 158, 8);
        draw_path(149, 6, 84, 14);
        draw_path(69, 13, 41, 12);
        if(getch()=='q'){
            return 0;
        }
        else{
            create_level4();
        }
}
int create_level4(){
    clear();
    mvprintw(1,1,"LEVEL4");
        //room1
        for(int i = 6 ; i < 15 ; i++){
            mvprintw(i,10,"|");
            mvprintw(i,18,"|");
        }
        for(int i = 10 ; i < 18 ; i++){
            mvprintw(6,i,"--");
            mvprintw(14,i,"--");
        }
        for(int i = 7 ; i < 14 ; i++){
            for(int j = 11 ; j < 18 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(7,18,"+");
        mvprintw(10,12,"o");
        mvprintw(8,16,"o");
        //room2
        for(int i = 5 ; i < 14 ; i++){
            mvprintw(i,35,"|");
            mvprintw(i,48,"|");
        }
        for(int i = 35 ; i < 48 ; i++){
            mvprintw(5,i,"--");
            mvprintw(13,i,"--");
        }
        for(int i = 6 ; i < 13 ; i++){
            for(int j = 36 ; j < 48 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(12,35,"+");
        mvprintw(9,48,"+");
        mvprintw(11,45,"o");
        mvprintw(9,37,"o");
        //room3
        for(int i = 3 ; i < 13 ; i++){
            mvprintw(i,120,"|");
            mvprintw(i,135,"|");
        }
        for(int i = 120 ; i < 135 ; i++){
            mvprintw(3,i,"--");
            mvprintw(12,i,"--");
        }
        for(int i = 4 ; i < 12 ; i++){
            for(int j = 121 ; j < 135 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(7,120,"+");
        mvprintw(12,127,"+");
        mvprintw(5,126,"o");
        mvprintw(6,131,"o");
        mvprintw(11,122,"o");
        //room4
        for(int i = 30 ; i < 42 ; i++){
            mvprintw(i,160,"|");
            mvprintw(i,175,"|");
        }
        for(int i = 160 ; i < 175 ; i++){
            mvprintw(30,i,"--");
            mvprintw(41,i,"--");
        }
        for(int i = 31 ; i < 41 ; i++){
            for(int j = 161 ; j < 175 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(30,167,"+");
        mvprintw(39,160,"+");
        mvprintw(35,169,"o");
        mvprintw(40,163,"o");
        //room5
        for(int i = 30 ; i < 36 ; i++){
            mvprintw(i,94,"|");
            mvprintw(i,110,"|");
        }
        for(int i = 94 ; i < 110 ; i++){
            mvprintw(30,i,"--");
            mvprintw(35,i,"--");
        }
        for(int i = 31 ; i < 35 ; i++){
            for(int j = 95 ; j < 110 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(32,94,"+");
        mvprintw(34,110,"+");
        mvprintw(31,99,"o");
        mvprintw(33,107,"o");
        //room6
        for(int i = 15 ; i < 23 ; i++){
            mvprintw(i,50,"|");
            mvprintw(i,60,"|");
        }
        for(int i = 50 ; i < 60 ; i++){
            mvprintw(15,i,"--");
            mvprintw(22,i,"--");
        }
        for(int i = 16 ; i < 22 ; i++){
            for(int j = 51 ; j < 60 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(17,60,"+");
        mvprintw(21,50,"+");
        mvprintw(20,53,"o");
        mvprintw(19,57,"o");
        //room7
        for(int i = 28 ; i < 33 ; i++){
            mvprintw(i,11,"|");
            mvprintw(i,26,"|");
        }
        for(int i = 11 ; i < 26 ; i++){
            mvprintw(28,i,"--");
            mvprintw(32,i,"--");
        }
        for(int i = 29 ; i < 32 ; i++){
            for(int j = 12 ; j < 26 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(28,20,"+");
        mvprintw(32,24,"+");
        mvprintw(30,12,"o");
        mvprintw(29,17,"o");
        //room8
        for(int i = 30 ; i < 42 ; i++){
            mvprintw(i,45,"|");
            mvprintw(i,60,"|");
        }
        for(int i = 45 ; i < 60 ; i++){
            mvprintw(30,i,"--");
            mvprintw(41,i,"--");
        }
        for(int i = 31 ; i < 41 ; i++){
            for(int j = 46 ; j < 60 ; j++){
                mvprintw(i,j,".");
            }
        }
        mvprintw(37,45,"+");
        mvprintw(33,55,"o");
        mvprintw(34,56,"o");
        draw_path(19, 7, 34, 12);
        draw_path(49, 9, 120, 7);
        draw_path(127, 13, 168, 29);
        draw_path(159, 39, 110, 34);
        draw_path(93, 32, 60, 17);
        draw_path(49, 21, 19, 27);
        draw_path(24, 33, 45, 37);
        
        if(getch()=='q'){
            return 0;
        }
}
int is_valid_move(int x, int y) {
    char ch = mvinch(y, x) & A_CHARTEXT;
    return ch == '.' || ch == '#' || ch == '+';
}
void draw_player(Player *player) {
    if(l_user.color==4){
        attron(COLOR_PAIR(4));
    }
    else if(l_user.color==2){
        attron(COLOR_PAIR(2));
    }
    else if(l_user.color==3){
        attron(COLOR_PAIR(3));
    }
    else if(l_user.color==5){
        attron(COLOR_PAIR(5));
    }
    else if(l_user.color==1){
        attron(COLOR_PAIR(1));
    }
    mvprintw(player->y, player->x, "@");
    if(l_user.color==4){
        attroff(COLOR_PAIR(4));
    }
    else if(l_user.color==2){
        attroff(COLOR_PAIR(2));
    }
    else if(l_user.color==3){
        attroff(COLOR_PAIR(3));
    }
    else if(l_user.color==5){
        attroff(COLOR_PAIR(5));
    }
    else if(l_user.color==1){
        attroff(COLOR_PAIR(1));
    }
}
void clear_player(Player *player) {
    if(fmsign==0){
       mvprintw(player->y, player->x, "%c", '.');
       fmsign++;
    }
    else{
        mvprintw(player->y, player->x, "%c", player->prev_char);
    }
}
void draw_path(int x1, int y1, int x2, int y2) {
    int cx = x1, cy = y1;

    while (cx != x2 || cy != y2) {
        mvprintw(cy, cx, "#");

        int dx = (cx < x2) - (cx > x2);
        int dy = (cy < y2) - (cy > y2);

        if (rand() % 2 == 0 && !is_wall(cx + dx, cy)) {
            cx += dx;
        } else if (!is_wall(cx, cy + dy)) {
            cy += dy;
        } else if (!is_wall(cx + dx, cy + dy)) {
            cx += dx;
            cy += dy;
        } else {
            break;
        }
    }
}
int handle_input(Player *player) {
    int ch = getch();
    int new_x = player->x, new_y = player->y;
    switch (ch) {
        case '1': new_x--; new_y++; break; 
        case '2': new_y++; break;         
        case '3': new_x++; new_y++; break; 
        case '4': new_x--; break;         
        case '6': new_x++; break;     
        case '7': new_x--; new_y--; break; 
        case '8': new_y--; break;       
        case '9': new_x++; new_y--; break;
    }

    if (is_valid_move(new_x, new_y)) {
        clear_player(player);

        player->prev_char = mvinch(new_y, new_x) & A_CHARTEXT;

        player->x = new_x;
        player->y = new_y;

        draw_player(player);
    }
}
int is_wall(int x, int y) {
    char ch = mvinch(y, x) & A_CHARTEXT;
    return ch == '|' || ch == '-' || ch == 'o';
}
// void save_information(User user){
//     FILE *reed=fopen("users.txt","r");
//     FILE *write=fopen("temp.txt", "w");
//     char buffer[1024];
//     int found = 0;

//     while (fgets(buffer, sizeof(buffer), reed) != NULL) {
//         char currentUsername[256], currentPassword[256], currentEmail[256],currentcolor[256],currentdifficulty[256];
//         int field1, field2, field3;

         
//         if (sscanf(buffer, "%s %s %s %d %d %d %s %s", currentUsername, currentPassword, currentEmail, &field1, &field2, &field3, currentcolor, currentdifficulty) == 8) {
//             if (strcmp(currentUsername, l_user.username) == 0) {
                 
//                 fprintf(write, "%s %s %s %d %d %d %s %s\n", l_user.username, l_user.password, l_user.email, l_user.score, l_user.gold, l_user.game,l_user.color, l_user.difficulty);
//                 found = 1;
//             } else {
                 
//                 fprintf(write, "%s", buffer);
//             }
//         } else {
             
//             fprintf(write, "%s", buffer);
//         }
//     }

//     fclose(reed);
//     fclose(write);
//     if (!found) {
//     } else {
//         if (remove("users.txt") != 0 || rename("temp.txt", "users.txt") != 0) {
//             exit(EXIT_FAILURE);
//         }
//     }
// }