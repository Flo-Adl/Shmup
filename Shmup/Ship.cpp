#include "Ship.h"

Ship::Ship(float x, float y, float speed)
    : _pos{ x, y }, _speed{ speed }, _scale{ 0.13f }, _lives{ 3 }
{
    _texture = LoadTexture("C:/Users/adell/Desktop/Programmation C++/TP1/Shmup/ship.png");

    if (_texture.id == 0)
        TraceLog(LOG_ERROR, "Ship texture not loaded!");
}

Ship::~Ship()
{
    UnloadTexture(_texture);
}

void Ship::Update(float dt)
{
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) _pos.x += _speed * 0.025;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  _pos.x -= _speed * 0.025;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    _pos.y -= _speed * dt;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  _pos.y += _speed * dt;

    float width = _texture.width * _scale;
    if (_pos.x < 0) _pos.x = 0;
    if (_pos.x > GetScreenWidth() - width)
        _pos.x = GetScreenWidth() - width;

    float height = _texture.height * _scale;
    float bottommargin = 75;
    if (_pos.y < 0) _pos.y = 0;
    if (_pos.y > GetScreenHeight() - height - bottommargin)
        _pos.y = GetScreenHeight() - height - bottommargin;
}

void Ship::Draw() const
{
    DrawTextureEx(_texture, _pos, 0.0f, _scale, WHITE);
}

Vector2 Ship::GetPosition() const
{
    return {
        _pos.x + (_texture.width * _scale) / 2.0f,
        _pos.y
    };
}

int Ship::GetLives() const
{
    return _lives;
}

void Ship::TakeDamage()
{
    _lives--;

    if (_lives < 0)
        _lives = 0;
}

void Ship::Reset(float x, float y)
{
    _pos = { x, y };
    _lives = 3;
}