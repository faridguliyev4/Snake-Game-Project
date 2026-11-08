#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>

#define WIDTH 50
#define HEIGHT 20
#define MAX_SNAKE 1000
#define HIGH_SCORE_FILE "high_score.txt"

typedef struct {
    int x, y;
} Point;

Point snake[MAX_SNAKE];
int snake_length = 1;

Point apple;
int apple_type = 0;
char direction = 'd';
int score = 0;
int game_over = 0;

char snake_head = '>';
int speed_usleep = 200000;

// Colors for snake, walls, and fruits
#define WHITE "\033[1;37m"
#define GREEN "\033[1;32m"
#define RESET "\033[0m"
#define BLUE "\033[1;34m"
#define RED "\033[1;31m"
#define ORANGE "\033[38;5;214m"
#define PURPLE "\033[1;35m"
#define YELLOW "\033[1;33m"

// Function to get the high score from a file
int get_high_score() {
    FILE *file = fopen(HIGH_SCORE_FILE, "r");
    int high_score = 0;
    if (file) {
        fscanf(file, "%d", &high_score);
        fclose(file);
    }
    return high_score;
}

// Function to save the high score to a file
void save_high_score(int score) {
    int high_score = get_high_score();
    if (score > high_score) {
        FILE *file = fopen(HIGH_SCORE_FILE, "w");
        if (file) {
            fprintf(file, "%d", score);
            fclose(file);
        }
    }
}

// Function to play the eating sound
void play_sound(const char* file) {
    char cmd[200];
    snprintf(cmd, sizeof(cmd),
             "ffplay -nodisp -autoexit \"%s\" >/dev/null 2>&1 &", file);
    system(cmd);
}

int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(0, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &newt);
    oldf = fcntl(0, F_GETFL, 0);
    fcntl(0, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(0, TCSANOW, &oldt);
    fcntl(0, F_SETFL, oldf);
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

char get_input() {
    if (kbhit()) {
        char ch = getchar();
        if (ch == 'q') game_over = 1;
        return ch;
    }
    return 0;
}

void spawn_apple() {
    apple_type = (apple_type + 1) % 4; // Cycle through fruit types

    int valid = 0;
    while (!valid) {
        apple.x = rand() % WIDTH;
        apple.y = rand() % HEIGHT;
        valid = 1;
        for (int i = 0; i < snake_length; i++) {
            if (snake[i].x == apple.x && snake[i].y == apple.y) {
                valid = 0;
                break;
            }
        }
    }
}

void update_direction(char input) {
    if ((input == 'w' && direction != 's') ||
        (input == 'a' && direction != 'd') ||
        (input == 's' && direction != 'w') ||
        (input == 'd' && direction != 'a')) {
        direction = input;
        if (input == 'w') snake_head = '^';
        if (input == 'a') snake_head = '<';
        if (input == 's') snake_head = 'v';
        if (input == 'd') snake_head = '>';
    }
}

void move_snake() {
    Point next = snake[0];
    if (direction == 'w') next.y--;
    if (direction == 's') next.y++;
    if (direction == 'a') next.x--;
    if (direction == 'd') next.x++;

    if (next.x < 0 || next.x >= WIDTH || next.y < 0 || next.y >= HEIGHT)
        game_over = 1;

    for (int i = 0; i < snake_length; i++) {
        if (snake[i].x == next.x && snake[i].y == next.y)
            game_over = 1;
    }

    for (int i = snake_length; i > 0; i--)
        snake[i] = snake[i - 1];
    snake[0] = next;

    if (next.x == apple.x && next.y == apple.y) {
        snake_length++;
        score++;
        play_sound("eat.wav");
        spawn_apple();
        if (speed_usleep > 60000) speed_usleep -= 5000;
    }
}

void draw() {
    system("clear");
    printf("Score: %d  High Score: %d\n", score, get_high_score());
    for (int y = -1; y <= HEIGHT; y++) {
        for (int x = -1; x <= WIDTH; x++) {
            if (y == -1 || y == HEIGHT || x == -1 || x == WIDTH) {
                printf(BLUE "# " RESET);
            } else if (x == apple.x && y == apple.y) {
                // Draw fruits in the defined order
                if (apple_type == 0) printf(RED "@ " RESET);
                else if (apple_type == 1) printf(ORANGE "1 " RESET);
                else if (apple_type == 2) printf(PURPLE "%% " RESET);
                else if (apple_type == 3) printf(YELLOW "* " RESET);
            } else {
                int is_body = 0;
                for (int i = 0; i < snake_length; i++) {
                    if (snake[i].x == x && snake[i].y == y) {
                        if (i == 0) {
                            printf(WHITE "%c" RESET, snake_head);
                        } else if (i % 2 == 1) {
                            printf(GREEN "O" RESET);
                        } else {
                            printf(WHITE "O" RESET);
                        }
                        is_body = 1;
                        break;
                    }
                }
                if (!is_body) printf("   ");
            }
        }
        printf("\n");
    }
}

void start_screen() {
    system("clear");
    printf("\n");
    printf("SSSSSS\n");
    printf(".    SSSSSS\n");
    printf(".         SSSSSS\n");
    printf("\nPress Enter to start...\n");
    getchar();
}

void end_game() {
    system("clear");
    save_high_score(score);
    play_sound("die.mp3");
    printf("\nGame Over!\n");
    printf("Score: %d\n", score);
    printf("Maybe you couldn't make it to 20,000...\n");
    printf("But you gained the right to give these little cute kids 20 points ❤️\n\n");
    printf("    -     -     \n");
    printf("  -   - -   -   \n");
    printf("  -         -   \n");
    printf("   -       -    \n");
    printf("     -   -      \n");
    printf("       -        \n\n");
}

int main() {
    srand(time(NULL));
    start_screen();
    spawn_apple();

    snake[0].x = WIDTH / 2;
    snake[0].y = HEIGHT / 2;

    while (!game_over) {
        char input = get_input();
        if (input) update_direction(input);
        move_snake();
        draw();
        usleep(speed_usleep);
    }

    end_game();
    return 0;
}

