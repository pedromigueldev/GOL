#include "../raylib/include/raylib.h"
#include <stdbool.h>
#include <sys/types.h>
#include "timer.h"

#define SCREEN_W 800
#define SCREEN_H 800

typedef struct {
    Rectangle rec;
    int neighbours;
    bool alive;
} Cell;

enum State {
    RUNNING,
    PAUSED
};

typedef struct {
    enum State state;
} GOL;

Texture button_play;
Texture button_pause;
Texture button_reset;

Cell GRID[100][100];
GOL game;

void InitGOL()
{
    game = (GOL) {
        .state = PAUSED
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
}

int main(void)
{
    InitWindow(SCREEN_W, SCREEN_H, "Game of life");
    InitGOL();

    Rectangle button_pp;
    Rectangle button_reset_rec;
    Rectangle click_position;
    int generation = 0;

    button_play = LoadTexture("res/btn_play.png");
    button_pause = LoadTexture("res/btn_pause.png");
    button_reset = LoadTexture("res/reset.png");

    button_reset_rec = (Rectangle) {
        .height = button_reset.height,
        .width = button_reset.width,
        .x = button_reset.width + 10 + 10,
        .y = 10
    };
    button_pp = (Rectangle) {
        .height = button_play.height,
        .width = button_play.width,
        .x = 10,
        .y = 10
    };

    click_position = (Rectangle) {
        .height = 1,
        .width = 1,
        .x = -1,
        .y = -1,
    };

    SetTargetFPS(24);

    float count_down = .03f;
    Timer timer = {0};

    while (!WindowShouldClose())
    {

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 a =  GetMousePosition();
            click_position.x = a.x;
            click_position.y = a.y;

            if (CheckCollisionRecs(click_position, button_pp)) {
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

            if (CheckCollisionRecs(click_position, button_reset_rec)) {
                game.state = PAUSED;

                for (int i = 0; i < 100; i++) {
                    for (int j = 0; j < 100; j++) {
                        GRID[i][j].alive = false;
                    }
                }

            }

            //this is O(log n)
            int x = (click_position.x - ((int)click_position.x) % 8) / 8;
            int y = (click_position.y - ((int)click_position.y) % 8) / 8;

            if (CheckCollisionRecs(
                click_position,
                GRID[y][x].rec
                )
            ) GRID[y][x].alive = !GRID[y][x].alive;
        }

        if (game.state == RUNNING && TimerDone(&timer)) {
            generation++;
            //count neighbours for each cell
            for (int i = 0; i < 100; i++) {
                for (int j = 0; j < 100; j++) {
                    register int alive = 0;
                    if (GRID[i][j].alive) alive--;
                    for (int line = i-1; line <= i+1; line++) {
                        for (int col = j-1; col <= j+1; col++) {
                            if (line < 0) line = 0;
                            if (col < 0) col = 0;
                            if (line >= 100 || col >= 100) continue;
                            if (GRID[line][col].alive) alive++;
                        };
                    }
                    GRID[i][j].neighbours = alive;
                }
            }

            //compute rules for each cell
            for (int i = 0; i < 100; i++) {
                for (int j = 0; j < 100; j++) {
                    if (!GRID[i][j].alive && GRID[i][j].neighbours == 3) {
                        GRID[i][j].alive = true;
                        continue;
                    }

                    if (GRID[i][j].alive) {
                        if (GRID[i][j].neighbours < 2 || GRID[i][j].neighbours > 3) {
                            GRID[i][j].alive = false;
                            continue;
                        }
                        if (GRID[i][j].neighbours >= 2 && GRID[i][j].neighbours <=3) {
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
            for (int i = 0; i < 100; i++) {
                for (int j = 0; j < 100; j++) {
                    Cell* this_cell = &GRID[i][j];

                    if (this_cell->alive)
                        DrawRectangleRec(this_cell->rec, RED);
                    else
                        DrawRectangleRec(this_cell->rec, WHITE);
                }
            }

            if (game.state == RUNNING)
                DrawTextureV(button_pause, (Vector2) {10, 10}, WHITE);
            else
                DrawTextureV(button_play, (Vector2) {10, 10}, WHITE);
            DrawTextureV(button_reset, (Vector2) {button_reset_rec.x, button_reset_rec.y}, WHITE);



        EndDrawing();
    }

    CloseWindow();

    return 0;
}
