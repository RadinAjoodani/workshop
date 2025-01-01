#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
// #include "function.h"
typedef struct {
    char username[50];
    char password[50];
    char email[100];
} User;

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

void main_menu();

int main() {
    initscr();        
    noecho();       
    cbreak();         
    keypad(stdscr, TRUE); 
    // curs_set(0);

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

    while (1) {
        clear();  
         
        for (int i = 0; i < n_items; i++) {
            if (i == choice)
                attron(A_REVERSE);  
            mvprintw(i + 1, 1, "%s", menu_items[i]);  
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
        while (fscanf(fptr, "%s %s %s", user.username, user.password, user.email) != EOF) {
            if (strcmp(user.username, username) == 0 && strcmp(user.password, password) == 0) {
                valid = 1;
                break;
            }
        }
        fclose(fptr);

        if (valid) {
            mvprintw(7, 1, "Login successful! Press any key to return to the main menu.");
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
    mvprintw(1, 1, "Play game menu");
    mvprintw(3, 1, "Press any key to return to the main menu...");
    getch();
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

    fprintf(file, "%s %s %s\n", user->username, user->password, user->email);
    fclose(file);
}