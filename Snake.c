#include "ripes_system.h"

#define SW0 (0x01)

#define LED_COLOR_RED 0xFF0000
#define LED_COLOR_GREEN 0x00FF00
#define LED_COLOR_OFF 0x000000

#define LED_MATRIX_WIDTH 16  // Asumimos que la matriz es de 16x16
#define LED_MATRIX_HEIGHT 16
#define INITIAL_SNAKE_SIZE 4
#define SNAKE_INCREMENT 8    // Incremento de 8 LEDs cuando come una manzana

volatile unsigned int * led_base = LED_MATRIX_0_BASE;
volatile unsigned int * d_pad_up = D_PAD_0_UP;
volatile unsigned int * d_pad_do = D_PAD_0_DOWN;
volatile unsigned int * d_pad_le = D_PAD_0_LEFT;
volatile unsigned int * d_pad_ri = D_PAD_0_RIGHT;
volatile unsigned int * switch_base = SWITCHES_0_BASE;

int main() {
    int snake_size = INITIAL_SNAKE_SIZE;
    int dx = 0, dy = -1;  // Dirección inicial hacia arriba
    int a = 0;
    srand(a);

    // Coordenadas iniciales de la serpiente en el centro de la matriz
    int snake_x[LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT];
    int snake_y[LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT];
    int center_x = LED_MATRIX_WIDTH / 2;
    int center_y = LED_MATRIX_HEIGHT / 2;

    for (int i = 0; i < snake_size; i++) {
        snake_x[i] = center_x;
        snake_y[i] = center_y - i;
        *(led_base + snake_y[i] * LED_MATRIX_WIDTH + snake_x[i]) = LED_COLOR_RED;
    }

    // Posición inicial de la manzana
    int apple_x = rand() % LED_MATRIX_WIDTH;
    int apple_y = rand() % LED_MATRIX_HEIGHT;
    *(led_base + apple_y * LED_MATRIX_WIDTH + apple_x) = LED_COLOR_GREEN;

    while (1) {
        // Leer el D-Pad para cambiar dirección
        if (*d_pad_up == 1) { dx = 0; dy = -1; }
        else if (*d_pad_do == 1) { dx = 0; dy = 1; }
        else if (*d_pad_le == 1) { dx = -1; dy = 0; }
        else if (*d_pad_ri == 1) { dx = 1; dy = 0; }

        // Calcular la nueva posición de la cabeza de la serpiente
        int new_head_x = snake_x[0] + dx;
        int new_head_y = snake_y[0] + dy;

        // Comprobar colisiones (pared o cuerpo)
        if (new_head_x < 0 || new_head_x >= LED_MATRIX_WIDTH || new_head_y < 0 || new_head_y >= LED_MATRIX_HEIGHT) {
            // Reiniciar el juego en caso de colisión
            snake_size = INITIAL_SNAKE_SIZE;
            for (int i = 0; i < snake_size; i++) {
                snake_x[i] = center_x;
                snake_y[i] = center_y - i;
                *(led_base + snake_y[i] * LED_MATRIX_WIDTH + snake_x[i]) = LED_COLOR_RED;
            }
            continue;
        }
        for (int i = 0; i < snake_size; i++) {
            if (snake_x[i] == new_head_x && snake_y[i] == new_head_y) {
                // Reiniciar el juego en caso de colisión con el propio cuerpo
                snake_size = INITIAL_SNAKE_SIZE;
                for (int i = 0; i < snake_size; i++) {
                    snake_x[i] = center_x;
                    snake_y[i] = center_y - i;
                    *(led_base + snake_y[i] * LED_MATRIX_WIDTH + snake_x[i]) = LED_COLOR_RED;
                }
                continue;
            }
        }

        // Mover el cuerpo de la serpiente
        for (int i = snake_size - 1; i > 0; i--) {
            snake_x[i] = snake_x[i - 1];
            snake_y[i] = snake_y[i - 1];
            *(led_base + snake_y[i] * LED_MATRIX_WIDTH + snake_x[i]) = LED_COLOR_RED;
        }

        // Actualizar la posición de la cabeza de la serpiente
        snake_x[0] = new_head_x;
        snake_y[0] = new_head_y;
        *(led_base + snake_y[0] * LED_MATRIX_WIDTH + snake_x[0]) = LED_COLOR_RED;

        // Comprobar si la serpiente ha comido la manzana
        if (new_head_x == apple_x && new_head_y == apple_y) {
            // Incrementar el tamaño de la serpiente
            snake_size += SNAKE_INCREMENT;

            // Generar una nueva posición para la manzana
            do {
                apple_x = rand() % LED_MATRIX_WIDTH;
                apple_y = rand() % LED_MATRIX_HEIGHT;
            } while (*(led_base + apple_y * LED_MATRIX_WIDTH + apple_x) == LED_COLOR_RED);

            *(led_base + apple_y * LED_MATRIX_WIDTH + apple_x) = LED_COLOR_GREEN;
        }

        // Comprobar si se debe reiniciar el juego con el switch 0
        if (*switch_base & SW0) {
            for (int i = 0; i < LED_MATRIX_HEIGHT; i++) {
                for (int j = 0; j < LED_MATRIX_WIDTH; j++) {
                    *(led_base + i * LED_MATRIX_WIDTH + j) = LED_COLOR_OFF;
                }
            }
            snake_size = INITIAL_SNAKE_SIZE;
            for (int i = 0; i < snake_size; i++) {
                snake_x[i] = center_x;
                snake_y[i] = center_y - i;
                *(led_base + snake_y[i] * LED_MATRIX_WIDTH + snake_x[i]) = LED_COLOR_RED;
            }
            apple_x = rand() % LED_MATRIX_WIDTH;
            apple_y = rand() % LED_MATRIX_HEIGHT;
            *(led_base + apple_y * LED_MATRIX_WIDTH + apple_x) = LED_COLOR_GREEN;
        }

        // Simple delay para ralentizar el movimiento
        for (int i = 0; i < 100000; i++);
    }

    return 0;
}
