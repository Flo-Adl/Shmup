#pragma once
#include "raylib.h"

class Enemy
{
public:
    static void LoadSharedTexture();
    static void UnloadSharedTexture();

    Enemy(float x, float y, float speed, float scale);
    void Update(float dt);
    void Draw() const;
    bool IsOffScreen() const;

    Vector2 GetPosition() const;
    float GetRadius() const;

private:
    Vector2 _pos;
    float _speed;
    float _scale;

    static Texture2D s_texture;
    static bool s_loaded;
};