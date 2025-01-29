#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAP_HEIGHT 50
#define MAP_WIDTH 200
typedef struct
{
    int normal;
    int special;
} Food;
typedef struct
{
    int H;
    int S;
    int G;
} Spell;
typedef struct
{
    char password[50];
    char email[50];
    char username[50];
    int score;
    int game;
    int gold;
    int health;
    int power;
    char weapon;
    char spell;
    int difficulty;
    int color;
    int level_num;
    Food food_bar;
    Spell spell_bar;
} User;
typedef struct
{
    int x, y;
    int color;
    int difficulty;
    char prev_char;
} Player;

char map1[MAP_HEIGHT][MAP_WIDTH];
char map2[MAP_HEIGHT][MAP_WIDTH];
char map3[MAP_HEIGHT][MAP_WIDTH];
char map4[MAP_HEIGHT][MAP_WIDTH];

int memory_map1[MAP_HEIGHT][MAP_WIDTH];
int memory_map2[MAP_HEIGHT][MAP_WIDTH];
int memory_map3[MAP_HEIGHT][MAP_WIDTH];
int memory_map4[MAP_HEIGHT][MAP_WIDTH];

int fmsign = 0;
Player l_player;
User l_user;
User s_user;
int is_logged_in = 0;
int show_count = 0;
int full_food = 0;
int full_spell = 0;
int pace = 1;
int pace_counter1 = 0;
int pace_counter2 = 0;
int Hspell;
int Sspell;
int Gspell;
int Hspellc = 0;
int Sspellc = 0;
int Gspellc = 0;
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
void draw_path(int x1, int y1, int x2, int y2, char map[MAP_HEIGHT][MAP_WIDTH]);
void draw_player(Player *player);
int handle_input(Player *player);
void clear_player(Player *player);
int is_valid_move(int x, int y, char map[MAP_HEIGHT][MAP_WIDTH]);
void initialize_memory_map(int memory_map[MAP_HEIGHT][MAP_WIDTH]);
void draw_visible_map(int player_x, int player_y, int memory_map[MAP_HEIGHT][MAP_WIDTH], char map[MAP_HEIGHT][MAP_WIDTH]);
void update_memory_map(int player_x, int player_y, int memory_map[MAP_HEIGHT][MAP_WIDTH]);
int is_in_room(int x, int y);
int get_room_id(int x, int y);
void refresh_map(Player *player, int memory_map[MAP_HEIGHT][MAP_WIDTH], char map[MAP_HEIGHT][MAP_WIDTH]);
void show_full_map_temporarily(Player *player);
char *generate_code();
void show_code(const char *code);
char *enter_code();
int check_code(const char *entered_code, const char *correct_code);
void draw_bar(int y, int x, int width, int value, int max_value, const char *label);
int food_manager(char food);
void food_table();
int gold_manager(char gold);
int spell_manager(char spell);
int spell_table();
char message(int height, int width);
// void save_information(User user);

int main()
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    start_color();
    srand(time(NULL));
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_BLACK);
    init_pair(5, COLOR_BLUE, COLOR_BLACK);
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(7, COLOR_CYAN, COLOR_BLACK);
    main_menu();
    getch();
    endwin();
}

void main_menu()
{
    curs_set(0);
    char *menu_items[] = {
        "Sign up",
        "Log in",
        "Play game",
        "Play as a guest",
        "Profile",
        "Scores",
        "Exit"};
    int n_items = sizeof(menu_items) / sizeof(menu_items[0]);
    int choice = 0;
    int key;
    int rows, cols;

    while (1)
    {
        clear();
        getmaxyx(stdscr, rows, cols);
        attron(COLOR_PAIR(3));
        for (int i = LINES / 2 - 4; i < LINES / 2 + 5; i++)
        {
            mvprintw(i, COLS / 2 - 17, "||");
            mvprintw(i, COLS / 2 + 12, "||");
        }
        mvprintw(LINES / 2 + 5, COLS / 2 - 14, "=========================");
        mvprintw(LINES / 2 - 5, COLS / 2 - 14, "=========================");
        attroff(COLOR_PAIR(3));
        attron(COLOR_PAIR(2));
        mvprintw(LINES / 2, COLS / 2 - 10, "ENJOY THE GAME :)");
        attroff(COLOR_PAIR(2));

        // mvprintw(LINES/2,1,"%d %d",LINES,COLS);

        for (int i = 0; i < n_items; i++)
        {
            if (i == choice)
                attron(COLOR_PAIR(2));
            mvprintw(i + 1, 1, "%s", menu_items[i]);
            if (i == choice)
                attroff(COLOR_PAIR(2));
        }

        if (is_logged_in)
        {
            mvprintw(rows - 1, 1, "Logged in as: %s | Score: %d", l_user.username, l_user.score);
        }
        else
        {
            mvprintw(rows - 1, 1, "Not logged in. Please log in or sign up.");
        }

        key = getch();

        switch (key)
        {
        case KEY_UP:
            choice = (choice - 1 + n_items) % n_items;
            break;
        case KEY_DOWN:
            choice = (choice + 1) % n_items;
            break;
        case '\n':
            clear();
            switch (choice)
            {
            case 0:
                sign_up();
                break;
            case 1:
                log_in();
                break;
            case 2:
                play_game();
                break;
            case 3:
                play_as_guest();
                break;
            case 4:
                profile();
                break;
            case 5:
                show_table();
                break;
            case 6:
                eexit();
                return;
            }
            break;
        default:
            break;
        }
    }
}
void sign_up()
{
    int sign = 0;
    attron(COLOR_PAIR(3));
    mvprintw(1, 1, "Sign up menu");
    attroff(COLOR_PAIR(3));
    attron(COLOR_PAIR(1));
    while (1)
    {
        mvprintw(2, 1, "Enter your username: ");
        clrtoeol();
        echo();
        getstr(s_user.username);
        noecho();
        if (username_check(s_user.username))
        {
            mvprintw(3, 1, "Username already exists. Try again.");
            clrtoeol();
            mvprintw(4, 1, "press q to exit");
            if (getch() == 'q')
            {
                return;
            }
            else
            {
                move(3, 1);
                clrtoeol();
                move(4, 1);
                clrtoeol();
            }
        }
        else
        {
            mvprintw(3, 1, "Username is ok");
            clrtoeol();
            sign++;
            break;
        }
    }
    while (1)
    {
        mvprintw(5, 1, "Enter your password: ");
        clrtoeol();
        echo();
        getstr(s_user.password);
        noecho();

        if (!pass_check(s_user.password))
        {
            mvprintw(6, 1, "Password does not meet requirements. Try again.");
            clrtoeol();
            mvprintw(7, 1, "press q to exit");
            if (getch() == 'q')
            {
                return;
            }
            else
            {
                move(6, 1);
                clrtoeol();
                move(7, 1);
                clrtoeol();
            }
        }
        else
        {
            mvprintw(6, 1, "Password is ok");
            clrtoeol();
            sign++;
            break;
        }
    }
    while (1)
    {
        mvprintw(8, 1, "Enter your email: ");
        clrtoeol();
        echo();
        getstr(s_user.email);
        noecho();

        if (!email_check(s_user.email))
        {
            mvprintw(9, 1, "Email format is incorrect. Try again.");
            clrtoeol();
            mvprintw(10, 1, "press q to exit");
            if (getch() == 'q')
            {
                return;
            }
            else
            {
                move(9, 1);
                clrtoeol();
                move(10, 1);
                clrtoeol();
            }
        }
        else
        {
            mvprintw(9, 1, "Email is ok");
            clrtoeol();
            sign++;
            break;
        }
        s_user.game = 0;
        s_user.gold = 0;
        s_user.score = 0;
        s_user.difficulty = 0;
        s_user.color = 0;
    }
    if (sign == 3)
    {
        save_user();
        mvprintw(11, 1, "ACCOUNT CREATED SUCCESSFULLY");
        mvprintw(12, 1, "press Enter to return to menu");
        while (1)
        {
            if (getch() == '\n')
            {
                break;
            }
        }
    }
}
void log_in()
{
    if (is_logged_in == 1)
    {
        mvprintw(1, 1, "You are logged in!press any key to exit.");
        getch();
        return;
    }
    else
    {
        User user;
        int ch;

        while (1)
        {
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
            if (fptr == NULL)
            {
                mvprintw(7, 1, "Error: Unable to open users file.");
                getch();
                return;
            }

            int valid = 0;
            while (fscanf(fptr, "%s %s %s %d %d %d %d %d", user.username, user.password, user.email, &user.score, &user.gold, &user.game, &user.color, &user.difficulty) != EOF)
            {
                if (strcmp(l_user.username, user.username) == 0 && strcmp(l_user.password, user.password) == 0)
                {
                    valid = 1;
                    strcpy(l_user.username, user.username);
                    strcpy(l_user.password, user.password);
                    strcpy(l_user.email, user.email);
                    l_user.score = user.score;
                    l_user.gold = user.gold;
                    l_user.game = user.game;
                    l_user.color = user.color;
                    l_user.difficulty = user.difficulty;
                    l_user.level_num = 1;
                    break;
                }
            }

            fclose(fptr);

            if (valid)
            {
                mvprintw(7, 1, "Login successful! Press any key to return to the main menu.");
                is_logged_in = 1;
                getch();
                return;
            }
            else
            {
                mvprintw(7, 1, "Invalid username or password!");
                mvprintw(9, 1, "Press 'r' to retry or any other key to return to the main menu.");
                ch = getch();

                if (ch != 'r' && ch != 'R')
                {
                    return;
                }
            }
        }
    }
}
int username_check(char *username)
{
    FILE *fptr = fopen("users.txt", "r");
    if (fptr == NULL)
    {
        return 0;
    }
    User user;
    while (fscanf(fptr, "%s %s %s %d %d %d %d %d", user.username, user.password, user.email, &user.score, &user.gold, &user.game, &user.color, &user.difficulty) != EOF)
    {
        if (strcmp(user.username, username) == 0)
        {
            fclose(fptr);
            return 1;
        }
    }
    fclose(fptr);
    return 0;
}
int pass_check(char *password)
{
    int sign1 = 0;
    int sign2 = 0;
    int sign3 = 0;
    int sign4 = 0;
    int index = 0;
    while (password[index] != '\0')
    {
        if (password[index] >= 'A' && password[index] <= 'Z')
        {
            sign1 = 1;
        }
        if (strlen(password) >= 7)
        {
            sign2 = 1;
        }
        if (password[index] >= '0' && password[index] <= '9')
        {
            sign3 = 1;
        }
        if (password[index] >= 'a' && password[index] <= 'z')
        {
            sign4 = 1;
        }
        index++;
    }
    return sign1 && sign2 && sign3 && sign4;
}
int email_check(char *email)
{
    int sign1 = 0;
    int sign2 = 0;
    int sign3 = 0;
    int index = 0;
    char *at = strchr(email, '@');
    char *dot = strrchr(email, '.');
    if (at == NULL || dot == NULL || at > dot)
    {
        return 0;
    }
    for (int i = 0; i < at - email; i++)
    {
        if ((email[i] >= 'A' && email[i] <= 'Z') || (email[i] >= 'a' && email[i] <= 'z'))
        {
            sign1 = 1;
            break;
        }
    }
    for (int i = at - email + 1; i < dot - email; i++)
    {
        if ((email[i] >= 'A' && email[i] <= 'Z') || (email[i] >= 'a' && email[i] <= 'z'))
        {
            sign2 = 1;
            break;
        }
    }
    for (int i = dot - email + 1; email[i] != '\0'; i++)
    {
        if ((email[i] >= 'A' && email[i] <= 'Z') || (email[i] >= 'a' && email[i] <= 'z'))
        {
            sign3 = 1;
            break;
        }
    }
    return sign1 && sign2 && sign3;
}
void play_game()
{
    char *play_menu_items[] = {
        "New Game",
        "Continue Your Last Game",
        "Score Table",
        "Profile Status",
        "Settings",
        "Back to Main Menu"};
    int n_items = sizeof(play_menu_items) / sizeof(play_menu_items[0]);
    int choice = 0;
    int key;
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    while (1)
    {
        clear();

        if (is_logged_in)
        {
            mvprintw(0, 1, "Play Game Menu");
            mvprintw(rows - 1, 1, "Logged in as: %s | Score: %d", l_user.username, l_user.score);
        }
        else
        {
            mvprintw(LINES / 2, COLS / 2 - 20, "Not logged in. Please log in or sign up.");
            getch();
            return;
        }

        for (int i = 0; i < n_items; i++)
        {
            if (i == choice)
                attron(COLOR_PAIR(2));
            mvprintw(i + 2, 1, "%s", play_menu_items[i]);
            if (i == choice)
                attroff(COLOR_PAIR(2));
        }

        key = getch();

        switch (key)
        {
        case KEY_UP:
            choice = (choice - 1 + n_items) % n_items;
            break;
        case KEY_DOWN:
            choice = (choice + 1) % n_items;
            break;
        case '\n':
            switch (choice)
            {
            case 0:
                start_new_game();
                break;
            case 1:
                continue_last_game();
                break;
            case 2:
                show_table();
                break;
            case 3:
                profile();
                break;
            case 4:
                settings_menu();
                break;
            case 5:
                return;
            }
            break;
        default:
            break;
        }
    }
}
void play_as_guest()
{
    clear();
    l_user.level_num = 1;
    l_user.gold = 0;
    l_user.health = 10000;
    l_user.power = 100;
    l_user.difficulty = 1;
    l_user.food_bar.normal = 0;
    l_user.food_bar.special = 0;
    l_user.spell_bar.H = 0;
    l_user.spell_bar.S = 0;
    l_user.spell_bar.G = 0;
    memset(memory_map1, 0, sizeof(memory_map1));
    create_map1();
    Player player = {5, 4, '.'};
    refresh_map(&player, memory_map1, map1);

    while (1)
    {
        handle_input(&player);
    }
}
void show_table()
{
    clear();
    mvprintw(1, 1, "Score Table:");

    FILE *file = fopen("users.txt", "r");
    if (!file)
    {
        mvprintw(3, 1, "Error: Unable to open users file.");
        getch();
        return;
    }

    User users[100];
    int user_count = 0;

    while (fscanf(file, "%s %s %s %d %d %d %d %d", users[user_count].username, users[user_count].password, users[user_count].email, &users[user_count].score, &users[user_count].gold, &users[user_count].game, &users[user_count].color, &users[user_count].difficulty) != EOF)
    {
        user_count++;
    }
    fclose(file);

    for (int i = 0; i < user_count - 1; i++)
    {
        for (int j = 0; j < user_count - i - 1; j++)
        {
            if (users[j].score < users[j + 1].score)
            {
                User temp = users[j];
                users[j] = users[j + 1];
                users[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < user_count; i++)
    {
        if (!strcmp(users[i].username, l_user.username))
        {
            attron(COLOR_PAIR(2));
        }
        if (i == 0)
        {
            attron(A_BOLD);
            mvprintw(3 + i, 1, "%d. %s - Score: %d gold %d: game-played: %d(THE GOAT)", i + 1, users[i].username, users[i].score, users[i].gold, users[i].game);
            attroff(A_BOLD);
        }
        else if (i == 1)
        {
            attron(A_BOLD);
            mvprintw(3 + i, 1, "%d. %s - Score: %d gold %d: game-played: %d(ALMOST GOAT)", i + 1, users[i].username, users[i].score, users[i].gold, users[i].game);
            attroff(A_BOLD);
        }
        else if (i == 2)
        {
            attron(A_BOLD);
            mvprintw(3 + i, 1, "%d. %s - Score: %d gold %d: game-played: %d(SEMI GOAT)", i + 1, users[i].username, users[i].score, users[i].gold, users[i].game);
            attroff(A_BOLD);
        }
        else
        {

            mvprintw(3 + i, 1, "%d. %s - Score: %d gold %d: game-played: %d", i + 1, users[i].username, users[i].score, users[i].gold, users[i].game);
        }
        if (!strcmp(users[i].username, l_user.username))
        {
            attroff(COLOR_PAIR(2));
        }
    }

    mvprintw(5 + user_count, 1, "Press any key to return.");
    getch();
}
void profile()
{
    if (is_logged_in)
    {
        while (1)
        {
            attron(COLOR_PAIR(4));
            mvprintw(1, 1, "Here is your information:");
            mvprintw(3, 1, "Name: %s", l_user.username);
            mvprintw(4, 1, "Password: %s", l_user.password);
            mvprintw(5, 1, "Email: %s", l_user.email);
            mvprintw(6, 1, "Score: %d", l_user.score);
            mvprintw(7, 1, "Gold: %d", l_user.gold);
            mvprintw(8, 1, "Game-played numbers: %d", l_user.game);

            attroff(COLOR_PAIR(4));
            attron(COLOR_PAIR(2));
            mvprintw(11, 1, "press Enter to exit");
            attroff(COLOR_PAIR(2));
            int ch = getch();
            if (ch == '\n')
            {
                return;
            }
        }
    }
    else
    {
        mvprintw(1, 1, "you are not logged in,press Enter to exit!");
        int ch = getch();
        if (ch == '\n')
        {
            return;
        }
    }
}
void eexit()
{
    mvprintw(22, 90, "GOODBYE!");
    getch();
    return;
}
void save_user()
{
    FILE *file = fopen("users.txt", "a");
    if (!file)
    {
        perror("Error opening file");
        exit(1);
    }

    fprintf(file, "%s %s %s %d %d %d %d %d\n", s_user.username, s_user.password, s_user.email, s_user.score, s_user.gold, s_user.game, s_user.color, s_user.difficulty);
    fclose(file);
}
void settings_menu()
{
    char *settings_items[] = {
        "Select Difficulty Level",
        "Change Character Color",
        "Back to Play Menu"};
    int n_items = sizeof(settings_items) / sizeof(settings_items[0]);
    int choice = 0;
    int key;

    static int current_difficulty = 0;
    static int current_color = 1;

    while (1)
    {
        clear();
        mvprintw(0, 1, "Settings Menu");

        for (int i = 0; i < n_items; i++)
        {
            if (i == choice)
                attron(COLOR_PAIR(2));
            mvprintw(i + 2, 1, "%s", settings_items[i]);
            if (i == choice)
                attroff(COLOR_PAIR(2));
        }

        key = getch();

        switch (key)
        {
        case KEY_UP:
            choice = (choice - 1 + n_items) % n_items;
            break;
        case KEY_DOWN:
            choice = (choice + 1) % n_items;
            break;
        case '\n':
            switch (choice)
            {
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
void select_difficulty(int *current_difficulty)
{
    char *difficulty_levels[] = {
        "Easy",
        "Medium",
        "Hard"};
    int n_levels = sizeof(difficulty_levels) / sizeof(difficulty_levels[0]);
    int choice = *current_difficulty;
    int key;

    while (1)
    {
        clear();
        mvprintw(0, 1, "Select Difficulty Level");

        for (int i = 0; i < n_levels; i++)
        {
            if (i == choice)
                attron(COLOR_PAIR(2));
            mvprintw(i + 2, 1, "%s%s", difficulty_levels[i], (i == *current_difficulty) ? " (Selected)" : "");
            if (i == choice)
                attroff(COLOR_PAIR(2));
        }

        mvprintw(n_levels + 4, 1, "Press ENTER to confirm, or ESC to cancel.");

        key = getch();

        switch (key)
        {
        case KEY_UP:
            choice = (choice - 1 + n_levels) % n_levels;
            break;
        case KEY_DOWN:
            choice = (choice + 1) % n_levels;
            break;
        case '\n':
            *current_difficulty = choice;
            mvprintw(n_levels + 6, 1, "Difficulty set to %s!", difficulty_levels[choice]);
            l_user.difficulty = choice;
            getch();
            return;
        case 27:
            return;
        }
    }
}
void change_character_color(int *current_color)
{
    char *colors[] = {
        "White",
        "Red",
        "Green",
        "Blue",
        "Yellow"};
    int n_colors = sizeof(colors) / sizeof(colors[0]);
    int choice = *current_color;
    int key;

    while (1)
    {
        clear();
        mvprintw(0, 1, "Change Character Color");

        for (int i = 0; i < n_colors; i++)
        {
            if (i == choice)
                attron(COLOR_PAIR(2));
            mvprintw(i + 2, 1, "%s%s", colors[i], (i == *current_color) ? " " : "");
            if (i == choice)
                attroff(COLOR_PAIR(2));
        }

        mvprintw(n_colors + 4, 1, "Press ENTER to confirm, or ESC to cancel.");

        key = getch();

        switch (key)
        {
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
            if (strcmp(colors[choice], "White") == 0)
            {
                l_user.color = 4;
            }
            else if (strcmp(colors[choice], "Red") == 0)
            {
                l_user.color = 2;
            }
            else if (strcmp(colors[choice], "Green") == 0)
            {
                l_user.color = 3;
            }
            else if (strcmp(colors[choice], "Blue") == 0)
            {
                l_user.color = 5;
            }
            else if (strcmp(colors[choice], "Yellow") == 0)
            {
                l_user.color = 1;
            }
            return;
        case 27:
            return;
        }
    }
}
void start_new_game()
{
    clear();
    l_user.level_num = 1;
    l_user.gold = 0;
    l_user.health = 10000;
    l_user.power = 100;
    l_user.food_bar.normal = 0;
    l_user.food_bar.special = 0;
    l_user.spell_bar.H = 0;
    l_user.spell_bar.S = 0;
    l_user.spell_bar.G = 0;
    memset(memory_map1, 0, sizeof(memory_map1));
    create_map1();
    Player player = {5, 4, '.'};
    refresh_map(&player, memory_map1, map1);

    while (1)
    {
        handle_input(&player);
    }
}
void start_level2()
{
    clear();
    show_count = 0;
    l_user.level_num = 2;
    pace_counter2 = 0;
    memset(memory_map2, 0, sizeof(memory_map2));
    create_map2();
    Player player = {12, 10, '.'};
    refresh_map(&player, memory_map2, map2);

    while (1)
    {
        handle_input(&player);
    }
}
void start_level3()
{
    clear();
    show_count = 0;
    l_user.level_num = 3;
    pace_counter2 = 0;
    memset(memory_map3, 0, sizeof(memory_map3));
    create_map3();
    Player player = {20, 26, '.'};
    refresh_map(&player, memory_map3, map3);

    while (1)
    {
        handle_input(&player);
    }
}
void start_level4()
{
    clear();
    show_count = 0;
    pace_counter2 = 0;
    l_user.level_num = 4;
    memset(memory_map4, 0, sizeof(memory_map4));
    create_map4();
    Player player = {48, 35, '.'};
    refresh_map(&player, memory_map4, map4);

    while (1)
    {
        handle_input(&player);
    }
}
void continue_last_game()
{
}
int is_valid_move(int x, int y, char map[MAP_HEIGHT][MAP_WIDTH])
{
    char ch = map[y][x];
    return ch == '.' || ch == '#' || ch == '+' || ch == '<' || ch == '&' || ch == 'G' || ch == 'T' || ch == 'Z' || ch == 'X' || ch == '%' | ch == 'S' | ch == 'H' | ch == '8' || ch == '^' || ch == '?';
}
void draw_player(Player *player)
{
    if (l_user.color == 4)
    {
        attron(COLOR_PAIR(4));
    }
    else if (l_user.color == 2)
    {
        attron(COLOR_PAIR(2));
    }
    else if (l_user.color == 3)
    {
        attron(COLOR_PAIR(3));
    }
    else if (l_user.color == 5)
    {
        attron(COLOR_PAIR(5));
    }
    else if (l_user.color == 1)
    {
        attron(COLOR_PAIR(1));
    }
    mvprintw(player->y, player->x, "@");
    if (l_user.color == 4)
    {
        attroff(COLOR_PAIR(4));
    }
    else if (l_user.color == 2)
    {
        attroff(COLOR_PAIR(2));
    }
    else if (l_user.color == 3)
    {
        attroff(COLOR_PAIR(3));
    }
    else if (l_user.color == 5)
    {
        attroff(COLOR_PAIR(5));
    }
    else if (l_user.color == 1)
    {
        attroff(COLOR_PAIR(1));
    }
}
void clear_player(Player *player)
{
    if (fmsign == 0)
    {
        mvprintw(player->y, player->x, "%c", '.');
        fmsign++;
    }
    else if (l_user.level_num == 1)
    {
        if (player->prev_char == '^')
        {
            map1[player->y][player->x] = '?';
        }
        else if (player->y == 9 && player->x == 100)
        {
            map1[9][100] = '|';
        }
        else if (player->y == 34 && player->x == 110)
        {
            map1[34][110] = '|';
        }
        else
        {
            mvprintw(player->y, player->x, "%c", player->prev_char);
        }
    }
    else if (l_user.level_num == 2)
    {
        if (player->prev_char == '^')
        {
            map2[player->y][player->x] = '?';
        }
        else if (player->y == 7 && player->x == 120)
        {
            map2[7][120] = '|';
        }
        else if (player->y == 17 && player->x == 60)
        {
            map2[17][60] = '|';
        }
        else
        {
            mvprintw(player->y, player->x, "%c", player->prev_char);
        }
    }
    else if (l_user.level_num == 3)
    {
        if (player->prev_char == '^')
        {
            map3[player->y][player->x] = '?';
        }
        else if (player->y == 39 && player->x == 160)
        {
            map3[39][160] = '|';
        }
        else if (player->y == 8 && player->x == 158)
        {
            map3[8][158] = '-';
        }
        else
        {
            mvprintw(player->y, player->x, "%c", player->prev_char);
        }
    }
    else if (l_user.level_num == 4)
    {
        if (player->prev_char == '^')
        {
            map4[player->y][player->x] = '?';
        }
        else if (player->y == 32 && player->x == 94)
        {
            map4[32][94] = '|';
        }
        else if (player->y == 12 && player->x == 127)
        {
            map4[12][127] = '-';
        }
        else
        {
            mvprintw(player->y, player->x, "%c", player->prev_char);
        }
    }
}
void draw_path(int x1, int y1, int x2, int y2, char map[MAP_HEIGHT][MAP_WIDTH])
{
    int cx = x1, cy = y1;

    while (cx != x2 || cy != y2)
    {
        map[cy][cx] = '#';

        int dx = (cx < x2) - (cx > x2);
        int dy = (cy < y2) - (cy > y2);

        if (rand() % 2 == 0 && map[cy][cx + dx] != '|' && map[cy][cx + dx] != '-')
        {
            cx += dx;
        }
        else if (map[cy + dy][cx] != '|' && map[cy + dy][cx] != '-')
        {
            cy += dy;
        }
        else if (map[cy + dy][cx + dx] != '|' && map[cy + dy][cx + dx] != '-')
        {
            cx += dx;
            cy += dy;
        }
        else
        {
            break;
        }
    }
}
int create_map1()
{
    memset(map1, ' ', sizeof(map1));
    // room 1
    for (int i = 4; i < 9; i++)
    {
        map1[i][3] = '|';
        map1[i][23] = '|';
    }
    for (int i = 3; i <= 23; i++)
    {
        map1[3][i] = '-';
        map1[9][i] = '-';
    }
    for (int i = 4; i < 9; i++)
    {
        for (int j = 4; j < 23; j++)
        {
            map1[i][j] = '.';
        }
    }
    int n1 = rand() % 5 + 3;
    for (int i = 0; i < n1; i++)
    {
        int x = rand() % 5 + 4;
        int y = rand() % 19 + 4;
        if (map1[x][y] != '.')
        {
            continue;
        }
        map1[x][y] = '%';
    }
    int t1 = rand() % 3 + 1;
    for (int i = 0; i < t1; i++)
    {
        int x = rand() % 5 + 4;
        int y = rand() % 19 + 4;
        if (map1[x][y] != '.')
        {
            continue;
        }
        map1[x][y] = 'T';
    }
    int q1 = rand() % 3;
    for (int i = 0; i < q1; i++)
    {
        int x = rand() % 5 + 4;
        int y = rand() % 19 + 4;
        if (map1[x][y] != '.')
        {
            continue;
        }
        int p = rand() % 3;
        if (p == 0)
        {
            map1[x][y] = 'S';
        }
        else if (p == 1)
        {
            map1[x][y] = 'H';
        }
        else if (p == 2)
        {
            map1[x][y] = '8';
        }
    }

    map1[5][23] = '+';
    map1[7][5] = 'o';
    map1[5][15] = 'o';
    map1[8][9] = '<';

    //  room ۲
    for (int i = 8; i < 20; i++)
    {
        map1[i][50] = '|';
        map1[i][65] = '|';
    }
    for (int i = 50; i <= 65; i++)
    {
        map1[7][i] = '-';
        map1[20][i] = '-';
    }
    for (int i = 8; i <= 19; i++)
    {
        for (int j = 51; j < 65; j++)
        {
            map1[i][j] = '.';
        }
    }
    int n2 = rand() % 5 + 4;
    for (int i = 0; i < n2; i++)
    {
        int x = rand() % 12 + 8;
        int y = rand() % 14 + 51;
        if (map1[x][y] != '.')
        {
            continue;
        }
        map1[x][y] = '%';
    }
    for (int i = 0; i < 3; i++)
    {
        int x = rand() % 12 + 8;
        int y = rand() % 14 + 51;
        map1[x][y] = '^';
    }

    int m2 = rand() % 3;
    for (int i = 0; i < m2; i++)
    {
        int x = rand() % 12 + 8;
        int y = rand() % 14 + 51;
        if (map1[x][y] != '.')
        {
            continue;
        }
        map1[x][y] = 'T';
    }
    int q2 = rand() % 3;
    for (int i = 0; i < q2; i++)
    {
        int x = rand() % 12 + 8;
        int y = rand() % 14 + 51;
        if (map1[x][y] != '.')
        {
            continue;
        }
        int p = rand() % 3;
        if (p == 0)
        {
            map1[x][y] = 'S';
        }
        else if (p == 1)
        {
            map1[x][y] = 'H';
        }
        else if (p == 2)
        {
            map1[x][y] = '8';
        }
    }
    map1[7][58] = '+';
    map1[18][65] = '+';
    map1[6][45] = 'o';
    map1[8][41] = 'o';

    //  room ۳
    for (int i = 6; i < 17; i++)
    {
        map1[i][100] = '|';
        map1[i][111] = '|';
    }
    for (int i = 100; i <= 111; i++)
    {
        map1[6][i] = '-';
        map1[16][i] = '-';
    }
    for (int i = 7; i < 16; i++)
    {
        for (int j = 101; j < 111; j++)
        {
            map1[i][j] = '.';
        }
    }
    int n3 = rand() % 4 + 3;
    for (int i = 0; i < n3; i++)
    {
        int x = rand() % 9 + 7;
        int y = rand() % 10 + 101;
        if (map1[x][y] != '.')
        {
            continue;
        }
        map1[x][y] = '%';
    }
    int m3 = rand() % 3;
    for (int i = 0; i < m3; i++)
    {
        int x = rand() % 9 + 7;
        int y = rand() % 10 + 101;
        if (map1[x][y] != '.')
        {
            continue;
        }
        map1[x][y] = 'T';
    }
    int q3 = rand() % 3;
    for (int i = 0; i < q3; i++)
    {
        int x = rand() % 9 + 7;
        int y = rand() % 10 + 101;
        if (map1[x][y] != '.')
        {
            continue;
        }
        int p = rand() % 3;
        if (p == 0)
        {
            map1[x][y] = 'S';
        }
        else if (p == 1)
        {
            map1[x][y] = 'H';
        }
        else if (p == 2)
        {
            map1[x][y] = '8';
        }
    }
    for (int i = 0; i < 2; i++)
    {
        int x = rand() % 9 + 7;
        int y = rand() % 10 + 101;
        map1[x][y] = '^';
    }

    map1[9][100] = '+';
    // map1[14][111] = '+';
    map1[10][105] = 'o';
    map1[14][102] = 'o';
    map1[15][109] = 'o';

    //  room ۴
    for (int i = 20; i < 30; i++)
    {
        map1[i][150] = '|';
        map1[i][159] = '|';
    }
    for (int i = 150; i <= 159; i++)
    {
        map1[20][i] = '-';
        map1[29][i] = '-';
    }
    for (int i = 21; i < 29; i++)
    {
        for (int j = 151; j < 159; j++)
        {
            map1[i][j] = '.';
        }
    }
    int n4 = rand() % 4 + 3;
    for (int i = 0; i < n4; i++)
    {
        int x = rand() % 8 + 21;
        int y = rand() % 8 + 151;
        if (map1[x][y] != '.')
        {
            continue;
        }
        map1[x][y] = '%';
    }
    int m4 = rand() % 3;
    for (int i = 0; i < m4; i++)
    {
        int x = rand() % 8 + 21;
        int y = rand() % 8 + 151;
        if (map1[x][y] != '.')
        {
            continue;
        }
        map1[x][y] = 'T';
    }
    int a4 = rand() % 2 + 2;
    for (int i = 0; i < a4; i++)
    {
        int x = rand() % 8 + 21;
        int y = rand() % 8 + 151;
        if (map1[x][y] != '.')
        {
            continue;
        }
        map1[x][y] = 'X';
    }
    int q4 = rand() % 2 + 1;
    for (int i = 0; i < q4; i++)
    {
        int x = rand() % 8 + 21;
        int y = rand() % 8 + 151;
        if (map1[x][y] != '.')
        {
            continue;
        }
        int p = rand() % 3;
        if (p == 0)
        {
            map1[x][y] = 'S';
        }
        else if (p == 1)
        {
            map1[x][y] = 'H';
        }
        else if (p == 2)
        {
            map1[x][y] = '8';
        }
    }
    map1[28][158] = '&';
    map1[20][152] = '+';
    map1[25][150] = '@';
    map1[22][152] = 'o';
    map1[28][156] = 'o';

    //  room ۵
    for (int i = 30; i < 36; i++)
    {
        map1[i][94] = '|';
        map1[i][110] = '|';
    }
    for (int i = 94; i <= 110; i++)
    {
        map1[30][i] = '-';
        map1[35][i] = '-';
    }
    for (int i = 31; i < 35; i++)
    {
        for (int j = 95; j < 110; j++)
        {
            map1[i][j] = '.';
        }
    }
    int n5 = rand() % 3 + 3;
    for (int i = 0; i < n5; i++)
    {
        int x = rand() % 4 + 31;
        int y = rand() % 15 + 95;
        if (map1[x][y] != '.')
        {
            continue;
        }
        map1[x][y] = '%';
    }
    int m5 = rand() % 3;
    for (int i = 0; i < m5; i++)
    {
        int x = rand() % 4 + 31;
        int y = rand() % 15 + 95;
        if (map1[x][y] != '.')
        {
            continue;
        }
        map1[x][y] = 'T';
    }
    int q5 = rand() % 3 + 4;
    for (int i = 0; i < q5; i++)
    {
        int x = rand() % 4 + 31;
        int y = rand() % 15 + 95;
        if (map1[x][y] != '.')
        {
            continue;
        }
        int p = rand() % 3;
        if (p == 0)
        {
            map1[x][y] = 'S';
        }
        else if (p == 1)
        {
            map1[x][y] = 'H';
        }
        else if (p == 2)
        {
            map1[x][y] = '8';
        }
    }
    int x = rand() % 4 + 31;
    int y = rand() % 15 + 95;
    map1[x][y] = '^';
    // map1[32][94] = '+';
    map1[34][110] = '+';
    map1[31][99] = 'o';
    map1[33][107] = 'o';

    //  room ۶
    for (int i = 28; i < 38; i++)
    {
        map1[i][10] = '|';
        map1[i][20] = '|';
    }
    for (int i = 10; i <= 20; i++)
    {
        map1[28][i] = '-';
        map1[37][i] = '-';
    }
    for (int i = 29; i < 37; i++)
    {
        for (int j = 11; j < 20; j++)
        {
            map1[i][j] = '.';
        }
    }
    int n6 = rand() % 3 + 2;
    for (int i = 0; i < n6; i++)
    {
        int x = rand() % 8 + 29;
        int y = rand() % 9 + 11;
        if (map1[x][y] != '.')
        {
            continue;
        }
        map1[x][y] = '%';
    }
    int m6 = rand() % 2 + 1;
    for (int i = 0; i < m6; i++)
    {
        int x = rand() % 8 + 29;
        int y = rand() % 9 + 11;
        if (map1[x][y] != '.')
        {
            continue;
        }
        map1[x][y] = 'Z';
    }
    int q6 = rand() % 3 + 4;
    for (int i = 0; i < q6; i++)
    {
        int x = rand() % 8 + 29;
        int y = rand() % 9 + 11;
        if (map1[x][y] != '.')
        {
            continue;
        }
        int p = rand() % 2 + 1;
        if (p == 0)
        {
            map1[x][y] = 'S';
        }
        else if (p == 1)
        {
            map1[x][y] = 'H';
        }
        else if (p == 2)
        {
            map1[x][y] = '8';
        }
    }
    for (int i = 0; i < 2; i++)
    {
        int x = rand() % 8 + 29;
        int y = rand() % 9 + 11;
        map1[x][y] = '^';
    }
    map1[30][20] = '+';
    map1[30][12] = 'o';
    map1[36][17] = 'o';
    map1[36][13] = '<';

    // ایجاد راهروها
    draw_path(24, 5, 59, 6, map1);
    draw_path(66, 18, 100, 9, map1);
    draw_path(112, 14, 153, 19, map1);
    draw_path(149, 25, 110, 34, map1);
    draw_path(93, 32, 20, 30, map1);

    return 0;
}
int create_map2()
{
    memset(map2, ' ', sizeof(map2));

    //  room ۱
    for (int i = 6; i < 15; i++)
    {
        map2[i][10] = '|';
        map2[i][18] = '|';
    }
    for (int i = 10; i <= 18; i++)
    {
        map2[6][i] = '-';
        map2[14][i] = '-';
    }
    for (int i = 7; i < 14; i++)
    {
        for (int j = 11; j < 18; j++)
        {
            map2[i][j] = '.';
        }
    }
    int q1 = rand() % 3 + 7;
    for (int i = 0; i < q1; i++)
    {
        int x = rand() % 7 + 7;
        int y = rand() % 7 + 11;
        if (map2[x][y] != '.')
        {
            continue;
        }
        int p = rand() % 3;
        if (p == 0)
        {
            map2[x][y] = 'S';
        }
        else if (p == 1)
        {
            map2[x][y] = 'H';
        }
        else if (p == 2)
        {
            map2[x][y] = '8';
        }
    }
    // map2[7][18] = '+';
    map2[10][12] = 'o';
    map2[8][16] = 'o';
    map2[9][17] = '<';

    //  room ۲
    for (int i = 5; i < 14; i++)
    {
        map2[i][35] = '|';
        map2[i][48] = '|';
    }
    for (int i = 35; i <= 48; i++)
    {
        map2[5][i] = '-';
        map2[13][i] = '-';
    }
    for (int i = 6; i < 13; i++)
    {
        for (int j = 36; j < 48; j++)
        {
            map2[i][j] = '.';
        }
    }
    int n1 = rand() % 3 + 4;
    for (int i = 0; i < n1; i++)
    {
        int x = rand() % 7 + 6;
        int y = rand() % 12 + 36;
        if (map2[x][y] != '.')
        {
            continue;
        }
        map2[x][y] = '%';
    }
    int t1 = rand() % 2 + 1;
    for (int i = 0; i < t1; i++)
    {
        int x = rand() % 7 + 6;
        int y = rand() % 12 + 36;
        if (map2[x][y] != '.')
        {
            continue;
        }
        map2[x][y] = 'T';
    }
    map2[6][36] = '&';
    map2[12][35] = '+';
    map2[9][48] = '@';
    map2[11][45] = 'o';
    map2[9][37] = 'o';

    //  room ۳
    for (int i = 3; i < 13; i++)
    {
        map2[i][120] = '|';
        map2[i][135] = '|';
    }
    for (int i = 120; i <= 135; i++)
    {
        map2[3][i] = '-';
        map2[12][i] = '-';
    }
    for (int i = 4; i < 12; i++)
    {
        for (int j = 121; j < 135; j++)
        {
            map2[i][j] = '.';
        }
    }
    int n3 = rand() % 3 + 4;
    for (int i = 0; i < n3; i++)
    {
        int x = rand() % 8 + 4;
        int y = rand() % 14 + 121;
        if (map2[x][y] != '.')
        {
            continue;
        }
        map2[x][y] = '%';
    }
    int q3 = rand() % 3 + 2;
    for (int i = 0; i < q3; i++)
    {
        int x = rand() % 8 + 4;
        int y = rand() % 14 + 121;
        if (map2[x][y] != '.')
        {
            continue;
        }
        int p = rand() % 3;
        if (p == 0)
        {
            map2[x][y] = 'S';
        }
        else if (p == 1)
        {
            map2[x][y] = 'H';
        }
        else if (p == 2)
        {
            map2[x][y] = '8';
        }
    }
    map2[7][120] = '+';
    map2[5][126] = 'o';
    map2[6][131] = 'o';
    map2[11][122] = 'o';

    //  room ۴
    for (int i = 30; i < 42; i++)
    {
        map2[i][160] = '|';
        map2[i][175] = '|';
    }
    for (int i = 160; i <= 175; i++)
    {
        map2[30][i] = '-';
        map2[41][i] = '-';
    }
    for (int i = 31; i < 41; i++)
    {
        for (int j = 161; j < 175; j++)
        {
            map2[i][j] = '.';
        }
    }
    int n2 = rand() % 3 + 2;
    for (int i = 0; i < n2; i++)
    {
        int x = rand() % 10 + 31;
        int y = rand() % 14 + 161;
        if (map2[x][y] != '.')
        {
            continue;
        }
        map2[x][y] = '%';
    }
    int t2 = rand() % 2 + 2;
    for (int i = 0; i < t2; i++)
    {
        int x = rand() % 10 + 31;
        int y = rand() % 14 + 161;
        if (map2[x][y] != '.')
        {
            continue;
        }
        map2[x][y] = 'T';
    }
    int q4 = rand() % 3 + 1;
    for (int i = 0; i < q4; i++)
    {
        int x = rand() % 10 + 31;
        int y = rand() % 14 + 161;
        if (map2[x][y] != '.')
        {
            continue;
        }
        int p = rand() % 3;
        if (p == 0)
        {
            map2[x][y] = 'S';
        }
        else if (p == 1)
        {
            map2[x][y] = 'H';
        }
        else if (p == 2)
        {
            map2[x][y] = '8';
        }
    }
    map2[30][167] = '+';
    map2[39][160] = '+';
    map2[35][169] = 'o';
    map2[40][163] = 'o';

    //  room ۵
    for (int i = 30; i < 36; i++)
    {
        map2[i][94] = '|';
        map2[i][110] = '|';
    }
    for (int i = 94; i <= 110; i++)
    {
        map2[30][i] = '-';
        map2[35][i] = '-';
    }
    for (int i = 31; i < 35; i++)
    {
        for (int j = 95; j < 110; j++)
        {
            map2[i][j] = '.';
        }
    }
    int n5 = rand() % 3 + 3;
    for (int i = 0; i < n5; i++)
    {
        int x = rand() % 4 + 31;
        int y = rand() % 15 + 95;
        if (map2[x][y] != '.')
        {
            continue;
        }
        map2[x][y] = '%';
    }
    int q5 = rand() % 3 + 5;
    for (int i = 0; i < q5; i++)
    {
        int x = rand() % 4 + 31;
        int y = rand() % 15 + 95;
        if (map2[x][y] != '.')
        {
            continue;
        }
        int p = rand() % 3;
        if (p == 0)
        {
            map2[x][y] = 'S';
        }
        else if (p == 1)
        {
            map2[x][y] = 'H';
        }
        else if (p == 2)
        {
            map2[x][y] = '8';
        }
    }
    map2[33][108] = '&';
    map2[32][94] = '@';
    map2[34][110] = '+';
    map2[31][99] = 'o';
    map2[33][107] = 'o';

    //  room ۶
    for (int i = 15; i < 23; i++)
    {
        map2[i][50] = '|';
        map2[i][60] = '|';
    }
    for (int i = 50; i <= 60; i++)
    {
        map2[15][i] = '-';
        map2[22][i] = '-';
    }
    for (int i = 16; i < 22; i++)
    {
        for (int j = 51; j < 60; j++)
        {
            map2[i][j] = '.';
        }
    }
    int n6 = rand() % 3 + 2;
    for (int i = 0; i < n6; i++)
    {
        int x = rand() % 6 + 16;
        int y = rand() % 9 + 51;
        if (map2[x][y] != '.')
        {
            continue;
        }
        map2[x][y] = '%';
    }
    int t3 = rand() % 2 + 1;
    for (int i = 0; i < t3; i++)
    {
        int x = rand() % 6 + 166;
        int y = rand() % 9 + 51;
        if (map2[x][y] != '.')
        {
            continue;
        }
        map2[x][y] = 'T';
    }
    int q6 = rand() % 3 + 1;
    for (int i = 0; i < q6; i++)
    {
        int x = rand() % 6 + 166;
        int y = rand() % 9 + 51;
        if (map2[x][y] != '.')
        {
            continue;
        }
        int p = rand() % 3;
        if (p == 0)
        {
            map2[x][y] = 'S';
        }
        else if (p == 1)
        {
            map2[x][y] = 'H';
        }
        else if (p == 2)
        {
            map2[x][y] = '8';
        }
    }
    map2[17][60] = '+';
    map2[20][53] = 'o';
    map2[19][57] = 'o';

    //  room ۷
    for (int i = 28; i < 33; i++)
    {
        map2[i][11] = '|';
        map2[i][26] = '|';
    }
    for (int i = 11; i <= 26; i++)
    {
        map2[28][i] = '-';
        map2[32][i] = '-';
    }
    for (int i = 29; i < 32; i++)
    {
        for (int j = 12; j < 26; j++)
        {
            map2[i][j] = '.';
        }
    }
    int q7 = rand() % 3;
    for (int i = 0; i < q7; i++)
    {
        int x = rand() % 3 + 29;
        int y = rand() % 14 + 12;
        if (map2[x][y] != '.')
        {
            continue;
        }
        int p = rand() % 3;
        if (p == 0)
        {
            map2[x][y] = 'S';
        }
        else if (p == 1)
        {
            map2[x][y] = 'H';
        }
        else if (p == 2)
        {
            map2[x][y] = '8';
        }
    }
    int n7 = rand() % 3 + 2;
    for (int i = 0; i < n7; i++)
    {
        int x = rand() % 3 + 29;
        int y = rand() % 14 + 12;
        if (map2[x][y] != '.')
        {
            continue;
        }
        map2[x][y] = '%';
    }
    map2[28][20] = '+';
    map2[30][12] = 'o';
    map2[29][17] = 'o';

    // ایجاد راهروها
    draw_path(19, 7, 35, 12, map2);
    draw_path(49, 9, 120, 7, map2);
    draw_path(127, 13, 168, 29, map2);
    draw_path(159, 39, 110, 34, map2);
    draw_path(93, 32, 60, 17, map2);
    draw_path(49, 21, 19, 27, map2);
}
int create_map3()
{
    memset(map3, ' ', sizeof(map3));

    //  room ۱
    for (int i = 6; i < 16; i++)
    {
        map3[i][35] = '|';
        map3[i][41] = '|';
    }
    for (int i = 35; i <= 41; i++)
    {
        map3[6][i] = '-';
        map3[15][i] = '-';
    }
    for (int i = 7; i < 15; i++)
    {
        for (int j = 36; j < 41; j++)
        {
            map3[i][j] = '.';
        }
    }
    int n3 = rand() % 3 + 1;
    for (int i = 0; i < n3; i++)
    {
        int x = rand() % 8 + 7;
        int y = rand() % 5 + 36;
        if (map3[x][y] != '.')
        {
            continue;
        }
        map3[x][y] = '%';
    }
    int t3 = rand() % 2 + 1;
    for (int i = 0; i < t3; i++)
    {
        int x = rand() % 8 + 7;
        int y = rand() % 5 + 36;
        if (map3[x][y] != '.')
        {
            continue;
        }
        map3[x][y] = 'T';
    }
    int m3 = rand() % 2 + 1;
    for (int i = 0; i < m3; i++)
    {
        int x = rand() % 8 + 7;
        int y = rand() % 5 + 36;
        if (map3[x][y] != '.')
        {
            continue;
        }
        map3[x][y] = 'X';
    }
    int q1 = rand() % 4;
    for (int i = 0; i < q1; i++)
    {
        int x = rand() % 8 + 7;
        int y = rand() % 5 + 36;
        if (map3[x][y] != '.')
        {
            continue;
        }
        int p = rand() % 3;
        if (p == 0)
        {
            map3[x][y] = 'S';
        }
        else if (p == 1)
        {
            map3[x][y] = 'H';
        }
        else if (p == 2)
        {
            map3[x][y] = '8';
        }
    }
    map3[12][41] = '+';
    map3[11][40] = 'o';
    map3[9][37] = 'o';

    //  room ۲
    for (int i = 12; i < 17; i++)
    {
        map3[i][70] = '|';
        map3[i][84] = '|';
    }
    for (int i = 70; i <= 84; i++)
    {
        map3[12][i] = '-';
        map3[16][i] = '-';
    }
    for (int i = 13; i < 16; i++)
    {
        for (int j = 71; j < 84; j++)
        {
            map3[i][j] = '.';
        }
    }
    int n9 = rand() % 4 + 2;
    for (int i = 0; i < n9; i++)
    {
        int x = rand() % 3 + 13;
        int y = rand() % 13 + 71;
        if (map3[x][y] != '.')
        {
            continue;
        }
        map3[x][y] = '%';
    }
    int t9 = rand() % 4 + 2;
    for (int i = 0; i < t9; i++)
    {
        int x = rand() % 3 + 13;
        int y = rand() % 13 + 71;
        if (map3[x][y] != '.')
        {
            continue;
        }
        map3[x][y] = 't';
    }
    int q2 = rand() % 3 + 1;
    for (int i = 0; i < q2; i++)
    {
        int x = rand() % 3 + 13;
        int y = rand() % 13 + 71;
        if (map3[x][y] != '.')
        {
            continue;
        }
        int p = rand() % 3;
        if (p == 0)
        {
            map3[x][y] = 'S';
        }
        else if (p == 1)
        {
            map3[x][y] = 'H';
        }
        else if (p == 2)
        {
            map3[x][y] = '8';
        }
    }
    map3[15][72] = '&';
    map3[13][70] = '@';
    map3[14][84] = '+';
    map3[13][75] = 'o';
    map3[15][80] = 'o';

    //  room ۳
    for (int i = 5; i < 20; i++)
    {
        map3[i][125] = '|';
        map3[i][145] = '|';
    }
    for (int i = 125; i <= 145; i++)
    {
        map3[5][i] = '-';
        map3[19][i] = '-';
    }
    for (int i = 6; i < 19; i++)
    {
        for (int j = 126; j < 145; j++)
        {
            map3[i][j] = '.';
        }
    }
    int n1 = rand() % 3 + 3;
    for (int i = 0; i < n1; i++)
    {
        int x = rand() % 13 + 6;
        int y = rand() % 9 + 126;
        if (map3[x][y] != '.')
        {
            continue;
        }
        map3[x][y] = '%';
    }
    int t1 = rand() % 5 + 3;
    for (int i = 0; i < t1; i++)
    {
        int x = rand() % 13 + 6;
        int y = rand() % 9 + 126;
        if (map3[x][y] != '.')
        {
            continue;
        }
        map3[x][y] = 'T';
    }
    int m1 = rand() % 2 + 1;
    for (int i = 0; i < t3; i++)
    {
        int x = rand() % 13 + 6;
        int y = rand() % 9 + 126;
        if (map3[x][y] != '.')
        {
            continue;
        }
        map3[x][y] = 'X';
    }
    int q3 = rand() % 3 + 1;
    for (int i = 0; i < q3; i++)
    {
        int x = rand() % 13 + 6;
        int y = rand() % 9 + 126;
        if (map3[x][y] != '.')
        {
            continue;
        }
        int p = rand() % 3;
        if (p == 0)
        {
            map3[x][y] = 'S';
        }
        else if (p == 1)
        {
            map3[x][y] = 'H';
        }
        else if (p == 2)
        {
            map3[x][y] = '8';
        }
    }
    // mvprintw(7,125,"+");
    map3[19][132] = '+';
    map3[10][140] = 'o';
    map3[15][136] = 'o';
    map3[18][130] = 'o';

    //  room ۴
    for (int i = 30; i < 42; i++)
    {
        map3[i][160] = '|';
        map3[i][175] = '|';
    }
    for (int i = 160; i <= 175; i++)
    {
        map3[30][i] = '-';
        map3[41][i] = '-';
    }
    for (int i = 31; i < 41; i++)
    {
        for (int j = 161; j < 175; j++)
        {
            map3[i][j] = '.';
        }
    }
    int n2 = rand() % 3 + 4;
    for (int i = 0; i < n2; i++)
    {
        int x = rand() % 10 + 31;
        int y = rand() % 14 + 161;
        if (map3[x][y] != '.')
        {
            continue;
        }
        map3[x][y] = '%';
    }
    int t2 = rand() % 5 + 3;
    for (int i = 0; i < t2; i++)
    {
        int x = rand() % 10 + 31;
        int y = rand() % 14 + 161;
        if (map3[x][y] != '.')
        {
            continue;
        }
        map3[x][y] = 'T';
    }
    int m2 = rand() % 2 + 1;
    for (int i = 0; i < m2; i++)
    {
        int x = rand() % 10 + 31;
        int y = rand() % 14 + 161;
        if (map3[x][y] != '.')
        {
            continue;
        }
        map3[x][y] = 'Z';
    }
    int q4 = rand() % 3;
    for (int i = 0; i < q4; i++)
    {
        int x = rand() % 10 + 31;
        int y = rand() % 14 + 161;
        if (map3[x][y] != '.')
        {
            continue;
        }
        int p = rand() % 3;
        if (p == 0)
        {
            map3[x][y] = 'S';
        }
        else if (p == 1)
        {
            map3[x][y] = 'H';
        }
        else if (p == 2)
        {
            map3[x][y] = '8';
        }
    }
    // mvprintw(30,167,"+");
    map3[39][160] = '+';
    map3[35][169] = 'o';
    map3[40][163] = 'o';

    //  room ۵
    for (int i = 30; i < 36; i++)
    {
        map3[i][84] = '|';
        map3[i][100] = '|';
    }
    for (int i = 84; i <= 100; i++)
    {
        map3[30][i] = '-';
        map3[35][i] = '-';
    }
    for (int i = 31; i < 35; i++)
    {
        for (int j = 85; j < 100; j++)
        {
            map3[i][j] = '.';
        }
    }
    map3[34][87] = '&';
    map3[32][84] = '+';
    map3[34][100] = '@';
    map3[31][99] = 'o';
    map3[33][86] = 'o';
    int t5 = rand() % 5 + 3;
    for (int i = 0; i < t5; i++)
    {
        int x = rand() % 4 + 31;
        int y = rand() % 15 + 85;
        if (map3[x][y] != '.')
        {
            continue;
        }
        map3[x][y] = 'T';
    }
    int a5 = rand() % 2 + 1;
    for (int i = 0; i < a5; i++)
    {
        int x = rand() % 4 + 31;
        int y = rand() % 15 + 85;
        if (map3[x][y] != '.')
        {
            continue;
        }
        map3[x][y] = 'Z';
    }
    int n5 = rand() % 5 + 3;
    for (int i = 0; i < n5; i++)
    {
        int x = rand() % 4 + 31;
        int y = rand() % 15 + 85;
        if (map3[x][y] != '.')
        {
            continue;
        }
        map3[x][y] = '%';
    }
    int q5 = rand() % 3 + 2;
    for (int i = 0; i < q5; i++)
    {
        int x = rand() % 4 + 31;
        int y = rand() % 15 + 85;
        if (map3[x][y] != '.')
        {
            continue;
        }
        int p = rand() % 3;
        if (p == 0)
        {
            map3[x][y] = 'S';
        }
        else if (p == 1)
        {
            map3[x][y] = 'H';
        }
        else if (p == 2)
        {
            map3[x][y] = '8';
        }
    }
    //  room ۶
    for (int i = 21; i < 35; i++)
    {
        map3[i][12] = '|';
        map3[i][26] = '|';
    }
    for (int i = 12; i <= 26; i++)
    {
        map3[21][i] = '-';
        map3[34][i] = '-';
    }
    for (int i = 22; i < 34; i++)
    {
        for (int j = 13; j < 26; j++)
        {
            map3[i][j] = '.';
        }
    }
    int n7 = rand() % 5 + 4;
    for (int i = 0; i < n7; i++)
    {
        int x = rand() % 12 + 22;
        int y = rand() % 13 + 13;
        if (map3[x][y] != '.')
        {
            continue;
        }
        map3[x][y] = '%';
    }
    int t7 = rand() % 3 + 2;
    for (int i = 0; i < t7; i++)
    {
        int x = rand() % 12 + 22;
        int y = rand() % 13 + 13;
        if (map3[x][y] != '.')
        {
            continue;
        }
        map3[x][y] = 'T';
    }
    int q6 = rand() % 7 + 4;
    for (int i = 0; i < q6; i++)
    {
        int x = rand() % 12 + 22;
        int y = rand() % 13 + 13;
        if (map3[x][y] != '.')
        {
            continue;
        }
        int p = rand() % 3;
        if (p == 0)
        {
            map3[x][y] = 'S';
        }
        else if (p == 1)
        {
            map3[x][y] = 'H';
        }
        else if (p == 2)
        {
            map3[x][y] = '8';
        }
    }
    map3[22][26] = '+';
    map3[30][20] = 'o';
    map3[30][20] = '<';

    // ایجاد راهروها
    draw_path(27, 22, 84, 32, map3);
    draw_path(101, 34, 160, 39, map3);
    draw_path(167, 29, 131, 20, map3);
    draw_path(124, 7, 84, 14, map3);
    draw_path(69, 13, 41, 12, map3);
}
int create_map4()
{
    memset(map4, ' ', sizeof(map4));

    //  room ۱
    for (int i = 6; i < 15; i++)
    {
        map4[i][10] = '|';
        map4[i][18] = '|';
    }
    for (int i = 10; i <= 18; i++)
    {
        map4[6][i] = '-';
        map4[14][i] = '-';
    }
    for (int i = 7; i < 14; i++)
    {
        for (int j = 11; j < 18; j++)
        {
            map4[i][j] = '.';
        }
    }
    int n0 = rand() % 3 + 3;
    for (int i = 0; i < n0; i++)
    {
        int x = rand() % 7 + 7;
        int y = rand() % 7 + 11;
        if (map4[x][y] != '.')
        {
            continue;
        }
        map4[x][y] = '%';
    }
    int t0 = rand() % 4 + 4;
    for (int i = 0; i < t0; i++)
    {
        int x = rand() % 7 + 7;
        int y = rand() % 7 + 11;
        if (map4[x][y] != '.')
        {
            continue;
        }
        map4[x][y] = 'Z';
    }
    int a1 = rand() % 5 + 6;
    for (int i = 0; i < a1; i++)
    {
        int x = rand() % 7 + 7;
        int y = rand() % 7 + 11;
        if (map4[x][y] != '.')
        {
            continue;
        }
        map4[x][y] = 'T';
    }
    map4[7][18] = '+';
    map4[10][12] = 'o';
    map4[8][16] = 'o';

    //  room ۲
    for (int i = 5; i < 14; i++)
    {
        map4[i][35] = '|';
        map4[i][48] = '|';
    }
    for (int i = 35; i <= 48; i++)
    {
        map4[5][i] = '-';
        map4[13][i] = '-';
    }
    for (int i = 6; i < 13; i++)
    {
        for (int j = 36; j < 48; j++)
        {
            map4[i][j] = '.';
        }
    }
    int n1 = rand() % 3 + 3;
    for (int i = 0; i < n1; i++)
    {
        int x = rand() % 7 + 6;
        int y = rand() % 12 + 36;
        if (map4[x][y] != '.')
        {
            continue;
        }
        map4[x][y] = '%';
    }
    int t1 = rand() % 2 + 3;
    for (int i = 0; i < t1; i++)
    {
        int x = rand() % 7 + 6;
        int y = rand() % 12 + 36;
        if (map4[x][y] != '.')
        {
            continue;
        }
        map4[x][y] = 'T';
    }
    int q2 = rand() % 3 + 2;
    for (int i = 0; i < q2; i++)
    {
        int x = rand() % 7 + 6;
        int y = rand() % 12 + 36;
        if (map4[x][y] != '.')
        {
            continue;
        }
        int p = rand() % 3;
        if (p == 0)
        {
            map4[x][y] = 'S';
        }
        else if (p == 1)
        {
            map4[x][y] = 'H';
        }
        else if (p == 2)
        {
            map4[x][y] = '8';
        }
    }
    map4[6][37] = '&';
    map4[12][35] = '@';
    map4[9][48] = '+';
    map4[11][45] = 'o';
    map4[9][37] = 'o';

    //  room ۳
    for (int i = 3; i < 13; i++)
    {
        map4[i][120] = '|';
        map4[i][135] = '|';
    }
    for (int i = 120; i <= 135; i++)
    {
        map4[3][i] = '-';
        map4[12][i] = '-';
    }
    for (int i = 4; i < 12; i++)
    {
        for (int j = 121; j < 135; j++)
        {
            map4[i][j] = '.';
        }
    }
    int n5 = rand() % 3 + 4;
    for (int i = 0; i < n5; i++)
    {
        int x = rand() % 8 + 4;
        int y = rand() % 14 + 121;
        if (map4[x][y] != '.')
        {
            continue;
        }
        map4[x][y] = '%';
    }
    int t5 = rand() % 2 + 2;
    for (int i = 0; i < t5; i++)
    {
        int x = rand() % 8 + 4;
        int y = rand() % 14 + 121;
        if (map4[x][y] != '.')
        {
            continue;
        }
        map4[x][y] = 'T';
    }
    int m5 = rand() % 2 + 1;
    for (int i = 0; i < m5; i++)
    {
        int x = rand() % 8 + 4;
        int y = rand() % 14 + 121;
        if (map4[x][y] != '.')
        {
            continue;
        }
        map4[x][y] = 'Z';
    }
    int q3 = rand() % 5 + 5;
    for (int i = 0; i < q3; i++)
    {
        int x = rand() % 8 + 4;
        int y = rand() % 14 + 121;
        if (map4[x][y] != '.')
        {
            continue;
        }
        int p = rand() % 3;
        if (p == 0)
        {
            map4[x][y] = 'S';
        }
        else if (p == 1)
        {
            map4[x][y] = 'H';
        }
        else if (p == 2)
        {
            map4[x][y] = '8';
        }
    }
    map4[12][127] = '+';
    map4[5][126] = 'o';
    map4[6][131] = 'o';
    map4[11][122] = 'o';

    //  room ۴
    for (int i = 30; i < 42; i++)
    {
        map4[i][160] = '|';
        map4[i][175] = '|';
    }
    for (int i = 160; i <= 175; i++)
    {
        map4[30][i] = '-';
        map4[41][i] = '-';
    }
    for (int i = 31; i < 41; i++)
    {
        for (int j = 161; j < 175; j++)
        {
            map4[i][j] = '.';
        }
    }
    int n2 = rand() % 3 + 2;
    for (int i = 0; i < n2; i++)
    {
        int x = rand() % 10 + 31;
        int y = rand() % 14 + 161;
        if (map4[x][y] != '.')
        {
            continue;
        }
        map4[x][y] = '%';
    }
    int t2 = rand() % 2 + 4;
    for (int i = 0; i < t2; i++)
    {
        int x = rand() % 10 + 31;
        int y = rand() % 14 + 161;
        if (map4[x][y] != '.')
        {
            continue;
        }
        map4[x][y] = 'T';
    }
    int m2 = rand() % 2;
    for (int i = 0; i < m2; i++)
    {
        int x = rand() % 10 + 31;
        int y = rand() % 14 + 161;
        if (map4[x][y] != '.')
        {
            continue;
        }
        map4[x][y] = 'X';
    }
    int q4 = rand() % 2 + 2;
    for (int i = 0; i < q4; i++)
    {
        int x = rand() % 10 + 31;
        int y = rand() % 14 + 161;
        if (map4[x][y] != '.')
        {
            continue;
        }
        int p = rand() % 3;
        if (p == 0)
        {
            map4[x][y] = 'S';
        }
        else if (p == 1)
        {
            map4[x][y] = 'H';
        }
        else if (p == 2)
        {
            map4[x][y] = '8';
        }
    }
    map4[40][174] = '&';
    map4[30][167] = '@';
    map4[39][160] = '+';
    map4[35][169] = 'o';
    map4[40][163] = 'o';

    //  room ۵
    for (int i = 30; i < 36; i++)
    {
        map4[i][94] = '|';
        map4[i][110] = '|';
    }
    for (int i = 94; i <= 110; i++)
    {
        map4[30][i] = '-';
        map4[35][i] = '-';
    }
    for (int i = 31; i < 35; i++)
    {
        for (int j = 95; j < 110; j++)
        {
            map4[i][j] = '.';
        }
    }
    int n3 = rand() % 2 + 3;
    for (int i = 0; i < n3; i++)
    {
        int x = rand() % 4 + 31;
        int y = rand() % 15 + 95;
        if (map4[x][y] != '.')
        {
            continue;
        }
        map4[x][y] = '%';
    }
    int t3 = rand() % 2;
    for (int i = 0; i < t3; i++)
    {
        int x = rand() % 4 + 31;
        int y = rand() % 15 + 95;
        if (map4[x][y] != '.')
        {
            continue;
        }
        map4[x][y] = 'T';
    }
    int m3 = rand() % 2 + 1;
    for (int i = 0; i < m3; i++)
    {
        int x = rand() % 4 + 31;
        int y = rand() % 15 + 95;
        if (map4[x][y] != '.')
        {
            continue;
        }
        map4[x][y] = 'X';
    }
    int q5 = rand() % 2 + 2;
    for (int i = 0; i < q5; i++)
    {
        int x = rand() % 4 + 31;
        int y = rand() % 15 + 95;
        if (map4[x][y] != '.')
        {
            continue;
        }
        int p = rand() % 3;
        if (p == 0)
        {
            map4[x][y] = 'S';
        }
        else if (p == 1)
        {
            map4[x][y] = 'H';
        }
        else if (p == 2)
        {
            map4[x][y] = '8';
        }
    }
    map4[32][94] = '+';
    map4[31][99] = 'o';
    map4[33][107] = 'o';

    //  room ۶
    for (int i = 15; i < 23; i++)
    {
        map4[i][50] = '|';
        map4[i][60] = '|';
    }
    for (int i = 50; i <= 60; i++)
    {
        map4[15][i] = '-';
        map4[22][i] = '-';
    }
    for (int i = 16; i < 22; i++)
    {
        for (int j = 51; j < 60; j++)
        {
            map4[i][j] = '.';
        }
    }
    int t6 = rand() % 3;
    for (int i = 0; i < t6; i++)
    {
        int x = rand() % 6 + 16;
        int y = rand() % 9 + 51;
        if (map4[x][y] != '.')
        {
            continue;
        }
        map4[x][y] = 'T';
    }
    int n6 = rand() % 3;
    for (int i = 0; i < n6; i++)
    {
        int x = rand() % 6 + 16;
        int y = rand() % 9 + 51;
        if (map4[x][y] != '.')
        {
            continue;
        }
        map4[x][y] = '%';
    }
    int q6 = rand() % 5 + 4;
    for (int i = 0; i < q6; i++)
    {
        int x = rand() % 6 + 16;
        int y = rand() % 9 + 51;
        if (map4[x][y] != '.')
        {
            continue;
        }
        int p = rand() % 3;
        if (p == 0)
        {
            map4[x][y] = 'S';
        }
        else if (p == 1)
        {
            map4[x][y] = 'H';
        }
        else if (p == 2)
        {
            map4[x][y] = '8';
        }
    }
    map4[16][52] = '&';
    map4[17][60] = '@';
    map4[21][50] = '+';
    map4[20][53] = 'o';
    map4[19][57] = 'o';

    // اتاق ۷
    for (int i = 28; i < 33; i++)
    {
        map4[i][11] = '|';
        map4[i][26] = '|';
    }
    for (int i = 11; i <= 26; i++)
    {
        map4[28][i] = '-';
        map4[32][i] = '-';
    }
    for (int i = 29; i < 32; i++)
    {
        for (int j = 12; j < 26; j++)
        {
            map4[i][j] = '.';
        }
    }
    int m7 = rand() % 2 + 1;
    for (int i = 0; i < m7; i++)
    {
        int x = rand() % 3 + 29;
        int y = rand() % 14 + 12;
        if (map4[x][y] != '.')
        {
            continue;
        }
        map4[x][y] = 'Z';
    }
    int t7 = rand() % 3;
    for (int i = 0; i < t7; i++)
    {
        int x = rand() % 3 + 29;
        int y = rand() % 14 + 12;
        if (map4[x][y] != '.')
        {
            continue;
        }
        map4[x][y] = 'T';
    }
    int n7 = rand() % 3 + 1;
    for (int i = 0; i < n7; i++)
    {
        int x = rand() % 3 + 29;
        int y = rand() % 14 + 12;
        if (map4[x][y] != '.')
        {
            continue;
        }
        map4[x][y] = '%';
    }
    int q7 = rand() % 2 + 2;
    for (int i = 0; i < q7; i++)
    {
        int x = rand() % 3 + 29;
        int y = rand() % 14 + 12;
        if (map4[x][y] != '.')
        {
            continue;
        }
        int p = rand() % 3;
        if (p == 0)
        {
            map4[x][y] = 'S';
        }
        else if (p == 1)
        {
            map4[x][y] = 'H';
        }
        else if (p == 2)
        {
            map4[x][y] = '8';
        }
    }
    map4[28][20] = '+';
    map4[32][24] = '+';
    map4[30][12] = 'o';
    map4[29][17] = 'o';

    // room 8
    for (int i = 30; i < 42; i++)
    {
        map4[i][45] = '|';
        map4[i][60] = '|';
    }
    for (int i = 45; i <= 60; i++)
    {
        map4[30][i] = '-';
        map4[41][i] = '-';
    }
    for (int i = 31; i < 41; i++)
    {
        for (int j = 46; j < 60; j++)
        {
            map4[i][j] = '.';
        }
    }
    int m8 = rand() % 2 + 1;
    for (int i = 0; i < m8; i++)
    {
        int x = rand() % 10 + 31;
        int y = rand() % 14 + 46;
        if (map4[x][y] != '.')
        {
            continue;
        }
        map4[x][y] = 'Z';
    }
    int t8 = rand() % 3 + 3;
    for (int i = 0; i < t8; i++)
    {
        int x = rand() % 10 + 31;
        int y = rand() % 14 + 46;
        if (map4[x][y] != '.')
        {
            continue;
        }
        map4[x][y] = 'T';
    }
    int n8 = rand() % 3 + 3;
    for (int i = 0; i < n8; i++)
    {
        int x = rand() % 10 + 31;
        int y = rand() % 14 + 46;
        if (map4[x][y] != '.')
        {
            continue;
        }
        map4[x][y] = '%';
    }
    int q8 = rand() % 5 + 5;
    for (int i = 0; i < q8; i++)
    {
        int x = rand() % 10 + 31;
        int y = rand() % 14 + 46;
        if (map4[x][y] != '.')
        {
            continue;
        }
        int p = rand() % 3;
        if (p == 0)
        {
            map4[x][y] = 'S';
        }
        else if (p == 1)
        {
            map4[x][y] = 'H';
        }
        else if (p == 2)
        {
            map4[x][y] = '8';
        }
    }
    map4[33][55] = 'o';
    map4[34][56] = 'o';

    // ایجاد راهروها
    draw_path(34, 12, 18, 7, map4);
    draw_path(119, 7, 48, 9, map4);
    draw_path(167, 29, 126, 13, map4);
    draw_path(111, 34, 160, 39, map4);
    draw_path(61, 17, 94, 32, map4);
    draw_path(20, 27, 50, 21, map4);
    draw_path(44, 37, 23, 33, map4);
}
int handle_input(Player *player)
{
    if (l_user.health <= 0)
    {
        clear();
        mvprintw(17, 80, "YOU LOST");
        mvprintw(18, 80, "PRESS ANY KEY TO RETURN TO MAIN MENU");
        getch();
        main_menu();
    }
    if (l_user.level_num == 1)
    {
        int ch = getch();
        if (ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '6' || ch == '7' || ch == '8' || ch == '9')
        {
            if (Hspell == 1)
            {
                Hspellc++;
            }
            if (Sspell == 1 && Sspellc <= 10)
            {
                pace = 2;
                Sspellc++;
            }
            if (Gspell == 1)
            {
                Gspellc++;
            }
            pace_counter1++;
            if (l_user.difficulty == 0)
            {
                l_user.health -= 5;
            }
            if (l_user.difficulty == 1)
            {
                l_user.health -= 10;
            }
            if (l_user.difficulty == 2)
            {
                l_user.health -= 20;
            }
        }
        int new_x = player->x, new_y = player->y;
        if (ch == 's' && pace_counter2 < 5)
        {
            pace = 2;
            pace_counter1 = 0;
            pace_counter2++;
        }
        if (pace_counter1 >= 5 && Sspellc >= 10)
        {
            pace = 1;
        }
        switch (ch)
        {
        case '1':
            new_x -= pace;
            new_y += pace;
            break;
        case '2':
            new_y += pace;
            break;
        case '3':
            new_x += pace;
            new_y += pace;
            break;
        case '4':
            new_x -= pace;
            break;
        case '6':
            new_x += pace;
            break;
        case '7':
            new_x -= pace;
            new_y -= pace;
            break;
        case '8':
            new_y -= pace;
            break;
        case '9':
            new_x += pace;
            new_y -= pace;
            break;
        case 'e':
            food_table();
            refresh_map(player, memory_map1, map1);
            break;
        case 'q':
            main_menu();
        case 'm':
            show_count++;
            if (show_count >= 4)
            {
                clear();
                attron(COLOR_PAIR(2));
                mvprintw(22, 82, "YOU LOST");
                getch();
                endwin();
                exit(0);
            }
            show_full_map_temporarily(player);
            break;
        case 'p':
            spell_table();
            refresh_map(player, memory_map1, map1);
            break;
        }

        if (is_valid_move(new_x, new_y, map1) && map1[new_y][new_x] != '<')
        {
            clear_player(player);

            player->prev_char = map1[new_y][new_x];
            player->x = new_x;
            player->y = new_y;
            if (new_x == 110 && new_y == 14)
            {
                map1[14][111] = '+';
            }
            else if (new_x == 95 && new_y == 32)
            {
                map1[32][94] = '+';
            }
            else if (map1[new_y][new_x] == '&')
            {
                char *code = generate_code();
                show_code(code);
                char *entered_code = enter_code();
                if (check_code(entered_code, code))
                {
                    map1[28][158] = '.';
                    map1[25][150] = 'G';
                }
            }

            else if (map1[new_y][new_x] == '%' || map1[new_y][new_x] == 'X')
            {
                if (message(5, 40) == '\n')
                {
                    food_manager(map1[new_y][new_x]);
                    if (full_food == 0)
                    {
                        map1[new_y][new_x] = '.';
                    }
                }
            }

            else if (map1[new_y][new_x] == 'T' || map1[new_y][new_x] == 'Z')
            {
                gold_manager(map1[new_y][new_x]);
                map1[new_y][new_x] = '.';
            }
            else if (map1[new_y][new_x] == '^')
            {
                l_user.health -= 10;
            }
            else if (map1[new_y][new_x] == 'H' || map1[new_y][new_x] == 'S' || map1[new_y][new_x] == '8')
            {
                if (message(5, 40) == '\n')
                {
                    spell_manager(map1[new_y][new_x]);
                    if (full_spell == 0)
                    {
                        map1[new_y][new_x] = '.';
                    }
                }
            }

            refresh_map(player, memory_map1, map1);
        }
        else if (map1[new_y][new_x] == '<')
        {
            start_level2();
        }
    }
    else if (l_user.level_num == 2)
    {
        int ch = getch();
        int new_x = player->x, new_y = player->y;
        if (ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '6' || ch == '7' || ch == '8' || ch == '9')
        {
            if (Hspell == 1)
            {
                Hspellc++;
            }
            if (Sspell == 1 && Sspellc <= 10)
            {
                pace = 2;
                Sspellc++;
            }
            if (Gspell == 1)
            {
                Gspellc++;
            }
            if (l_user.difficulty == 0)
            {
                l_user.health -= 5;
            }
            if (l_user.difficulty == 1)
            {
                l_user.health -= 10;
            }
            if (l_user.difficulty == 2)
            {
                l_user.health -= 20;
            }
            pace_counter1++;
        }
        if (ch == 's' && pace_counter2 < 5)
        {
            pace = 2;
            pace_counter1 = 0;
            pace_counter2++;
        }
        if (pace_counter1 >= 5 && Sspellc >= 10)
        {
            pace = 1;
        }
        switch (ch)
        {
        case '1':
            new_x -= pace;
            new_y += pace;
            break;
        case '2':
            new_y += pace;
            break;
        case '3':
            new_x += pace;
            new_y += pace;
            break;
        case '4':
            new_x -= pace;
            break;
        case '6':
            new_x += pace;
            break;
        case '7':
            new_x -= pace;
            new_y -= pace;
            break;
        case '8':
            new_y -= pace;
            break;
        case '9':
            new_x += pace;
            new_y -= pace;
            break;
        case 'e':
            food_table();
            refresh_map(player, memory_map2, map2);
            break;
        case 'q':
            main_menu();
        case 'm':
            show_count++;
            if (show_count >= 4)
            {
                clear();
                attron(COLOR_PAIR(2));
                mvprintw(22, 82, "YOU LOST");
                getch();
                endwin();
                exit(0);
            }
            show_full_map_temporarily(player);
            break;
        case 'p':
            spell_table();
            refresh_map(player, memory_map2, map2);
            break;
        }

        if (is_valid_move(new_x, new_y, map2) && map2[new_y][new_x] != '<')
        {
            clear_player(player);

            player->prev_char = map2[new_y][new_x];
            player->x = new_x;
            player->y = new_y;
            if (new_x == 17 && new_y == 7)
            {
                map2[7][18] = '+';
            }
            else if (new_x == 128 && new_y == 11)
            {
                map2[12][128] = '+';
            }
            else if (new_x == 51 && new_y == 21)
            {
                map2[21][50] = '+';
            }
            else if (map2[new_y][new_x] == '&' && get_room_id(new_x, new_y) == 2)
            {
                char *code = generate_code();
                show_code(code);
                char *entered_code = enter_code();
                if (check_code(entered_code, code))
                {
                    map2[6][36] = '.';
                    map2[9][48] = 'G';
                }
            }
            else if (map2[new_y][new_x] == '&' && get_room_id(new_x, new_y) == 5)
            {
                char *code = generate_code();
                show_code(code);
                char *entered_code = enter_code();
                if (check_code(entered_code, code))
                {
                    map2[33][108] = '.';
                    map2[32][94] = 'G';
                }
            }
            else if (map2[new_y][new_x] == '%' || map2[new_y][new_x] == 'X')
            {
                if (message(5, 40) == '\n')
                {
                    food_manager(map2[new_y][new_x]);
                    if (full_food == 0)
                    {
                        map2[new_y][new_x] = '.';
                    }
                }
            }

            else if (map2[new_y][new_x] == 'T' || map2[new_y][new_x] == 'Z')
            {
                gold_manager(map2[new_y][new_x]);
                map2[new_y][new_x] = '.';
            }
            else if (map2[new_y][new_x] == '^')
            {
                l_user.health -= 10;
            }
            else if (map2[new_y][new_x] == 'H' || map2[new_y][new_x] == 'S' || map2[new_y][new_x] == '8')
            {
                if (message(5, 40) == '\n')
                {
                    spell_manager(map2[new_y][new_x]);
                    if (full_spell == 0)
                    {
                        map2[new_y][new_x] = '.';
                    }
                }
            }
            refresh_map(player, memory_map2, map2);
        }
        else if (map2[new_y][new_x] == '<')
        {
            start_level3();
        }
    }
    else if (l_user.level_num == 3)
    {
        int ch = getch();
        int new_x = player->x, new_y = player->y;
        if (ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '6' || ch == '7' || ch == '8' || ch == '9')
        {
            if (Hspell == 1)
            {
                Hspellc++;
            }
            if (Sspell == 1 && Sspellc <= 10)
            {
                pace = 2;
                Sspellc++;
            }
            if (Gspell == 1)
            {
                Gspellc++;
            }
            pace_counter1++;
            if (l_user.difficulty == 0)
            {
                l_user.health -= 5;
            }
            if (l_user.difficulty == 1)
            {
                l_user.health -= 10;
            }
            if (l_user.difficulty == 2)
            {
                l_user.health -= 20;
            }
        }
        if (ch == 's' && pace_counter2 < 5)
        {
            pace = 2;
            pace_counter1 = 0;
            pace_counter2++;
        }
        if (pace_counter1 >= 5 && Sspellc >= 10)
        {
            pace = 1;
        }
        switch (ch)
        {
        case '1':
            new_x -= pace;
            new_y += pace;
            break;
        case '2':
            new_y += pace;
            break;
        case '3':
            new_x += pace;
            new_y += pace;
            break;
        case '4':
            new_x -= pace;
            break;
        case '6':
            new_x += pace;
            break;
        case '7':
            new_x -= pace;
            new_y -= pace;
            break;
        case '8':
            new_y -= pace;
            break;
        case '9':
            new_x += pace;
            new_y -= pace;
            break;
        case 'e':
            food_table();
            refresh_map(player, memory_map3, map3);
            break;
        case 'q':
            main_menu();
        case 'm':
            show_count++;
            if (show_count >= 4)
            {
                clear();
                attron(COLOR_PAIR(2));
                mvprintw(22, 82, "YOU LOST");
                getch();
                endwin();
                exit(0);
            }
            show_full_map_temporarily(player);
            break;
        case 'p':
            spell_table();
            refresh_map(player, memory_map3, map3);
            break;
        }

        if (is_valid_move(new_x, new_y, map3) && map3[new_y][new_x] != '<')
        {
            clear_player(player);

            player->prev_char = map3[new_y][new_x];
            player->x = new_x;
            player->y = new_y;
            if (new_x == 167 && new_y == 31)
            {
                map3[30][167] = '+';
            }
            else if (new_x == 126 && new_y == 7)
            {
                map3[7][125] = '+';
            }
            else if (map3[new_y][new_x] == '&' && get_room_id(new_x, new_y) == 5)
            {
                char *code = generate_code();
                show_code(code);
                char *entered_code = enter_code();
                if (check_code(entered_code, code))
                {
                    map3[34][87] = '.';
                    map3[34][100] = 'G';
                }
            }
            else if (map3[new_y][new_x] == '&' && get_room_id(new_x, new_y) == 2)
            {
                char *code = generate_code();
                show_code(code);
                char *entered_code = enter_code();
                if (check_code(entered_code, code))
                {
                    map3[15][72] = '.';
                    map3[13][70] = 'G';
                }
            }
            else if (map3[new_y][new_x] == '%' || map3[new_y][new_x] == 'X')
            {
                if (message(5, 40) == '\n')
                {
                    food_manager(map3[new_y][new_x]);
                    if (full_food == 0)
                    {
                        map3[new_y][new_x] = '.';
                    }
                }
            }

            else if (map3[new_y][new_x] == 'T' || map3[new_y][new_x] == 'Z')
            {
                gold_manager(map3[new_y][new_x]);
                map3[new_y][new_x] = '.';
            }
            else if (map3[new_y][new_x] == '^')
            {
                l_user.health -= 10;
            }
            else if (map3[new_y][new_x] == 'H' || map3[new_y][new_x] == 'S' || map3[new_y][new_x] == '8')
            {
                if (message(5, 40) == '\n')
                {
                    spell_manager(map3[new_y][new_x]);
                    if (full_spell == 0)
                    {
                        map3[new_y][new_x] = '.';
                    }
                }
            }
            refresh_map(player, memory_map3, map3);
        }
        else if (map3[new_y][new_x] == '<')
        {
            start_level4();
        }
    }
    else if (l_user.level_num == 4)
    {
        int ch = getch();
        int new_x = player->x, new_y = player->y;
        if (ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '6' || ch == '7' || ch == '8' || ch == '9')
        {
            pace_counter1++;
            if (Hspell == 1)
            {
                Hspellc++;
            }
            if (Sspell == 1 && Sspellc <= 10)
            {
                pace = 2;
                Sspellc++;
            }
            if (Gspell == 1)
            {
                Gspellc++;
            }
            if (l_user.difficulty == 0)
            {
                l_user.health -= 5;
            }
            if (l_user.difficulty == 1)
            {
                l_user.health -= 10;
            }
            if (l_user.difficulty == 2)
            {
                l_user.health -= 20;
            }
        }
        if (ch == 's' && pace_counter2 < 5)
        {
            pace = 2;
            pace_counter1 = 0;
            pace_counter2++;
        }
        if (pace_counter1 >= 5 && Sspellc >= 10)
        {
            pace = 1;
        }
        switch (ch)
        {
        case '1':
            new_x -= pace;
            new_y += pace;
            break;
        case '2':
            new_y += pace;
            break;
        case '3':
            new_x += pace;
            new_y += pace;
            break;
        case '4':
            new_x -= pace;
            break;
        case '6':
            new_x += pace;
            break;
        case '7':
            new_x -= pace;
            new_y -= pace;
            break;
        case '8':
            new_y -= pace;
            break;
        case '9':
            new_x += pace;
            new_y -= pace;
            break;
        case 'e':
            food_table();
            refresh_map(player, memory_map4, map4);
            break;
        case 'q':
            main_menu();
        case 'm':
            show_count++;
            if (show_count >= 4)
            {
                clear();
                attron(COLOR_PAIR(2));
                mvprintw(22, 82, "YOU LOST");
                getch();
                endwin();
                exit(0);
            }
            show_full_map_temporarily(player);
            break;
        case 'p':
            spell_table();
            refresh_map(player, memory_map4, map4);
            break;
        }

        if (is_valid_move(new_x, new_y, map4))
        {
            clear_player(player);

            player->prev_char = map4[new_y][new_x];
            player->x = new_x;
            player->y = new_y;
            if (new_x == 109 && new_y == 34)
            {
                map4[34][110] = '+';
            }
            else if (new_x == 121 && new_y == 7)
            {
                map4[7][120] = '+';
            }
            else if (new_x == 46 && new_y == 37)
            {
                map4[37][45] = '+';
            }
            else if (map4[new_y][new_x] == '&' && get_room_id(new_x, new_y) == 6)
            {
                char *code = generate_code();
                show_code(code);
                char *entered_code = enter_code();
                if (check_code(entered_code, code))
                {
                    map4[16][52] = '.';
                    map4[17][60] = 'G';
                }
            }
            else if (map4[new_y][new_x] == '&' && get_room_id(new_x, new_y) == 4)
            {
                char *code = generate_code();
                show_code(code);
                char *entered_code = enter_code();
                if (check_code(entered_code, code))
                {
                    map4[40][174] = '.';
                    map4[30][167] = 'G';
                }
            }
            else if (map4[new_y][new_x] == '&' && get_room_id(new_x, new_y) == 2)
            {
                char *code = generate_code();
                show_code(code);
                char *entered_code = enter_code();
                if (check_code(entered_code, code))
                {
                    map4[6][37] = '.';
                    map4[12][35] = 'G';
                }
            }
            else if (map4[new_y][new_x] == '%' || map4[new_y][new_x] == 'X')
            {
                if (message(5, 40) == '\n')
                {
                    food_manager(map4[new_y][new_x]);
                    if (full_food == 0)
                    {
                        map4[new_y][new_x] = '.';
                    }
                }
            }
            else if (map4[new_y][new_x] == 'T' || map4[new_y][new_x] == 'Z')
            {
                gold_manager(map4[new_y][new_x]);
                map4[new_y][new_x] = '.';
            }
            else if (map4[new_y][new_x] == '^')
            {
                l_user.health -= 10;
            }
            else if (map4[new_y][new_x] == 'H' || map4[new_y][new_x] == 'S' || map4[new_y][new_x] == '8')
            {
                if (message(5, 40) == '\n')
                {
                    spell_manager(map4[new_y][new_x]);
                    if (full_spell == 0)
                    {
                        map4[new_y][new_x] = '.';
                    }
                }
            }
            refresh_map(player, memory_map4, map4);
        }
    }
}
void draw_visible_map(int player_x, int player_y, int memory_map[MAP_HEIGHT][MAP_WIDTH], char map[MAP_HEIGHT][MAP_WIDTH])
{
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            if (memory_map[i][j])
            {
                if (map[i][j] == '@')
                {
                    attron(COLOR_PAIR(2));
                    mvaddch(i, j, '@');
                    attroff(COLOR_PAIR(2));
                }
                else if (map[i][j] == 'G')
                {
                    attron(COLOR_PAIR(3));
                    mvaddch(i, j, map[i][j]);
                    attroff(COLOR_PAIR(3));
                }
                else if (map[i][j] == '|' || map[i][j] == '-')
                {
                    attron(COLOR_PAIR(7));
                    mvaddch(i, j, map[i][j]);
                    attroff(COLOR_PAIR(7));
                }
                else
                {
                    mvaddch(i, j, map[i][j]);
                }
            }
        }
    }
}
void refresh_map(Player *player, int memory_map[MAP_HEIGHT][MAP_WIDTH], char map[MAP_HEIGHT][MAP_WIDTH])
{
    update_memory_map(player->x, player->y, memory_map);
    draw_visible_map(player->x, player->y, memory_map, map);
    draw_player(player);
    draw_bar(LINES - 2, 12, 20, l_user.health, 10000, "Health");
    draw_bar(LINES - 2, 72, 20, l_user.power, 100, "Power");
    draw_bar(LINES - 2, 132, 20, l_user.gold, 10000, "Gold");
    refresh();
}
void update_memory_map(int player_x, int player_y, int memory_map[MAP_HEIGHT][MAP_WIDTH])
{
    if (l_user.level_num == 1)
    {
        if (is_in_room(player_x, player_y))
        {
            if (get_room_id(player_x, player_y) == 1)
            {
                for (int i = 3; i <= 9; i++)
                {
                    for (int j = 3; j <= 23; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 2)
            {
                for (int i = 7; i <= 20; i++)
                {
                    for (int j = 50; j <= 65; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 3)
            {
                for (int i = 6; i <= 16; i++)
                {
                    for (int j = 100; j <= 111; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 4)
            {
                for (int i = 20; i <= 29; i++)
                {
                    for (int j = 150; j <= 159; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 5)
            {
                for (int i = 30; i <= 35; i++)
                {
                    for (int j = 94; j <= 110; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 6)
            {
                for (int i = 28; i <= 37; i++)
                {
                    for (int j = 10; j <= 20; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }
        }
        else
        {
            int view_distance = 3;

            for (int y = player_y - view_distance; y <= player_y + view_distance; y++)
            {
                for (int x = player_x - view_distance; x <= player_x + view_distance; x++)
                {
                    if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT)
                    {
                        memory_map[y][x] = 1;
                    }
                }
            }
        }
    }
    else if (l_user.level_num == 2)
    {
        if (is_in_room(player_x, player_y))
        {
            if (get_room_id(player_x, player_y) == 1)
            {
                for (int i = 6; i <= 14; i++)
                {
                    for (int j = 10; j <= 18; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 2)
            {
                for (int i = 5; i <= 13; i++)
                {
                    for (int j = 35; j <= 48; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 3)
            {
                for (int i = 3; i <= 12; i++)
                {
                    for (int j = 120; j <= 135; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 4)
            {
                for (int i = 30; i <= 41; i++)
                {
                    for (int j = 160; j <= 175; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 5)
            {
                for (int i = 30; i <= 35; i++)
                {
                    for (int j = 94; j <= 110; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 6)
            {
                for (int i = 15; i <= 22; i++)
                {
                    for (int j = 50; j <= 60; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 7)
            {
                for (int i = 28; i <= 32; i++)
                {
                    for (int j = 11; j <= 26; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }
        }
        else
        {
            int view_distance = 3;

            for (int y = player_y - view_distance; y <= player_y + view_distance; y++)
            {
                for (int x = player_x - view_distance; x <= player_x + view_distance; x++)
                {
                    if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT)
                    {
                        memory_map[y][x] = 1;
                    }
                }
            }
        }
    }
    else if (l_user.level_num == 3)
    {
        if (is_in_room(player_x, player_y))
        {
            if (get_room_id(player_x, player_y) == 1)
            {
                for (int i = 6; i <= 15; i++)
                {
                    for (int j = 35; j <= 41; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 2)
            {
                for (int i = 12; i <= 16; i++)
                {
                    for (int j = 70; j <= 84; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 3)
            {
                for (int i = 5; i <= 20; i++)
                {
                    for (int j = 125; j <= 145; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 4)
            {
                for (int i = 30; i <= 41; i++)
                {
                    for (int j = 160; j <= 175; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 5)
            {
                for (int i = 30; i <= 35; i++)
                {
                    for (int j = 84; j <= 100; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 6)
            {
                for (int i = 21; i <= 35; i++)
                {
                    for (int j = 12; j <= 26; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }
        }
        else
        {
            int view_distance = 3;

            for (int y = player_y - view_distance; y <= player_y + view_distance; y++)
            {
                for (int x = player_x - view_distance; x <= player_x + view_distance; x++)
                {
                    if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT)
                    {
                        memory_map[y][x] = 1;
                    }
                }
            }
        }
    }
    else if (l_user.level_num == 4)
    {
        if (is_in_room(player_x, player_y))
        {
            if (get_room_id(player_x, player_y) == 1)
            {
                for (int i = 6; i <= 14; i++)
                {
                    for (int j = 10; j <= 18; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 2)
            {
                for (int i = 5; i <= 13; i++)
                {
                    for (int j = 35; j <= 48; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 3)
            {
                for (int i = 3; i <= 12; i++)
                {
                    for (int j = 120; j <= 135; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 4)
            {
                for (int i = 30; i <= 41; i++)
                {
                    for (int j = 160; j <= 175; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 5)
            {
                for (int i = 30; i <= 35; i++)
                {
                    for (int j = 94; j <= 110; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 6)
            {
                for (int i = 15; i <= 22; i++)
                {
                    for (int j = 50; j <= 60; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 7)
            {
                for (int i = 28; i <= 32; i++)
                {
                    for (int j = 11; j <= 26; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }

            else if (get_room_id(player_x, player_y) == 8)
            {
                for (int i = 30; i <= 41; i++)
                {
                    for (int j = 45; j <= 60; j++)
                    {
                        memory_map[i][j] = 1;
                    }
                }
            }
        }
        else
        {
            int view_distance = 3;

            for (int y = player_y - view_distance; y <= player_y + view_distance; y++)
            {
                for (int x = player_x - view_distance; x <= player_x + view_distance; x++)
                {
                    if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT)
                    {
                        memory_map[y][x] = 1;
                    }
                }
            }
        }
    }
}
int is_in_room(int x, int y)
{
    if (l_user.level_num == 1)
    {
        if ((x >= 3 && x <= 23 && y >= 3 && y <= 9) ||
            (x >= 50 && x <= 65 && y >= 7 && y <= 20) ||
            (x >= 100 && x <= 111 && y >= 6 && y <= 16) ||
            (x >= 150 && x <= 159 && y >= 20 && y <= 29) ||
            (x >= 94 && x <= 110 && y >= 30 && y <= 35) ||
            (x >= 10 && x <= 20 && y >= 28 && y <= 37))
        {
            return 1;
        }
        return 0;
    }
    else if (l_user.level_num == 2)
    {
        if ((y >= 6 && y <= 14 && x >= 10 && x <= 18) ||
            (y >= 5 && y <= 13 && x >= 35 && x <= 48) ||
            (y >= 3 && y <= 12 && x >= 120 && x <= 135) ||
            (y >= 30 && y <= 41 && x >= 160 && x <= 175) ||
            (y >= 30 && y <= 35 && x >= 94 && x <= 110) ||
            (y >= 15 && y <= 22 && x >= 50 && x <= 60) ||
            (y >= 28 && y <= 32 && x >= 11 && x <= 26))
        {
            return 1;
        }
        return 0;
    }
    else if (l_user.level_num == 3)
    {
        if ((x >= 35 && x <= 41 && y >= 6 && y <= 15) ||
            (x >= 70 && x <= 84 && y >= 12 && y <= 16) ||
            (x >= 125 && x <= 145 && y >= 5 && y <= 19) ||
            (x >= 160 && x <= 175 && y >= 30 && y <= 41) ||
            (x >= 84 && x <= 100 && y >= 30 && y <= 35) ||
            (x >= 12 && x <= 25 && y >= 21 && y <= 35))
        {
            return 1;
        }
        return 0;
    }
    else if (l_user.level_num == 4)
    {
        if ((x >= 10 && x <= 18 && y >= 6 && y <= 14) ||
            (x >= 35 && x <= 48 && y >= 5 && y <= 13) ||
            (x >= 120 && x <= 135 && y >= 3 && y <= 12) ||
            (x >= 160 && x <= 175 && y >= 30 && y <= 41) ||
            (x >= 94 && x <= 110 && y >= 30 && y <= 35) ||
            (x >= 50 && x <= 60 && y >= 15 && y <= 22) ||
            (x >= 11 && x <= 26 && y >= 28 && y <= 32) ||
            (x >= 45 && x <= 60 && y >= 30 && y <= 41))
        {
            return 1;
        }
        return 0;
    }
}
int get_room_id(int x, int y)
{
    if (l_user.level_num == 1)
    {
        if (x >= 4 && x <= 23 && y >= 4 && y <= 10)
            return 1;
        if (x >= 51 && x <= 65 && y >= 7 && y <= 20)
            return 2;
        if (x >= 101 && x <= 111 && y >= 7 && y <= 16)
            return 3;
        if (x >= 151 && x <= 159 && y >= 21 && y <= 29)
            return 4;
        if (x >= 95 && x <= 110 && y >= 31 && y <= 35)
            return 5;
        if (x >= 11 && x <= 20 && y >= 29 && y <= 37)
            return 6;
        return 0;
    }
    else if (l_user.level_num == 2)
    {
        if (y >= 6 && y <= 14 && x >= 10 && x <= 18)
            return 1;
        if (y >= 5 && y <= 13 && x >= 35 && x <= 48)
            return 2;
        if (y >= 3 && y <= 12 && x >= 120 && x <= 135)
            return 3;
        if (y >= 30 && y <= 41 && x >= 160 && x <= 175)
            return 4;
        if (y >= 30 && y <= 35 && x >= 94 && x <= 110)
            return 5;
        if (y >= 15 && y <= 22 && x >= 50 && x <= 60)
            return 6;
        if (y >= 28 && y <= 32 && x >= 11 && x <= 26)
            return 7;
        return 0;
    }
    else if (l_user.level_num == 3)
    {
        if (x >= 35 && x <= 41 && y >= 6 && y <= 15)
            return 1;
        if (x >= 70 && x <= 84 && y >= 12 && y <= 16)
            return 2;
        if (x >= 125 && x <= 145 && y >= 6 && y <= 20)
            return 3;
        if (x >= 160 && x <= 175 && y >= 30 && y <= 41)
            return 4;
        if (x >= 84 && x <= 100 && y >= 30 && y <= 35)
            return 5;
        if (x >= 12 && x <= 26 && y >= 21 && y <= 35)
        {
            return 6;
        }
        return 0;
    }
    else if (l_user.level_num == 4)
    {
        if (x >= 10 && x <= 18 && y >= 6 && y <= 14)
            return 1;
        if (x >= 35 && x <= 48 && y >= 5 && y <= 13)
            return 2;
        if (x >= 120 && x <= 135 && y >= 3 && y <= 12)
            return 3;
        if (x >= 160 && x <= 175 && y >= 30 && y <= 41)
            return 4;
        if (x >= 94 && x <= 110 && y >= 30 && y <= 35)
            return 5;
        if (x >= 50 && x <= 60 && y >= 15 && y <= 22)
            return 6;
        if (x >= 11 && x <= 26 && y >= 28 && y <= 32)
            return 7;
        if (x >= 45 && x <= 60 && y >= 30 && y <= 41)
            return 8;
        return 0;
    }
}
void show_full_map_temporarily(Player *player)
{
    if (l_user.level_num == 1)
    {
        for (int i = 0; i < MAP_HEIGHT; i++)
        {
            for (int j = 0; j < MAP_WIDTH; j++)
            {
                mvaddch(i, j, map1[i][j]);
            }
        }
        refresh();
        timeout(4000);
        int ch = getch();
        if (ch == 'm')
        {
            clear();
            refresh_map(player, memory_map1, map1);
        }
        else
        {
            clear();
            refresh_map(player, memory_map1, map1);
        }
        timeout(-1);
    }
    else if (l_user.level_num == 2)
    {
        for (int i = 0; i < MAP_HEIGHT; i++)
        {
            for (int j = 0; j < MAP_WIDTH; j++)
            {
                mvaddch(i, j, map2[i][j]);
            }
        }
        refresh();
        timeout(4000);
        int ch = getch();
        if (ch == 'm')
        {
            clear();
            refresh_map(player, memory_map2, map2);
        }
        else
        {
            clear();
            refresh_map(player, memory_map2, map2);
        }
        timeout(-1);
    }
    else if (l_user.level_num == 3)
    {
        for (int i = 0; i < MAP_HEIGHT; i++)
        {
            for (int j = 0; j < MAP_WIDTH; j++)
            {
                mvaddch(i, j, map3[i][j]);
            }
        }
        refresh();
        timeout(4000);
        int ch = getch();
        if (ch == 'm')
        {
            clear();
            refresh_map(player, memory_map3, map3);
        }
        else
        {
            clear();
            refresh_map(player, memory_map3, map3);
        }
        timeout(-1);
    }
    else if (l_user.level_num == 4)
    {
        for (int i = 0; i < MAP_HEIGHT; i++)
        {
            for (int j = 0; j < MAP_WIDTH; j++)
            {
                mvaddch(i, j, map4[i][j]);
            }
        }
        refresh();
        timeout(4000);
        int ch = getch();
        if (ch == 'm')
        {
            clear();
            refresh_map(player, memory_map4, map4);
        }
        else
        {
            clear();
            refresh_map(player, memory_map4, map4);
        }
        timeout(-1);
    }
}
char *generate_code()
{
    static char code[5];
    const char digits[] = "0123456789";
    for (int i = 0; i < 4; i++)
    {
        code[i] = digits[rand() % 10];
    }
    code[4] = '\0';
    return code;
}
void show_code(const char *code)
{
    attron(COLOR_PAIR(2));
    mvprintw(LINES / 2 - 2, COLS / 2 - 22, "==============================");
    mvprintw(LINES / 2 + 2, COLS / 2 - 22, "==============================");
    for (int i = LINES / 2 - 2; i <= LINES / 2 + 2; i++)
    {
        mvprintw(i, COLS / 2 - 23, "||");
        mvprintw(i, COLS / 2 + 8, "||");
    }
    attroff(COLOR_PAIR(2));
    mvprintw(LINES / 2 - 1, COLS / 2 - 16, "CODE IS: %s", code);
}
char *enter_code()
{
    static char input[5];
    echo();
    curs_set(1);
    mvprintw(LINES / 2, COLS / 2 - 16, "ENTER PASS:");
    getstr(input);
    noecho();
    curs_set(0);
    return input;
}
int check_code(const char *entered_code, const char *correct_code)
{
    char real_code[5];
    for (int i = 3; i >= 0; i--)
    {
        real_code[3 - i] = entered_code[i];
    }
    if (strcmp(real_code, correct_code) == 0)
    {
        mvprintw(LINES / 2 + 1, COLS / 2 - 16, "DOOR OPENED!");
        getch();
        move(LINES / 2 - 2, COLS / 2 - 23);
        clrtoeol();
        move(LINES / 2 - 1, COLS / 2 - 23);
        clrtoeol();
        move(LINES / 2, COLS / 2 - 23);
        clrtoeol();
        move(LINES / 2 + 1, COLS / 2 - 23);
        clrtoeol();
        move(LINES / 2 + 2, COLS / 2 - 23);
        clrtoeol();
        return 1;
    }
    else
    {
        mvprintw(LINES / 2 + 1, COLS / 2 - 16, "WRONG PASS HAHA!");
        getch();
        getch();
        move(LINES / 2 - 2, COLS / 2 - 23);
        clrtoeol();
        move(LINES / 2 - 1, COLS / 2 - 23);
        clrtoeol();
        move(LINES / 2, COLS / 2 - 23);
        clrtoeol();
        move(LINES / 2 + 1, COLS / 2 - 23);
        clrtoeol();
        move(LINES / 2 + 2, COLS / 2 - 23);
        clrtoeol();
        return 0;
    }
}
void draw_bar(int y, int x, int width, int value, int max_value, const char *label)
{
    if (strcmp(label, "Gold") == 0)
    {
        mvprintw(y, x, "%s: ", label);

        int bar_length = (value * width) / max_value;

        attron(COLOR_PAIR(1) | A_REVERSE);
        for (int i = 0; i < bar_length; i++)
        {
            mvaddch(y, x + strlen(label) + 2 + i, ' ');
        }
        attroff(COLOR_PAIR(1) | A_REVERSE);

        // attron(COLOR_PAIR(1) | A_REVERSE);
        for (int i = bar_length; i < width; i++)
        {
            mvaddch(y, x + strlen(label) + 2 + i, ' ');
        }
        // attroff(COLOR_PAIR(1) | A_REVERSE);
        mvprintw(y, x + strlen(label) + 2 + width + 1, " %d/%d", value, max_value);
    }
    else
    {
        mvprintw(y, x, "%s: ", label);

        int bar_length = (value * width) / max_value;

        attron(COLOR_PAIR(3) | A_REVERSE);
        for (int i = 0; i < bar_length; i++)
        {
            mvaddch(y, x + strlen(label) + 2 + i, ' ');
        }
        attroff(COLOR_PAIR(3) | A_REVERSE);

        attron(COLOR_PAIR(2) | A_REVERSE);
        for (int i = bar_length; i < width; i++)
        {
            mvaddch(y, x + strlen(label) + 2 + i, ' ');
        }
        attroff(COLOR_PAIR(2) | A_REVERSE);
        mvprintw(y, x + strlen(label) + 2 + width + 1, " %d/%d", value, max_value);
    }
}
int food_manager(char food)
{
    if (food == '%')
    {
        if (l_user.food_bar.normal + l_user.food_bar.special >= 5)
        {
            full_food = 1;
            attron(COLOR_PAIR(2));
            mvprintw(0, 0, "not enough space to restore food!");
            attroff(COLOR_PAIR(2));
            getch();
            mvprintw(0, 0, "                                 ");
        }
        else
        {
            full_food = 0;
            l_user.food_bar.normal += 1;
        }
    }
    else if (food == 'X')
    {
        if (l_user.food_bar.normal + l_user.food_bar.special >= 5)
        {
            full_food = 1;
            attron(COLOR_PAIR(2));
            mvprintw(0, 0, "not enough space to restore food!");
            attroff(COLOR_PAIR(2));
            getch();
            mvprintw(0, 0, "                                 ");
        }
        else
        {
            full_food = 0;
            l_user.food_bar.special += 1;
        }
    }
}
void food_table()
{
    clear();
    mvprintw(LINES / 2 - 6, COLS / 2 - 20, "      normal food : %d", l_user.food_bar.normal);
    mvprintw(LINES / 2 - 5, COLS / 2 - 20, "      special food : %d", l_user.food_bar.special);
    mvprintw(LINES / 2 - 4, COLS / 2 - 50, "press n to consume normal food or press s to consume special food,to quit press any key");
    int ch = getch();
    int x = rand() % 5;
    int add1 = 10;
    int add2 = 80;
    if (Hspell == 1 && Hspellc < 10)
    {
        add1 = 20;
        add2 = 160;
    }
    switch (ch)
    {
    case 'n':
        if (x)
        {
            if (l_user.food_bar.normal > 0)
            {
                if (l_user.health >= 9990 && Hspell == 0 || l_user.health >= 9980 && Hspell == 1)
                {
                    mvprintw(LINES / 2 - 3, COLS / 2 - 20, "           FULL!");
                    getch();
                    clear();
                }
                else
                {
                    l_user.food_bar.normal--;
                    l_user.health += add1;
                    mvprintw(LINES / 2 - 3, COLS / 2 - 20, "            Yummy!");
                    getch();
                    clear();
                }
            }
            else
            {
                mvprintw(LINES / 2 - 3, COLS / 2 - 20, "You have no more normal food");
                getch();
                clear();
            }
            break;
        }
        else
        {
            l_user.health -= 6;
            mvprintw(LINES / 2 - 3, COLS / 2 - 20, "         HE HE HE HE :o");
            getch();
            clear();
        }
        break;

    case 's':

        if (l_user.food_bar.special > 0)
        {
            if (l_user.health >= 9920 && Hspell == 0 || l_user.health >= 9840 && Hspell == 1)
            {
                mvprintw(LINES / 2 - 3, COLS / 2 - 20, "           FULL!");
                getch();
                clear();
            }
            else
            {
                l_user.food_bar.special--;
                l_user.health += add2;
                mvprintw(LINES / 2 - 3, COLS / 2 - 20, "        Delicous!");
                getch();
                clear();
            }
        }
        else
        {
            mvprintw(LINES / 2 - 3, COLS / 2 - 20, "You have no more special food");
            getch();
            clear();
        }
        break;
    default:
        clear();
        break;
    }
}
int gold_manager(char gold)
{
    if (gold == 'T')
    {
        l_user.gold += 5;
    }
    else
    {
        l_user.gold += 30;
    }
}
int spell_manager(char spell)
{
    if (l_user.spell_bar.H + l_user.spell_bar.S + l_user.spell_bar.G >= 15)
    {
        full_spell = 1;
        attron(COLOR_PAIR(2));
        mvprintw(0, 0, "not enough space to restore spell!");
        attroff(COLOR_PAIR(2));
        getch();
        mvprintw(0, 0, "                                 ");
    }
    else
    {
        full_spell = 0;
        if (spell == 'H')
        {
            l_user.spell_bar.H++;
        }
        else if (spell == 'S')
        {
            l_user.spell_bar.S++;
        }
        else if (spell == '8')
        {
            l_user.spell_bar.G++;
        }
    }
}
int spell_table()
{
    clear();
    mvprintw(LINES / 2 - 4, COLS / 2 - 27, "Health spell : %d press h to consume Health spell", l_user.spell_bar.H);
    mvprintw(LINES / 2 - 3, COLS / 2 - 27, "Speed  spell : %d press s to consume Speed  spell", l_user.spell_bar.S);
    mvprintw(LINES / 2 - 2, COLS / 2 - 27, "Power  spell : %d press p to consume Power  spell", l_user.spell_bar.G);
    mvprintw(LINES / 2, COLS / 2 - 27, "            press any other key to quit");
    int ch = getch();
    switch (ch)
    {
    case 'h':
        if (l_user.spell_bar.H <= 0)
        {
            mvprintw(LINES / 2 + 2, COLS / 2 - 27, "               not enough spell!");
            getch();
            clear();
            break;
        }
        else
        {
            mvprintw(LINES / 2 + 1, COLS / 2 - 27, "                    Healthy!");
            l_user.spell_bar.H--;
            Hspell = 1;
            Hspellc = 0;
            getch();
            clear();
            break;
        }

    case 's':
        if (l_user.spell_bar.S <= 0)
        {
            mvprintw(LINES / 2 + 2, COLS / 2 - 27, "               not enough spell!");
            getch();
            clear();
            break;
        }
        else
        {
            mvprintw(LINES / 2 + 1, COLS / 2 - 27, "                    Speedy!");
            l_user.spell_bar.S--;
            Sspell = 1;
            Sspellc = 0;
            getch();
            clear();
            break;
        }

    case 'p':
        if (l_user.spell_bar.G <= 0)
        {
            mvprintw(LINES / 2 + 2, COLS / 2 - 27, "               not enough spell!");
            getch();
            clear();
            break;
        }
        else
        {
            mvprintw(LINES / 2 + 1, COLS / 2 - 27, "                   Powerful!");
            l_user.spell_bar.G--;
            Gspell = 1;
            Gspellc = 0;
            getch();
            clear();
            break;
        }
    default:
        clear();
        break;
    }
}
char message(int height, int width)
{
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    int startY = (yMax - height) / 2;
    int startX = (xMax - width) / 2;

    for (int i = 0; i < width; i++)
    {
        mvaddch(startY, startX + i, '-');
        mvaddch(startY + height - 1, startX + i, '-');
    }
    for (int i = 0; i < height; i++)
    {
        mvaddch(startY + i, startX, '|');
        mvaddch(startY + i, startX + width - 1, '|');
    }
    mvaddch(startY, startX, '+');
    mvaddch(startY, startX + width - 1, '+');
    mvaddch(startY + height - 1, startX, '+');
    mvaddch(startY + height - 1, startX + width - 1, '+');
    attron(COLOR_PAIR(4));
    mvprintw(startY, startX + (width - 10) / 2, "CHOOSE :)");
    mvprintw(startY + height / 2 - 1, startX + 2, "                   ");
    mvprintw(startY + height / 2, startX + (width - 18) / 2 - 2, "press ENTER to pick");
    mvprintw(startY + height / 2 + 1, startX + 2, "                   ");
    attroff(COLOR_PAIR(4));
    int c = getch();
    if (c == '\n')
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                mvaddch(startY + i, startX + j, ' ');
            }
        }
        return '\n';
    }
    else
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                mvaddch(startY + i, startX + j, ' ');
            }
        }
        return 'a';
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