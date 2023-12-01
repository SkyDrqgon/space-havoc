#define RAYGUI_IMPLEMENTATION

#include "raylib.h"
#include "raygui.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include <vector>

typedef enum GameScreen
{
    TITLE = 0,
    GAME
} GameScreen;

void initializeGame();
void initializeCamera(Camera * camera);
void handleInput(Player * player);
void handleCamera(Camera * camera, Player * player);
void moveBullets();
void drawBullets();

std::vector<Bullet> bullets;

int lives = 3;
int score = 0;

int main()
{
    const int width = 1280;
    const int height = 720;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, "Space Havoc");
    // SetTargetFPS(60);

    GameScreen currentScreen = TITLE;
    bool isQuittingGame = false;

    Player player;
    Enemy enemy;

    Camera camera;
    initializeCamera(&camera);

    while (!WindowShouldClose())
    {        
        // Update
        switch (currentScreen)
        {
            case TITLE:
            {
                DrawText("SPACE HAVOC", GetScreenWidth() / 2 - MeasureText("SPACE HAVOC", 40) / 2, GetScreenHeight() / 4, 40, LIGHTGRAY);

                float buttonX = GetScreenWidth() / 2.0 - 200.0 / 2.0;
                float playButtonY = GetScreenHeight() / 2.0 - 70.0 / 2.0;
                float quitButtonY = playButtonY + 70.0 + 20.0;

                GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

                if (GuiButton((Rectangle){buttonX, playButtonY, 200, 70}, "PLAY"))
                {
                    DisableCursor();
                    initializeGame();
                    currentScreen = GAME;
                }

                if (GuiButton((Rectangle){buttonX, quitButtonY, 200,70}, "QUIT"))
                {
                    isQuittingGame = true;
                    break;
                }
        } break;


            case GAME:
            {
                // Update
                handleInput(&player);
                handleCamera(&camera, &player);

                enemy.move();
                moveBullets();
            } break;
        }

        if (isQuittingGame)
        {
            break;
        }

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);
        DrawFPS(10, 10);

        switch (currentScreen)
        {
            case TITLE:
            {
            } break;

            case GAME:
            {
                DrawText(TextFormat("Lives: %d", lives), 10, 35, 20, WHITE);
                DrawText(TextFormat("Score: %d", score), 10, 60, 20, WHITE);

                BeginMode3D(camera);

                DrawModel(player.getModel(), player.getPosition(), 1.0f, WHITE);
                DrawBoundingBox(player.getBoundingBox(), LIME);

                DrawModel(enemy.getModel(), enemy.getPosition(), enemy.getScale(), WHITE);
                DrawBoundingBox(enemy.getBoundingBox(), LIME);

                DrawGrid(10, 10.0f);

                drawBullets();

                EndMode3D();
            } break;
        }

        EndDrawing();
    }

    player.destroy();
    enemy.destroy();
    CloseWindow();
    return 0;
}

void initializeGame() {
    lives = 3;
    score = 0;
}

void initializeCamera(Camera * camera) {
    *camera = {0};
    camera->position = (Vector3){0.0f, 0.0f, 0.0f};
    camera->target = (Vector3){0.0f, 0.0f, 0.0f};
    camera->up = (Vector3){0.0f, 1.0f, 0.0f};
    camera->fovy = 30.0f;
    camera->projection = CAMERA_PERSPECTIVE;
}

void handleInput(Player * player) {
    if (IsKeyDown(KEY_LEFT))
    {
        player->moveLeft();
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        player->moveRight();
    }

    if (IsKeyPressed(KEY_SPACE))
    {
        bullets.push_back(Bullet(player->getPosition()));
    }
}

void handleCamera(Camera * camera, Player * player) {
    // UpdateCamera(camera, CAMERA_THIRD_PERSON);

    // Make the camera follow the player when they move
    camera->target = (Vector3){player->getPosition().x + 1.7f, 0.0f, 0.0f};
    camera->position = (Vector3){(player->getPosition().x + 1.7f), 4.0f, player->getPosition().z - 20.0f};
}

void moveBullets() {
    for (int i = 0; i < bullets.size(); i++) {
        bullets[i].move();
    }
}

void drawBullets() {
    for (int i = 0; i < bullets.size(); i++) {
        bullets[i].draw();
    }
}