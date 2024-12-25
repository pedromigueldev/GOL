#include "../raylib/include/raylib.h"
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include "timer.h"

#define SCREEN_W 800
#define SCREEN_H 800

typedef struct {
    Rectangle rec;
    bool alive;
    int niegh;
} Cell;

enum State {
    RUNNING,
    PAUSED
};

typedef struct {
    enum State state;
} GOL;

int main(void)
{
    InitWindow(SCREEN_W, SCREEN_H, "Game of life");

    Texture button_play;
    Texture button_pause;
    Rectangle button;

    Rectangle click_position;
    Cell GRID[100][100];
    GOL game;

    button_play = LoadTexture("res/btn_play.png");
    button_pause = LoadTexture("res/btn_pause.png");
    button = (Rectangle) {
        .height = button_play.height,
        .width = button_play.width,
        .x = 10,
        .y = 10
    };

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            GRID[i][j] = (Cell) {
                .rec = (Rectangle) {
                    .height = 8,
                    .width = 8,
                    .x = (j * 8),
                    .y = (i * 8)
                },
                .alive = false
            };
        }
    }
    click_position = (Rectangle) {
        .height = 1,
        .width = 1,
        .x = -1,
        .y = -1,
    };
    game = (GOL) {
        .state = PAUSED
    };

    SetTargetFPS(60);

    float count_down = .01f;
    Timer timer = {0};

    while (!WindowShouldClose())
    {

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 a =  GetMousePosition();
            click_position.x = a.x;
            click_position.y = a.y;

            if (CheckCollisionRecs(click_position, button)) {
                switch (game.state) {
                    case RUNNING:
                        game.state = PAUSED;
                        break;
                    case PAUSED:
                        game.state = RUNNING;
                        TimerStart(&timer, count_down);
                        break;
                }
            }

            for (int i = 0; i < 100; i++) {
                for (int j = 0; j < 100; j++) {
                    if (CheckCollisionRecs(
                        click_position,
                        GRID[i][j].rec
                        )
                    ) GRID[i][j].alive = !GRID[i][j].alive;
                }
            }
        }

        if (game.state == RUNNING && TimerDone(&timer)) {
            printf("Generation\n");
            for (int i = 0; i < 100; i++) {
                for (int j = 0; j < 100; j++) {

                    int alive = 0;

                    if (GRID[i][j].alive) alive--;
                    for (int line = i-1; line <= i+1; line++) {
                        for (int col = j-1; col <= j+1; col++) {
                            if (line < 0) line = 0;
                            if (col < 0) col = 0;
                            if (line >= 100 || col >= 100)
                                continue;
                            if (GRID[line][col].alive) alive++;
                        };
                    }

                    GRID[i][j].niegh = alive;
                }
            }
            for (int i = 0; i < 100; i++) {
                for (int j = 0; j < 100; j++) {

                    if (!GRID[i][j].alive && GRID[i][j].niegh == 3) {
                        GRID[i][j].alive = true;
                        continue;
                    }

                    if (GRID[i][j].alive) {
                        printf("Alive arround for GRID[%d][%d]: %d\n", i,j,GRID[i][j].niegh);
                        if (GRID[i][j].niegh < 2 || GRID[i][j].niegh > 3) {
                            GRID[i][j].alive = false;
                            continue;
                        }
                        if (GRID[i][j].niegh >= 2 && GRID[i][j].niegh <=3) {
                            GRID[i][j].alive = true;
                            continue;
                        }
                    }

                }
            }
            TimerStart(&timer, count_down);
        }

        TimerUpdate(&timer);
        BeginDrawing();
            ClearBackground(GRAY);

            for (int i = 0; i < 100; i++) {
                for (int j = 0; j < 100; j++) {
                    if (GRID[i][j].alive)
                        DrawRectangleRec(GRID[i][j].rec, RED);
                    else
                        DrawRectangleRec(GRID[i][j].rec, WHITE);
                }
            }

            if (game.state == RUNNING)
                DrawTextureV(button_pause, (Vector2) {10, 10}, WHITE);
            else
                DrawTextureV(button_play, (Vector2) {10, 10}, WHITE);


        EndDrawing();
    }

    CloseWindow();

    return 0;
}
