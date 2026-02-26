#include "Bullet.h"

bullet::bullet(float x, float y, Vector2 vel, float size, Color color)
    : _pos{ x, y }, _vel{ vel }, _size{ size }, _color{ color }
{
}

void bullet::Update(float dt)
{
    _pos.x += _vel.x * dt;
    _pos.y += _vel.y * dt;
}

void bullet::Draw() const
{
    DrawRectangle(
        (int)(_pos.x - _size / 2),
        (int)(_pos.y - _size),
        (int)_size,
        (int)(_size * 3),
        _color
    );
}

bool bullet::IsOffScreen() const
{
    return (_pos.y + _size < 0) || (_pos.y - _size > GetScreenHeight())
        || (_pos.x + _size < 0) || (_pos.x - _size > GetScreenWidth());
}

Rectangle bullet::GetRect() const
{
    return { _pos.x - _size / 2.0f, _pos.y - _size, _size, _size * 2.0f };
}