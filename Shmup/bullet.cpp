#include "bullet.h"

bullet::bullet(float x, float y, float speed, float size, Color color)
    : _pos{ x, y }, _speed{ speed }, _size{ size }, _color{ color }
{
}

void bullet::Update(float dt)
{
    // Monte vers le haut
    _pos.y -= _speed * dt;
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
    return _pos.y + _size < 0;
}

Vector2 bullet::GetPosition() const
{
    return _pos;
}

Rectangle bullet::GetRect() const
{
    return {
        _pos.x - _size / 2.0f,
        _pos.y - _size,
        _size,
        _size * 2.0f
    };
}