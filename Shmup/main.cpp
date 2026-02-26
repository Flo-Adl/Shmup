#include "raylib.h"
#include "Ship.h"
#include "Bullet.h"
#include "enemy.h"
#include <vector>

int main()
{
    InitWindow(600, 800, "Space Prototype");
    SetTargetFPS(60);
    Enemy::LoadSharedTexture();
    Texture2D background = LoadTexture("C:/Users/adell/Desktop/Programmation C++/TP1/Shmup/bg.jpg");

    Ship player(300, 600, 300.0f);
    std::vector<bullet> bullets;

    // ---- ENEMIES ----
    std::vector<Enemy> enemies;

    float spawnTimer = 0.0f;
    float spawnInterval = 1.0f; // 1 ennemi / seconde

    int score = 0;

    // --- MENU ---
    bool inMenu = true;
    Rectangle playBtn = { 600 / 2.0f - 120, 800 / 2.0f - 30, 240, 60 };

    // --- AMMO ---
    int maxAmmo = 21;
    int currentAmmo = maxAmmo;

    float reloadTime = 1.2;
    float reloadTimer = 0.0f;
    bool isReloading = false;

    int tripleShots = 0;
    int nextTripleAt = 15;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        // ---------------- MENU ----------------
        if (inMenu)
        {
            Rectangle quit = { 600 / 2.0f - 120, 1000 / 2.0f - 30, 240, 60 };
            Vector2 mouse = GetMousePosition();
            bool hover = CheckCollisionPointRec(mouse, playBtn);
            bool hover2 = CheckCollisionPointRec(mouse, quit);

            if (hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                // Reset simple au lancement
                bullets.clear();
                enemies.clear();
                score = 0;
                spawnTimer = 0.0f;

                inMenu = false;
            }

            if (hover2 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                CloseWindow();
            }

            BeginDrawing();
            ClearBackground(BLACK);

            DrawText("SHMUP", 215, 150, 50, RAYWHITE);
            DrawText("- C++ Game Project -", 125, 225, 35, RAYWHITE);

            DrawRectangleRec(playBtn, hover ? GRAY : DARKGRAY);
            DrawRectangleLinesEx(playBtn, 2, RAYWHITE);
            DrawText("PLAY", (int)playBtn.x + 80, (int)playBtn.y + 15, 30, RAYWHITE);

            DrawRectangleRec(quit, hover2 ? GRAY : DARKGRAY);
            DrawRectangleLinesEx(quit, 2, RAYWHITE);
            DrawText("QUIT", (int)quit.x + 85, (int)quit.y + 15, 30, RAYWHITE);


            EndDrawing();
            continue;
        }
        if (score < 15) {
            spawnInterval = 1.0f;
        }
        if (score > 15) {
            spawnInterval = 0.85f;
        }
        if (score > 30) {
            spawnInterval = 0.65f;
        }
        if (score > 45) {
            spawnInterval = 0.45f;
        }

        // ---------------- UPDATE ----------------
        player.Update(dt);
        
        // --- RELOAD ---
        if (isReloading)
        {
            reloadTimer -= dt;

            if (reloadTimer <= 0.0f)
            {
                isReloading = false;
                currentAmmo = maxAmmo;
            }
        }

        if (score >= nextTripleAt)
        {
            tripleShots += 3;
            nextTripleAt += 15;
        }

        // --- TIR ---
        if (!isReloading && IsKeyPressed(KEY_SPACE))
        {
            if (currentAmmo > 0)
            {
                Vector2 pos = player.GetPosition();
                float speed = 400.0f;

                bool wantTriple = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT));

                // --- SUPER ---
                if (wantTriple && tripleShots > 0)
                {
                    float a = 30.0f * DEG2RAD;

                    Vector2 v0 = { 0.0f, -speed };
                    Vector2 vL = { -speed * sinf(a), -speed * cosf(a) };
                    Vector2 vR = { speed * sinf(a), -speed * cosf(a) };

                    bullets.emplace_back(pos.x, pos.y - 20, v0, 6.0f, DARKPURPLE);
                    bullets.emplace_back(pos.x, pos.y - 20, vL, 6.0f, DARKPURPLE);
                    bullets.emplace_back(pos.x, pos.y - 20, vR, 6.0f, DARKPURPLE);

                    tripleShots--;
                    currentAmmo--;
                }
                else
                {
                    bullets.emplace_back(pos.x, pos.y - 20, Vector2{ 0.0f, -speed }, 6.0f, DARKPURPLE);
                    currentAmmo--;
                }

                // Si plus de munitions : reload
                if (currentAmmo == 0)
                {
                    isReloading = true;
                    reloadTimer = reloadTime;
                }
            }
        }

        // Update bullets + suppression
        for (int i = 0; i < (int)bullets.size(); i++)
        {
            bullets[i].Update(dt);
            if (bullets[i].IsOffScreen())
            {
                bullets.erase(bullets.begin() + i);
                i--;
            }
        }

        // Spawn enemies
        spawnTimer += dt;
        if (spawnTimer >= spawnInterval)
        {
            spawnTimer = 0.0f;

            float x = (float)GetRandomValue(20, GetScreenWidth() - 20);
            float y = -20.0f;
            enemies.emplace_back(x, y, 120.0f, 0.25f);
        }

        // Update enemies + suppression
        for (int i = 0; i < (int)enemies.size(); i++)
        {
            enemies[i].Update(dt);

            if (enemies[i].IsOffScreen())
            {
                enemies.erase(enemies.begin() + i);
                i--;

                player.TakeDamage();   
                continue;
            }
        }

        // Collision Bullet vs Enemy
        for (int i = 0; i < (int)enemies.size(); i++)
        {
            bool enemyKilled = false;

            for (int j = 0; j < (int)bullets.size(); j++)
            {
                // Enemy = cercle, Bullet = rectangle
                bool hit = CheckCollisionCircleRec(
                    enemies[i].GetPosition(),
                    enemies[i].GetRadius(),
                    bullets[j].GetRect()
                );

                if (hit)
                {
                    // Supprimer la balle
                    bullets.erase(bullets.begin() + j);
                    j--;

                    // Supprimer l'ennemi
                    enemies.erase(enemies.begin() + i);
                    i--;

                    score += 1;
                    enemyKilled = true;
                    break;
                }
            }
            if (enemyKilled)
                continue;
        }

        // Collision Enemy vs Ship
        for (int i = 0; i < (int)enemies.size(); i++)
        {
            bool hitPlayer = CheckCollisionCircles(
                enemies[i].GetPosition(),
                enemies[i].GetRadius(),
                player.GetPosition(),
                13.0f // rayon du ship
            );

            if (hitPlayer)
            {
                enemies.erase(enemies.begin() + i);
                i--;

                player.TakeDamage();
            }
        }

        if (player.GetLives() <= 0)
        {   
            Rectangle quit = { 600 / 2.0f - 120, 1000 / 2.0f - 30, 240, 60 };
            Vector2 mouse = GetMousePosition();
            bool hover = CheckCollisionPointRec(mouse, playBtn);
            bool hover2 = CheckCollisionPointRec(mouse, quit);

            if (hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                bullets.clear();
                enemies.clear();
                score = 0;
                currentAmmo = maxAmmo;
                spawnTimer = 0.0f;
                tripleShots = 0;
                nextTripleAt = 15;
               player.Reset(300,600);

            }

            if (hover2 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                CloseWindow();
            }

            Rectangle playBtn2 = { 600 / 2.0f - 120, 800 / 2.0f - 30, 240, 60 };
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("GAME OVER", 151, 151, 50, RED);
            DrawText("GAME OVER", 149, 149, 50, RED);
            DrawText("GAME OVER", 150, 150, 50, RED);
            
            DrawRectangleRec(playBtn2, hover ? GRAY : DARKGRAY);
            DrawRectangleLinesEx(playBtn2, 2, RAYWHITE);
            DrawText("RESTART", (int)playBtn2.x + 45, (int)playBtn2.y + 15, 30, RAYWHITE);

            
            DrawRectangleRec(quit, hover2 ? GRAY : DARKGRAY);
            DrawRectangleLinesEx(quit, 2, RAYWHITE);
            DrawText("QUIT", (int)quit.x + 85, (int)quit.y + 15, 30, RAYWHITE);

            EndDrawing();
            continue;
        }

        // --- DRAW ---
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(background, 0, 0, WHITE);

        player.Draw();

        for (const auto& b : bullets)
            b.Draw();

        for (const auto& e : enemies)
            e.Draw();

        //DrawText("Shmup - Prototype", 10, 10, 20, LIGHTGRAY);
        DrawText(TextFormat("Score : %d", score), 10, 20, 20, LIGHTGRAY);
        DrawText(TextFormat("Lives : %d", player.GetLives()), 10, 50, 20, LIGHTGRAY);
        
        DrawText(TextFormat("Ammo : %d / %d", currentAmmo, maxAmmo),10, 710, 20, LIGHTGRAY);
        DrawText(TextFormat("Super (SHIFT + SPACE) : %d", tripleShots), 10, 740, 20, LIGHTGRAY);

        if (isReloading)
        {
            DrawText("RELOADING...", 10, 770, 20, RED);
        }
        
        EndDrawing();
    }

    UnloadTexture(background);
    Enemy::UnloadSharedTexture();
    CloseWindow();
    return 0;
}
