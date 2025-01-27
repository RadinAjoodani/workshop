#include<ncurses.h>
#include<string.h>
#include<stdlib.h>
#define MAP_HEIGHT 50
#define MAP_WIDTH 200

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
    int level_num;
}User;
typedef struct{
    int x,y;
    int color;
    int difficulty;
    char prev_char;
}Player;

char map1[MAP_HEIGHT][MAP_WIDTH];
char map2[MAP_HEIGHT][MAP_WIDTH];
char map3[MAP_HEIGHT][MAP_WIDTH];
char map4[MAP_HEIGHT][MAP_WIDTH];

int memory_map1[MAP_HEIGHT][MAP_WIDTH];
int memory_map2[MAP_HEIGHT][MAP_WIDTH];  
int memory_map3[MAP_HEIGHT][MAP_WIDTH];  
int memory_map4[MAP_HEIGHT][MAP_WIDTH];  

int fmsign=0;
Player l_player;
User l_user;
User s_user;
int is_logged_in=0;
int show_count = 0;
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
void start_level2();
void start_level3();
void start_level4();
void continue_last_game();
void show_profile_status(); 
void select_difficulty(int *current_difficulty);
void change_character_color(int *current_color);
int create_map1();
int create_map2();
int create_map3();
int create_map4();
void draw_path(int x1, int y1, int x2, int y2,char map[MAP_HEIGHT][MAP_WIDTH]);
void draw_player(Player *player);
int handle_input(Player *player);
void clear_player(Player *player);
int is_valid_move(int x, int y,char map[MAP_HEIGHT][MAP_WIDTH]);
void initialize_memory_map(int memory_map[MAP_HEIGHT][MAP_WIDTH]);
void draw_visible_map(int player_x, int player_y,int memory_map[MAP_HEIGHT][MAP_WIDTH],
                        char map[MAP_HEIGHT][MAP_WIDTH]);
void update_memory_map(int player_x, int player_y,int memory_map[MAP_HEIGHT][MAP_WIDTH]);
int is_in_room(int x, int y);
int get_room_id(int x, int y);
void refresh_map(Player *player,int memory_map[MAP_HEIGHT][MAP_WIDTH],char map[MAP_HEIGHT][MAP_WIDTH]);
void show_full_map_temporarily(Player *player);
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
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(7,COLOR_CYAN,COLOR_BLACK);
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
                    l_user.level_num=1;
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
    l_user.level_num=1;
    initialize_memory_map(memory_map1);  
    create_map1();  
    Player player = {5, 4, '.'};
    refresh_map(&player,memory_map1,map1);  

    while (1) {
        handle_input(&player);
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
    initialize_memory_map(memory_map1);  
    create_map1();  
    Player player = {5, 4, '.'};
    refresh_map(&player,memory_map1,map1);  

    while (1) {
        handle_input(&player);
    }
}
void start_level2(){
    clear();
    show_count = 0;
    l_user.level_num=2;
    initialize_memory_map(memory_map2);  
    create_map2();  
    Player player = {12,10, '.'};
    refresh_map(&player,memory_map2,map2);  

    while (1) {
        handle_input(&player);
    }
}
void start_level3(){
    clear();
    show_count = 0;
    l_user.level_num=3;
    initialize_memory_map(memory_map3);  
    create_map3();  
    Player player = {18,36, '.'};
    refresh_map(&player,memory_map3,map3);  

    while (1) {
        handle_input(&player);
    }
}
void start_level4(){
    clear();
    show_count = 0;
    l_user.level_num=4;
    initialize_memory_map(memory_map4);  
    create_map4();  
    Player player = {48,35, '.'};
    refresh_map(&player,memory_map4,map4);  

    while (1) {
        handle_input(&player);
    }
}
void continue_last_game(){

}
int is_valid_move(int x, int y,char map[MAP_HEIGHT][MAP_WIDTH]) {
    char ch = map[y][x];
    return ch == '.' || ch == '#' || ch == '+'||ch=='<';
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
    else if (l_user.level_num==1)
    {
        if(player->y==9 && player->x==100){
            attron(COLOR_PAIR(1));
            mvprintw(9,100, "%c", '|');
            attroff(COLOR_PAIR(1));
        } 
        else if(player->y==34 && player->x==110){
            attron(COLOR_PAIR(6));
            mvprintw(34, 110, "%c", '|');
            attroff(COLOR_PAIR(6));
        }
        else{
            mvprintw(player->y, player->x, "%c", player->prev_char);
        }
    }
    else if (l_user.level_num==2)
    {
        if(player->y==7 && player->x==120){
            attron(COLOR_PAIR(7));
            mvprintw(7,120, "%c", '|');
            attroff(COLOR_PAIR(7));
        } 
        else if(player->y==17 && player->x==60){
            attron(COLOR_PAIR(1));
            mvprintw(17, 60, "%c", '|');
            attroff(COLOR_PAIR(1));
        }
        else{
            mvprintw(player->y, player->x, "%c", player->prev_char);
        }
    }
    else if (l_user.level_num==3)
    {
        if(player->y==39 && player->x==160){
            attron(COLOR_PAIR(1));
            mvprintw(39,160, "%c", '|');
            attroff(COLOR_PAIR(1));
        } 
        else if(player->y==8 && player->x==158){
            attron(COLOR_PAIR(1));
            mvprintw(8, 158, "%c", '-');
            attroff(COLOR_PAIR(1));
        }
        else{
            mvprintw(player->y, player->x, "%c", player->prev_char);
        }
    }
    else if (l_user.level_num==4)
    {
        if(player->y==32 && player->x==94){
            attron(COLOR_PAIR(1));
            mvprintw(32,94, "%c", '|');
            attroff(COLOR_PAIR(1));
        } 
        else if(player->y==12 && player->x==127){
            attron(COLOR_PAIR(6));
            mvprintw(12, 127, "%c", '-');
            attroff(COLOR_PAIR(6));
        }
        else{
            mvprintw(player->y, player->x, "%c", player->prev_char);
        }
    }
}
void draw_path(int x1, int y1, int x2, int y2,char map[MAP_HEIGHT][MAP_WIDTH]) {
    int cx = x1, cy = y1;

    while (cx != x2 || cy != y2) {
        map[cy][cx] = '#';

        int dx = (cx < x2) - (cx > x2);
        int dy = (cy < y2) - (cy > y2);

        if (rand() % 2 == 0 && map[cy][cx + dx] != '|' && map[cy][cx + dx] != '-') {
            cx += dx;
        } else if (map[cy + dy][cx] != '|' && map[cy + dy][cx] != '-') {
            cy += dy;
        } else if (map[cy + dy][cx + dx] != '|' && map[cy + dy][cx + dx] != '-') {
            cx += dx;
            cy += dy;
        } else {
            break;
        }
    }
}
int create_map1() {
    memset(map1, ' ', sizeof(map1));

    for (int i = 4; i < 9; i++) {
        map1[i][3] = '|';
        map1[i][10] = '|';
    }
    for (int i = 3; i <= 10; i++) {
        map1[3][i] = '-';
        map1[9][i] = '-';
    }
    for (int i = 4; i < 9; i++) {
        for (int j = 4; j < 10; j++) {
            map1[i][j] = '.';
        }
    }
    map1[6][10] = '+';
    map1[7][5] = 'o';
    map1[4][8] = 'o';
    map1[8][9] = '<';

    //  room ۲
    for (int i = 4; i < 11; i++) {
        map1[i][40] = '|';
        map1[i][48] = '|';
    }
    for (int i = 40; i <= 48; i++) {
        map1[4][i] = '-';
        map1[10][i] = '-';
    }
    for (int i = 5; i < 10; i++) {
        for (int j = 41; j < 48; j++) {
            map1[i][j] = '.';
        }
    }
    map1[9][40] = '+';
    map1[7][48] = '+';
    map1[6][45] = 'o';
    map1[8][41] = 'o';

    //  room ۳
    for (int i = 6; i < 17; i++) {
        map1[i][100] = '|';
        map1[i][111] = '|';
    }
    for (int i = 100; i <= 111; i++) {
        map1[6][i] = '-';
        map1[16][i] = '-';
    }
    for (int i = 7; i < 16; i++) {
        for (int j = 101; j < 111; j++) {
            map1[i][j] = '.';
        }
    }
    map1[9][100] = '+';
    //map1[14][111] = '+';
    map1[10][105] = 'o';
    map1[14][102] = 'o';
    map1[15][109] = 'o';

    //  room ۴
    for (int i = 20; i < 30; i++) {
        map1[i][150] = '|';
        map1[i][159] = '|';
    }
    for (int i = 150; i <= 159; i++) {
        map1[20][i] = '-';
        map1[29][i] = '-';
    }
    for (int i = 21; i < 29; i++) {
        for (int j = 151; j < 159; j++) {
            map1[i][j] = '.';
        }
    }
    map1[20][152] = '+';
    map1[25][150] = '+';
    map1[22][152] = 'o';
    map1[28][156] = 'o';

    //  room ۵
    for (int i = 30; i < 36; i++) {
        map1[i][94] = '|';
        map1[i][110] = '|';
    }
    for (int i = 94; i <= 110; i++) {
        map1[30][i] = '-';
        map1[35][i] = '-';
    }
    for (int i = 31; i < 35; i++) {
        for (int j = 95; j < 110; j++) {
            map1[i][j] = '.';
        }
    }
    //map1[32][94] = '+';
    map1[34][110] = '+';
    map1[31][99] = 'o';
    map1[33][107] = 'o';

    //  room ۶
    for (int i = 28; i < 38; i++) {
        map1[i][10] = '|';
        map1[i][20] = '|';
    }
    for (int i = 10; i <= 20; i++) {
        map1[28][i] = '-';
        map1[37][i] = '-';
    }
    for (int i = 29; i < 37; i++) {
        for (int j = 11; j < 20; j++) {
            map1[i][j] = '.';
        }
    }
    map1[30][20] = '+';
    map1[30][12] = 'o';
    map1[36][17] = 'o';
    map1[36][13] = '<';

    // ایجاد راهروها
    draw_path(11, 6, 40, 9,map1);
    draw_path(49, 7, 100, 9,map1);
    draw_path(112, 14, 153, 19,map1);
    draw_path(149, 25, 110, 34,map1);
    draw_path(93, 32, 20, 30,map1);

    return 0;
}
int create_map2() {
    // پاک کردن آرایه نقشه
    memset(map2, ' ', sizeof(map2));

    //  room ۱
    for (int i = 6; i < 15; i++) {
        map2[i][10] = '|';
        map2[i][18] = '|';
    }
    for (int i = 10; i <= 18; i++) {
        map2[6][i] = '-';
        map2[14][i] = '-';
    }
    for (int i = 7; i < 14; i++) {
        for (int j = 11; j < 18; j++) {
            map2[i][j] = '.';
        }
    }
    //map2[7][18] = '+';
    map2[10][12] = 'o';
    map2[8][16] = 'o';
    map2[9][17] = '<';

    //  room ۲
    for (int i = 5; i < 14; i++) {
        map2[i][35] = '|';
        map2[i][48] = '|';
    }
    for (int i = 35; i <= 48; i++) {
        map2[5][i] = '-';
        map2[13][i] = '-';
    }
    for (int i = 6; i < 13; i++) {
        for (int j = 36; j < 48; j++) {
            map2[i][j] = '.';
        }
    }
    map2[12][35] = '+';
    map2[9][48] = '+';
    map2[11][45] = 'o';
    map2[9][37] = 'o';

    //  room ۳
    for (int i = 3; i < 13; i++) {
        map2[i][120] = '|';
        map2[i][135] = '|';
    }
    for (int i = 120; i <= 135; i++) {
        map2[3][i] = '-';
        map2[12][i] = '-';
    }
    for (int i = 4; i < 12; i++) {
        for (int j = 121; j < 135; j++) {
            map2[i][j] = '.';
        }
    }
    map2[7][120] = '+';
    map2[5][126] = 'o';
    map2[6][131] = 'o';
    map2[11][122] = 'o';

    //  room ۴
    for (int i = 30; i < 42; i++) {
        map2[i][160] = '|';
        map2[i][175] = '|';
    }
    for (int i = 160; i <= 175; i++) {
        map2[30][i] = '-';
        map2[41][i] = '-';
    }
    for (int i = 31; i < 41; i++) {
        for (int j = 161; j < 175; j++) {
            map2[i][j] = '.';
        }
    }
    map2[30][167] = '+';
    map2[39][160] = '+';
    map2[35][169] = 'o';
    map2[40][163] = 'o';

    //  room ۵
    for (int i = 30; i < 36; i++) {
        map2[i][94] = '|';
        map2[i][110] = '|';
    }
    for (int i = 94; i <= 110; i++) {
        map2[30][i] = '-';
        map2[35][i] = '-';
    }
    for (int i = 31; i < 35; i++) {
        for (int j = 95; j < 110; j++) {
            map2[i][j] = '.';
        }
    }
    map2[32][94] = '+';
    map2[34][110] = '+';
    map2[31][99] = 'o';
    map2[33][107] = 'o';

    //  room ۶
    for (int i = 15; i < 23; i++) {
        map2[i][50] = '|';
        map2[i][60] = '|';
    }
    for (int i = 50; i <= 60; i++) {
        map2[15][i] = '-';
        map2[22][i] = '-';
    }
    for (int i = 16; i < 22; i++) {
        for (int j = 51; j < 60; j++) {
            map2[i][j] = '.';
        }
    }
    map2[17][60] = '+';
    map2[20][53] = 'o';
    map2[19][57] = 'o';

    //  room ۷
    for (int i = 28; i < 33; i++) {
        map2[i][11] = '|';
        map2[i][26] = '|';
    }
    for (int i = 11; i <= 26; i++) {
        map2[28][i] = '-';
        map2[32][i] = '-';
    }
    for (int i = 29; i < 32; i++) {
        for (int j = 12; j < 26; j++) {
            map2[i][j] = '.';
        }
    }
    map2[28][20] = '+';
    map2[30][12] = 'o';
    map2[29][17] = 'o';

    // ایجاد راهروها
    draw_path(19, 7, 35, 12,map2);
    draw_path(49, 9, 120, 7,map2);
    draw_path(127, 13, 168, 29,map2);
    draw_path(159, 39, 110, 34,map2);
    draw_path(93, 32, 60, 17,map2);
    draw_path(49, 21, 19, 27,map2);
}
int create_map3() {
    // پاک کردن آرایه نقشه
    memset(map3, ' ', sizeof(map3));

    //  room ۱
    for (int i = 6; i < 16; i++) {
        map3[i][35] = '|';
        map3[i][41] = '|';
    }
    for (int i = 35; i <= 41; i++) {
        map3[6][i] = '-';
        map3[15][i] = '-';
    }
    for (int i = 7; i < 15; i++) {
        for (int j = 36; j < 41; j++) {
            map3[i][j] = '.';
        }
    }
    map3[12][41] = '+';
    map3[11][40] = 'o';
    map3[9][37] = 'o';

    //  room ۲
    for (int i = 12; i < 17; i++) {
        map3[i][70] = '|';
        map3[i][84] = '|';
    }
    for (int i = 70; i <= 84; i++) {
        map3[12][i] = '-';
        map3[16][i] = '-';
    }
    for (int i = 13; i < 16; i++) {
        for (int j = 71; j < 84; j++) {
            map3[i][j] = '.';
        }
    }
    map3[13][70] = '+';
    map3[14][84] = '+';
    map3[13][75] = 'o';
    map3[15][80] = 'o';

    //  room ۳
    for (int i = 2; i < 9; i++) {
        map3[i][150] = '|';
        map3[i][160] = '|';
    }
    for (int i = 150; i <= 160; i++) {
        map3[2][i] = '-';
        map3[8][i] = '-';
    }
    for (int i = 3; i < 8; i++) {
        for (int j = 151; j < 160; j++) {
            map3[i][j] = '.';
        }
    }
    map3[8][158] = '+';
    map3[4][155] = 'o';
    map3[7][159] = 'o';
    map3[4][151] = 'o';

    //  room ۴
    for (int i = 30; i < 42; i++) {
        map3[i][160] = '|';
        map3[i][175] = '|';
    }
    for (int i = 160; i <= 175; i++) {
        map3[30][i] = '-';
        map3[41][i] = '-';
    }
    for (int i = 31; i < 41; i++) {
        for (int j = 161; j < 175; j++) {
            map3[i][j] = '.';
        }
    }
    map3[39][160] = '+';
    map3[35][169] = 'o';
    map3[40][163] = 'o';

    //  room ۵
    for (int i = 30; i < 36; i++) {
        map3[i][84] = '|';
        map3[i][100] = '|';
    }
    for (int i = 84; i <= 100; i++) {
        map3[30][i] = '-';
        map3[35][i] = '-';
    }
    for (int i = 31; i < 35; i++) {
        for (int j = 85; j < 100; j++) {
            map3[i][j] = '.';
        }
    }
    map3[32][84] = '+';
    map3[34][100] = '+';
    map3[31][99] = 'o';
    map3[33][86] = 'o';

    //  room ۶
    for (int i = 35; i < 40; i++) {
        map3[i][17] = '|';
        map3[i][26] = '|';
    }
    for (int i = 17; i <= 26; i++) {
        map3[35][i] = '-';
        map3[39][i] = '-';
    }
    for (int i = 36; i < 39; i++) {
        for (int j = 18; j < 26; j++) {
            map3[i][j] = '.';
        }
    }
    map3[37][26] = '+';
    map3[37][22] = 'o';
    map3[36][23] = '<';

    // ایجاد راهروها
    draw_path(27, 37, 84, 32,map3);
    draw_path(101, 34, 160, 39,map3);
    draw_path(167, 29, 158, 8,map3);
    draw_path(149, 6, 84, 14,map3);
    draw_path(69, 13, 41, 12,map3);
}
int create_map4() {
    // پاک کردن آرایه نقشه
    memset(map4, ' ', sizeof(map4));

    //  room ۱
    for (int i = 6; i < 15; i++) {
        map4[i][10] = '|';
        map4[i][18] = '|';
    }
    for (int i = 10; i <= 18; i++) {
        map4[6][i] = '-';
        map4[14][i] = '-';
    }
    for (int i = 7; i < 14; i++) {
        for (int j = 11; j < 18; j++) {
            map4[i][j] = '.';
        }
    }
    map4[7][18] = '+';
    map4[10][12] = 'o';
    map4[8][16] = 'o';

    //  room ۲
    for (int i = 5; i < 14; i++) {
        map4[i][35] = '|';
        map4[i][48] = '|';
    }
    for (int i = 35; i <= 48; i++) {
        map4[5][i] = '-';
        map4[13][i] = '-';
    }
    for (int i = 6; i < 13; i++) {
        for (int j = 36; j < 48; j++) {
            map4[i][j] = '.';
        }
    }
    map4[12][35] = '+';
    map4[9][48] = '+';
    map4[11][45] = 'o';
    map4[9][37] = 'o';

    //  room ۳
    for (int i = 3; i < 13; i++) {
        map4[i][120] = '|';
        map4[i][135] = '|';
    }
    for (int i = 120; i <= 135; i++) {
        map4[3][i] = '-';
        map4[12][i] = '-';
    }
    for (int i = 4; i < 12; i++) {
        for (int j = 121; j < 135; j++) {
            map4[i][j] = '.';
        }
    }
    map4[12][127] = '+';
    map4[5][126] = 'o';
    map4[6][131] = 'o';
    map4[11][122] = 'o';

    //  room ۴
    for (int i = 30; i < 42; i++) {
        map4[i][160] = '|';
        map4[i][175] = '|';
    }
    for (int i = 160; i <= 175; i++) {
        map4[30][i] = '-';
        map4[41][i] = '-';
    }
    for (int i = 31; i < 41; i++) {
        for (int j = 161; j < 175; j++) {
            map4[i][j] = '.';
        }
    }
    map4[30][167] = '+';
    map4[39][160] = '+';
    map4[35][169] = 'o';
    map4[40][163] = 'o';

    //  room ۵
    for (int i = 30; i < 36; i++) {
        map4[i][94] = '|';
        map4[i][110] = '|';
    }
    for (int i = 94; i <= 110; i++) {
        map4[30][i] = '-';
        map4[35][i] = '-';
    }
    for (int i = 31; i < 35; i++) {
        for (int j = 95; j < 110; j++) {
            map4[i][j] = '.';
        }
    }
    map4[32][94] = '+';
    map4[31][99] = 'o';
    map4[33][107] = 'o';

    //  room ۶
    for (int i = 15; i < 23; i++) {
        map4[i][50] = '|';
        map4[i][60] = '|';
    }
    for (int i = 50; i <= 60; i++) {
        map4[15][i] = '-';
        map4[22][i] = '-';
    }
    for (int i = 16; i < 22; i++) {
        for (int j = 51; j < 60; j++) {
            map4[i][j] = '.';
        }
    }
    map4[17][60] = '+';
    map4[21][50] = '+';
    map4[20][53] = 'o';
    map4[19][57] = 'o';

    // اتاق ۷
    for (int i = 28; i < 33; i++) {
        map4[i][11] = '|';
        map4[i][26] = '|';
    }
    for (int i = 11; i <= 26; i++) {
        map4[28][i] = '-';
        map4[32][i] = '-';
    }
    for (int i = 29; i < 32; i++) {
        for (int j = 12; j < 26; j++) {
            map4[i][j] = '.';
        }
    }
    map4[28][20] = '+';
    map4[32][24] = '+';
    map4[30][12] = 'o';
    map4[29][17] = 'o';

    // اتاق ۸
    for (int i = 30; i < 42; i++) {
        map4[i][45] = '|';
        map4[i][60] = '|';
    }
    for (int i = 45; i <= 60; i++) {
        map4[30][i] = '-';
        map4[41][i] = '-';
    }
    for (int i = 31; i < 41; i++) {
        for (int j = 46; j < 60; j++) {
            map4[i][j] = '.';
        }
    }
    map4[33][55] = 'o';
    map4[34][56] = 'o';

    // ایجاد راهروها
    draw_path(34, 12, 18, 7,map4);
    draw_path(119, 7, 48, 9,map4);
    draw_path(167, 29, 126, 13,map4);
    draw_path(111, 34, 160, 39,map4);
    draw_path(61, 17, 94, 32,map4);
    draw_path(20, 27, 50, 21,map4);
    draw_path(44, 37, 23, 33,map4);
}
int handle_input(Player *player) {
    if(l_user.level_num==1){
        int ch = getch();
        int new_x = player->x, new_y = player->y;
        if (ch == 'm') {
            show_count++;
            if(show_count>=4){
                clear();
                attron(COLOR_PAIR(2));
                mvprintw(22,82,"YOU LOST");
                getch();
                endwin();
                exit(0);
            }
            show_full_map_temporarily(player);
        }
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

        if (is_valid_move(new_x, new_y,map1) && map1[new_y][new_x]!='<') {
            clear_player(player);

            player->prev_char = map1[new_y][new_x];  
            player->x = new_x;
            player->y = new_y;
            if (new_x == 110 && new_y == 14) {
                map1[14][111]='+';
            }
            else if (new_x == 95 && new_y == 32) {
                map1[32][94]='+'; 
            }
            refresh_map(player,memory_map1,map1);  
        }
        else if(map1[new_y][new_x]=='<'){
            start_level2();
        }
        
    }
    else if(l_user.level_num==2){
        int ch = getch();
        int new_x = player->x, new_y = player->y;
        if (ch == 'm') {
            show_count++;
            if(show_count>=4){
                clear();
                attron(COLOR_PAIR(2));
                mvprintw(22,82,"YOU LOST");
                getch();
                endwin();
                exit(0);
            }
            show_full_map_temporarily(player);
        }
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

        if (is_valid_move(new_x, new_y,map2) && map2[new_y][new_x]!='<') {
            clear_player(player);

            player->prev_char = map2[new_y][new_x];  
            player->x = new_x;
            player->y = new_y;
            if (new_x == 17 && new_y == 7) {
                map2[7][18]='+';
            }
            else if (new_x == 128 && new_y == 11) {
                map2[12][128]='+';
            }
            else if (new_x == 51 && new_y == 21) {
                map2[21][50]='+';
            }
            refresh_map(player,memory_map2,map2);
        }
        else if(map2[new_y][new_x]=='<'){
            start_level3();
        }
    }
    else if(l_user.level_num==3){
        int ch = getch();
        int new_x = player->x, new_y = player->y;
        if (ch == 'm') {
            show_count++;
            if(show_count>=4){
                clear();
                attron(COLOR_PAIR(2));
                mvprintw(22,82,"YOU LOST");
                getch();
                endwin();
                exit(0);
            }
            show_full_map_temporarily(player);
        }
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

        if (is_valid_move(new_x, new_y,map3)&& map3[new_y][new_x]!='<') {
            clear_player(player);

            player->prev_char = map3[new_y][new_x];  
            player->x = new_x;
            player->y = new_y;
            if (new_x == 167 && new_y == 31) {
                map3[30][167]='+';
            }
            else if (new_x == 151 && new_y == 6) {
                map3[6][150]='+';
            }
            refresh_map(player,memory_map3,map3);  
        }
        else if(map3[new_y][new_x]=='<'){
            start_level4();
        }
    }
    else if(l_user.level_num==4){
        int ch = getch();
        int new_x = player->x, new_y = player->y;
        if (ch == 'm') {
            show_count++;
            if(show_count>=4){
                clear();
                attron(COLOR_PAIR(2));
                mvprintw(22,82,"YOU LOST");
                getch();
                endwin();
                exit(0);
            }
            show_full_map_temporarily(player);
        }
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

        if (is_valid_move(new_x, new_y,map4)) {
            clear_player(player);

            player->prev_char = map4[new_y][new_x];  
            player->x = new_x;
            player->y = new_y;
            if (new_x == 109 && new_y == 34) {
                map4[34][110]='+';
            }
            else if (new_x == 121 && new_y == 7) {
                map4[7][120]='+';
            }
            else if (new_x == 46 && new_y == 37) {
                map4[37][45]='+';
            }
            refresh_map(player,memory_map4,map4);  
        }
    }
}
void initialize_memory_map(int memory_map[MAP_HEIGHT][MAP_WIDTH]) {
    memset(memory_map,0,1000);
}
void draw_visible_map(int player_x, int player_y,int memory_map[MAP_HEIGHT][MAP_WIDTH],char map[MAP_HEIGHT][MAP_WIDTH]) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (memory_map[i][j]) {
            if (is_in_room(player_x, player_y) && map[i][j] == '#') {
                    continue;
                }

                attron(COLOR_PAIR(4));
                mvaddch(i, j, map[i][j]);
                attroff(COLOR_PAIR(4));
            }
        }
    }
}
void refresh_map(Player *player,int memory_map[MAP_HEIGHT][MAP_WIDTH],char map[MAP_HEIGHT][MAP_WIDTH]) {
    update_memory_map(player->x, player->y,memory_map);
    draw_visible_map(player->x, player->y,memory_map,map);
    draw_player(player);
    refresh();  
}
void update_memory_map(int player_x, int player_y,int memory_map[MAP_HEIGHT][MAP_WIDTH]) {
    if(l_user.level_num==1){
        if(is_in_room(player_x,player_y)){
            if (get_room_id(player_x,player_y)==1) {
                for (int i = 3; i <= 9; i++) {
                    for (int j = 3; j <= 10; j++) {
                        memory_map[i][j] = 1;  
                    }
                }
            }
            
            else if (get_room_id(player_x,player_y)==2) {
                for (int i = 4; i <= 10; i++) {
                    for (int j = 40; j <= 48; j++) {
                        memory_map[i][j] = 1;
                    }
                }
            }
            
            else if (get_room_id(player_x,player_y)==3) {
                for (int i = 6; i <= 16; i++) {
                    for (int j = 100; j <= 111; j++) {
                        memory_map[i][j] = 1;
                    }
                }
            }
            
            else if (get_room_id(player_x,player_y)==4) {
                for (int i = 20; i <= 29; i++) {
                    for (int j = 150; j <= 159; j++) {
                        memory_map[i][j] = 1;
                    }
                }
            }
            
            else if (get_room_id(player_x,player_y)==5) {
                for (int i = 30; i <= 35; i++) {
                    for (int j = 94; j <= 110; j++) {
                        memory_map[i][j] = 1;
                    }
                }
            }
            
            else if (get_room_id(player_x,player_y)==6) {
                for (int i = 28; i <= 37; i++) {
                    for (int j = 10; j <= 20; j++) {
                        memory_map[i][j] = 1;
                    }
                }
            }
        }
        else{
            int view_distance = 3;  

            for (int y = player_y - view_distance; y <= player_y + view_distance; y++) {
                for (int x = player_x - view_distance; x <= player_x + view_distance; x++) {
                    if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
                        memory_map[y][x] = 1;  
                    }
                }
            }
        }
    }
    else if (l_user.level_num == 2) {
        if (is_in_room(player_x, player_y)) {
            if (get_room_id(player_x, player_y) == 1) {
                for (int i = 6; i <= 14; i++) {
                    for (int j = 10; j <= 18; j++) {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 2) {
                for (int i = 5; i <= 13; i++) {
                    for (int j = 35; j <= 48; j++) {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 3) {
                for (int i = 3; i <= 12; i++) {
                    for (int j = 120; j <= 135; j++) {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 4) {
                for (int i = 30; i <= 41; i++) {
                    for (int j = 160; j <= 175; j++) {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 5) {
                for (int i = 30; i <= 35; i++) {
                    for (int j = 94; j <= 110; j++) {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 6) {
                for (int i = 15; i <= 22; i++) {
                    for (int j = 50; j <= 60; j++) {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 7) {
                for (int i = 28; i <= 32; i++) {
                    for (int j = 11; j <= 26; j++) {
                        memory_map[i][j] = 1;
                    }
                }
            }
        }
        else{
            int view_distance = 3;  

            for (int y = player_y - view_distance; y <= player_y + view_distance; y++) {
                for (int x = player_x - view_distance; x <= player_x + view_distance; x++) {
                    if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
                        memory_map[y][x] = 1;  
                    }
                }
            }
        }
    }
    else if (l_user.level_num == 3) {
        if (is_in_room(player_x, player_y)) {
            if (get_room_id(player_x, player_y) == 1) {
                for (int i = 6; i <= 15; i++) {  
                    for (int j = 35; j <= 41; j++) {  
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 2) {
                for (int i = 12; i <= 16; i++) {  
                    for (int j = 70; j <= 84; j++) {  
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 3) {
                for (int i = 2; i <= 8; i++) {  
                    for (int j = 150; j <= 160; j++) {  
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 4) {
                for (int i = 30; i <= 41; i++) {  
                    for (int j = 160; j <= 175; j++) {  
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 5) {
                for (int i = 30; i <= 35; i++) {  
                    for (int j = 84; j <= 100; j++) {  
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 6) {
                for (int i = 35; i <= 39; i++) {  
                    for (int j = 17; j <= 26; j++) {  
                        memory_map[i][j] = 1;
                    }
                }
            }
        }
        else{
            int view_distance = 3;  

            for (int y = player_y - view_distance; y <= player_y + view_distance; y++) {
                for (int x = player_x - view_distance; x <= player_x + view_distance; x++) {
                    if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
                        memory_map[y][x] = 1;  
                    }
                }
            }
        }
    }
    else if (l_user.level_num == 4) {
        if (is_in_room(player_x, player_y)) {
            if (get_room_id(player_x, player_y) == 1) {
                for (int i = 6; i <= 14; i++) {  
                    for (int j = 10; j <= 18; j++) {  
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 2) {
                for (int i = 5; i <= 13; i++) {  
                    for (int j = 35; j <= 48; j++) {  
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 3) {
                for (int i = 3; i <= 12; i++) {  
                    for (int j = 120; j <= 135; j++) {  
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 4) {
                for (int i = 30; i <= 41; i++) {  
                    for (int j = 160; j <= 175; j++) {  
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 5) {
                for (int i = 30; i <= 35; i++) {  
                    for (int j = 94; j <= 110; j++) {  
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 6) {
                for (int i = 15; i <= 22; i++) {  
                    for (int j = 50; j <= 60; j++) {  
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 7) {
                for (int i = 28; i <= 32; i++) {  
                    for (int j = 11; j <= 26; j++) {  
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 8) {
                for (int i = 30; i <= 41; i++) {  
                    for (int j = 45; j <= 60; j++) {  
                        memory_map[i][j] = 1;
                    }
                }
            }
        }
        else{
            int view_distance = 3;  

            for (int y = player_y - view_distance; y <= player_y + view_distance; y++) {
                for (int x = player_x - view_distance; x <= player_x + view_distance; x++) {
                    if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
                        memory_map[y][x] = 1;  
                    }
                }
            }
        }
    }
    
}
int is_in_room(int x, int y) {
    if(l_user.level_num==1){
        if ((x >= 3 && x <= 10 && y >= 3 && y <= 9) ||  
        (x >= 40 && x <= 48 && y >= 4 && y <= 10) ||  
        (x >= 100 && x <= 111 && y >= 6 && y <= 16) ||  
        (x >= 150 && x <= 159 && y >= 20 && y <= 29) ||  
        (x >= 94 && x <= 110 && y >= 30 && y <= 35) ||  
        (x >= 10 && x <= 20 && y >= 28 && y <= 37)) {
        return 1;
        }
    return 0;
    }
    else if(l_user.level_num==2){
        if ((y >= 6 && y <= 14 && x >= 10 && x <= 18) ||   
        (y >= 5 && y <= 13 && x >= 35 && x <= 48) ||   
        (y >= 3 && y <= 12 && x >= 120 && x <= 135) ||   
        (y >= 30 && y <= 41 && x >= 160 && x <= 175) ||   
        (y >= 30 && y <= 35 && x >= 94 && x <= 110) ||   
        (y >= 15 && y <= 22 && x >= 50 && x <= 60) ||   
        (y >= 28 && y <= 32 && x >= 11 && x <= 26)) {   
        return 1;
    }
    return 0;
    }
    else if(l_user.level_num==3){
        if ((x >= 35 && x <= 41 && y >= 6 && y <= 15) ||   
        (x >= 70 && x <= 84 && y >= 12 && y <= 16) ||   
        (x >= 150 && x <= 160 && y >= 2 && y <= 8) ||   
        (x >= 160 && x <= 175 && y >= 30 && y <= 41) ||   
        (x >= 84 && x <= 100 && y >= 30 && y <= 35) ||   
        (x >= 17 && x <= 26 && y >= 35 && y <= 39)) {   
        return 1;
        }
    return 0;
    }
    else if(l_user.level_num==4){
        if ((x >= 10 && x <= 18 && y >= 6 && y <= 14) ||   
        (x >= 35 && x <= 48 && y >= 5 && y <= 13) ||   
        (x >= 120 && x <= 135 && y >= 3 && y <= 12) ||   
        (x >= 160 && x <= 175 && y >= 30 && y <= 41) ||   
        (x >= 94 && x <= 110 && y >= 30 && y <= 35) ||   
        (x >= 50 && x <= 60 && y >= 15 && y <= 22) ||   
        (x >= 11 && x <= 26 && y >= 28 && y <= 32) ||   
        (x >= 45 && x <= 60 && y >= 30 && y <= 41)) {   
        return 1;
        }
    return 0;
    }
    
}
int get_room_id(int x, int y) {
    if(l_user.level_num==1){
        if (x >= 4 && x <= 10 && y >= 4 && y <= 10) return 1;
        if (x >= 41 && x <= 48 && y >= 5 && y <= 10) return 2;
        if (x >= 101 && x <= 111 && y >= 7 && y <= 16) return 3;
        if (x >= 151 && x <= 159 && y >= 21 && y <= 29) return 4;
        if (x >= 95 && x <= 110 && y >= 31 && y <= 35) return 5;
        if (x >= 11 && x <= 20 && y >= 29 && y <= 37) return 6;
        return 0;
    }
    else if(l_user.level_num==2){
        if (y >= 6 && y <= 14 && x >= 10 && x <= 18) return 1;   
        if (y >= 5 && y <= 13 && x >= 35 && x <= 48) return 2;   
        if (y >= 3 && y <= 12 && x >= 120 && x <= 135) return 3;   
        if (y >= 30 && y <= 41 && x >= 160 && x <= 175) return 4;   
        if (y >= 30 && y <= 35 && x >= 94 && x <= 110) return 5;   
        if (y >= 15 && y <= 22 && x >= 50 && x <= 60) return 6;   
        if (y >= 28 && y <= 32 && x >= 11 && x <= 26) return 7;    
        return 0;
    }
    else if(l_user.level_num==3){
        if (x >= 35 && x <= 41 && y >= 6 && y <= 15) return 1;  
        if (x >= 70 && x <= 84 && y >= 12 && y <= 16) return 2;  
        if (x >= 150 && x <= 160 && y >= 2 && y <= 8) return 3;  
        if (x >= 160 && x <= 175 && y >= 30 && y <= 41) return 4;  
        if (x >= 84 && x <= 100 && y >= 30 && y <= 35) return 5;  
        if (x >= 17 && x <= 26 && y >= 35 && y <= 39) return 6;  
        return 0;
    }
    else if(l_user.level_num==4){
        if (x >= 10 && x <= 18 && y >= 6 && y <= 14) return 1;  
        if (x >= 35 && x <= 48 && y >= 5 && y <= 13) return 2;  
        if (x >= 120 && x <= 135 && y >= 3 && y <= 12) return 3;  
        if (x >= 160 && x <= 175 && y >= 30 && y <= 41) return 4;  
        if (x >= 94 && x <= 110 && y >= 30 && y <= 35) return 5;  
        if (x >= 50 && x <= 60 && y >= 15 && y <= 22) return 6;  
        if (x >= 11 && x <= 26 && y >= 28 && y <= 32) return 7;  
        if (x >= 45 && x <= 60 && y >= 30 && y <= 41)return 8; 
        return 0;
    }
}
void show_full_map_temporarily(Player *player) {
    if(l_user.level_num==1){
        for (int i = 0; i < MAP_HEIGHT; i++) {
            for (int j = 0; j < MAP_WIDTH; j++) {
                mvaddch(i, j, map1[i][j]);
            }
        }
        refresh();
        timeout(4000);
        int ch = getch();
        if(ch=='m'){
            clear();
            refresh_map(player,memory_map1,map1);
        }
        else {
            clear();
            refresh_map(player,memory_map1,map1);
        }
        timeout(-1);
    }
    else if(l_user.level_num==2){
        for (int i = 0; i < MAP_HEIGHT; i++) {
            for (int j = 0; j < MAP_WIDTH; j++) {
                mvaddch(i, j, map2[i][j]);
            }
        }
        refresh();
        timeout(4000);
        int ch = getch();
        if(ch=='m'){
            clear();
            refresh_map(player,memory_map2,map2);
        }
        else {
            clear();
            refresh_map(player,memory_map2,map2);
        }
        timeout(-1);
    }
    else if(l_user.level_num==3){
        for (int i = 0; i < MAP_HEIGHT; i++) {
            for (int j = 0; j < MAP_WIDTH; j++) {
                mvaddch(i, j, map3[i][j]);
            }
        }
        refresh();
        timeout(4000);
        int ch = getch();
        if(ch=='m'){
            clear();
            refresh_map(player,memory_map3,map3);
        }
        else {
            clear();
            refresh_map(player,memory_map3,map3); 
        }
        timeout(-1);
    }
    else if(l_user.level_num==4){
        for (int i = 0; i < MAP_HEIGHT; i++) {
            for (int j = 0; j < MAP_WIDTH; j++) {
                mvaddch(i, j, map4[i][j]);
            }
        }
        refresh();
        timeout(4000);
        int ch = getch();
        if(ch=='m'){
            clear();
            refresh_map(player,memory_map4,map4);
        }
        else {
            clear();
            refresh_map(player,memory_map4,map4);
        }
        timeout(-1);
    }
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