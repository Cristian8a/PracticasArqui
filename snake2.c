#include "ripes_system.h"
#include <stdlib.h>
#include <stdio.h>

#define SW0 (0x01)

#define LED_COLOR_RED 0xFF0000
#define LED_COLOR_GREEN 0x00FF00
#define LED_COLOR_OFF 0x000000

#define LED_MATRIX_WIDTH 16  // Define as per LED Matrix 0 specs
#define LED_MATRIX_HEIGHT 16
#define INITIAL_SNAKE_SIZE 4
#define SNAKE_INCREMENT 8    // Increase by 2 rows (8 LEDs)

volatile unsigned int * led_base = LED_MATRIX_0_BASE;
volatile unsigned int * d_pad_up = D_PAD_0_UP;
volatile unsigned int * d_pad_do = D_PAD_0_DOWN;
volatile unsigned int * d_pad_le = D_PAD_0_LEFT;
volatile unsigned int * d_pad_ri = D_PAD_0_RIGHT;
volatile unsigned int * switch_base = SWITCHES_0_BASE;

typedef struct {
    int x, y;
} Point;

Point snake[LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT]; // Array to hold snake body positions
int snake_size = INITIAL_SNAKE_SIZE;
Point apple;

void initialize_game() {
    snake_size = INITIAL_SNAKE_SIZE;

    // Initialize snake position (starting at center)
    for (int i = 0; i < snake_size; i++) {
        snake[i].x = LED_MATRIX_WIDTH / 2;
        snake[i].y = (LED_MATRIX_HEIGHT / 2) - i;
        *(led_base + snake[i].y * LED_MATRIX_WIDTH + snake[i].x) = LED_COLOR_RED;
    }

    // Generate first apple
    generate_apple();
}

void clear_matrix() {
    for (int i = 0; i < LED_MATRIX_HEIGHT; i++) {
        for (int j = 0; j < LED_MATRIX_WIDTH; j++) {
            *(led_base + i * LED_MATRIX_WIDTH + j) = LED_COLOR_OFF;
        }
    }
}

void generate_apple() {
    apple.x = rand() % LED_MATRIX_WIDTH;
    apple.y = rand() % LED_MATRIX_HEIGHT;
    *(led_base + apple.y * LED_MATRIX_WIDTH + apple.x) = LED_COLOR_GREEN;
}

int check_collision(Point p) {
    // Check wall collision
    if (p.x < 0 || p.x >= LED_MATRIX_WIDTH || p.y < 0 || p.y >= LED_MATRIX_HEIGHT) return 1;

    // Check self collision
    for (int i = 0; i < snake_size; i++) {
        if (snake[i].x == p.x && snake[i].y == p.y) return 1;
    }
    return 0;
}

void move_snake(int dx, int dy) {
    Point new_head = {snake[0].x + dx, snake[0].y + dy};

    if (check_collision(new_head)) {
        initialize_game(); // Restart game on collision
        return;
    }

    // Check if apple is eaten
    int ate_apple = (new_head.x == apple.x && new_head.y == apple.y);

    // Shift snake body
    for (int i = snake_size - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
        *(led_base + snake[i].y * LED_MATRIX_WIDTH + snake[i].x) = LED_COLOR_RED;
    }

    // Update head position
    snake[0] = new_head;
    *(led_base + snake[0].y * LED_MATRIX_WIDTH + snake[0].x) = LED_COLOR_RED;

    // If apple eaten, increase snake size
    if (ate_apple) {
        snake_size += SNAKE_INCREMENT;
        generate_apple();
    }
}

void main() {
    int dx = 0, dy = -1; // Start moving upwards
    int a = 0;
    srand(a);

    initialize_game();

    while (1) {
        // Read D-Pad inputs to determine direction
        if (*d_pad_up == 1) { dx = 0; dy = -1; }
        else if (*d_pad_do == 1) { dx = 0; dy = 1; }
        else if (*d_pad_le == 1) { dx = -1; dy = 0; }
        else if (*d_pad_ri == 1) { dx = 1; dy = 0; }

        move_snake(dx, dy);

        // Check for game reset
        if (*switch_base & SW0) {
            clear_matrix();
            initialize_game();
        }

        // Simple delay loop
        for (int i = 0; i < 100000; i++);
    }
}
