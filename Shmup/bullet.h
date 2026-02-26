#pragma once
#include "raylib.h"

class bullet
{
public:
    bullet(float x, float y, Vector2 vel, float size, Color color);

    void Update(float dt);
    void Draw() const;
    bool IsOffScreen() const;

    Rectangle GetRect() const;

private:
    Vector2 _pos;
    Vector2 _vel;
    float _size;
    Color _color;
};