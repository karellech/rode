#include <raylib.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define CELL_SIZE 20
#define MAX_SNAKE_LENGTH 1000
#define FPS 10

typedef struct {
    int x, y;
} Vector2i;

typedef enum {
    UP, DOWN, LEFT, RIGHT
} Direction;

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake Game - Raylib");
    SetTargetFPS(FPS);

    Vector2i snake[MAX_SNAKE_LENGTH];
    int snakeLength = 5;
    Direction dir = RIGHT;

    // Initialiser serpent
    for (int i = 0; i < snakeLength; i++) {
        snake[i].x = 10 - i;
        snake[i].y = 10;
    }

    Vector2i fruit = { GetRandomValue(0, SCREEN_WIDTH / CELL_SIZE - 1), GetRandomValue(0, SCREEN_HEIGHT / CELL_SIZE - 1) };
    int score = 0;
    bool gameOver = false;
    bool paused = false;  // <-- Variable pause

    while (!WindowShouldClose()) {
        // === INPUT ===

        if (IsKeyPressed(KEY_P)) {  // Touche P pour pause / reprise
            paused = !paused;
        }

        if (!paused && !gameOver) {  // Les mouvements ne se font que si pas en pause et pas game over
            if (IsKeyPressed(KEY_UP) && dir != DOWN) dir = UP;
            if (IsKeyPressed(KEY_DOWN) && dir != UP) dir = DOWN;
            if (IsKeyPressed(KEY_LEFT) && dir != RIGHT) dir = LEFT;
            if (IsKeyPressed(KEY_RIGHT) && dir != LEFT) dir = RIGHT;

            // === MOVE SNAKE ===
            for (int i = snakeLength - 1; i > 0; i--) {
                snake[i] = snake[i - 1];
            }

            switch (dir) {
                case UP: snake[0].y -= 1; break;
                case DOWN: snake[0].y += 1; break;
                case LEFT: snake[0].x -= 1; break;
                case RIGHT: snake[0].x += 1; break;
            }

            // === COLLISION WALL ===
            if (snake[0].x < 0 || snake[0].x >= SCREEN_WIDTH / CELL_SIZE ||
                snake[0].y < 0 || snake[0].y >= SCREEN_HEIGHT / CELL_SIZE) {
                gameOver = true;
            }

            // === COLLISION SELF ===
            for (int i = 1; i < snakeLength; i++) {
                if (snake[i].x == snake[0].x && snake[i].y == snake[0].y) {
                    gameOver = true;
                }
            }

            // === EAT FRUIT ===
            if (snake[0].x == fruit.x && snake[0].y == fruit.y) {
                if (snakeLength < MAX_SNAKE_LENGTH) snakeLength++;
                fruit.x = GetRandomValue(0, SCREEN_WIDTH / CELL_SIZE - 1);
                fruit.y = GetRandomValue(0, SCREEN_HEIGHT / CELL_SIZE - 1);
                score += 10;
            }
        }

        // === DRAWING ===
        BeginDrawing();
        ClearBackground(BLACK);

        // Dessiner fruit
        DrawRectangle(fruit.x * CELL_SIZE, fruit.y * CELL_SIZE, CELL_SIZE, CELL_SIZE, RED);

        // Choisir la couleur de la grenouille selon le score
        Color frogColor = (score >= 100) ? ORANGE : GREEN;

        // Dessiner grenouille (serpent) - on peut dessiner un rond pour la tête pour ressembler à une grenouille
        for (int i = 0; i < snakeLength; i++) {
            int px = snake[i].x * CELL_SIZE;
            int py = snake[i].y * CELL_SIZE;
            if (i == 0) {
                // Tête : un cercle plus grand (grenouille)
                DrawCircle(px + CELL_SIZE/2, py + CELL_SIZE/2, CELL_SIZE/2, frogColor);
                // Dessiner deux petits cercles blancs pour les yeux (optionnel)
                DrawCircle(px + CELL_SIZE/3, py + CELL_SIZE/3, CELL_SIZE/8, RAYWHITE);
                DrawCircle(px + 2*CELL_SIZE/3, py + CELL_SIZE/3, CELL_SIZE/8, RAYWHITE);
            } else {
                // Corps : des rectangles verts/orange
                DrawRectangle(px, py, CELL_SIZE, CELL_SIZE, frogColor);
            }
        }

        // Score
        DrawText(TextFormat("Score: %d", score), 10, 10, 20, WHITE);

        if (paused) {
            DrawText("PAUSE", SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 - 20, 40, YELLOW);
        }

        if (gameOver) {
            DrawText("GAME OVER", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, 40, RAYWHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}


