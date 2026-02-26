#pragma once
#include "raylib.h"

class bullet
{
public:
    bullet(float x, float y, float speed, float size, Color color);

    void Update(float dt);
    void Draw() const;

    bool IsOffScreen() const;
    Vector2 GetPosition() const;
    Rectangle GetRect() const;

private:
    Vector2 _pos;
    float _speed;
    float _size;
    Color _color;
};

