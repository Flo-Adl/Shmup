#include "Enemy.h"

Texture2D Enemy::s_texture = {};
bool Enemy::s_loaded = false;

void Enemy::LoadSharedTexture()
{
    if (!s_loaded)
    {
        s_texture = LoadTexture("C:/Users/adell/Desktop/Programmation C++/TP1/Shmup/asteroid.png");
        if (s_texture.id == 0) TraceLog(LOG_ERROR, "Enemy texture not loaded!");
        s_loaded = true;
    }
}

void Enemy::UnloadSharedTexture()
{
    if (s_loaded && s_texture.id != 0)
    {
        UnloadTexture(s_texture);
        s_texture = {};
        s_loaded = false;
    }
}

Enemy::Enemy(float x, float y, float speed, float scale)
    : _pos{ x, y }, _speed{ speed }, _scale{ scale }
{
}

void Enemy::Update(float dt)
{
    _pos.y += _speed * dt;

    float width = s_texture.width * _scale;
   
    if (_pos.x < 0) _pos.x = 0;
    if (_pos.x > GetScreenWidth() - width)
        _pos.x = GetScreenWidth() - width;
}

void Enemy::Draw() const
{
    if (s_texture.id == 0) return;
    DrawTextureEx(s_texture, _pos, 0.0f, _scale, WHITE);
}

bool Enemy::IsOffScreen() const
{
    float h = s_texture.height * _scale;
    return _pos.y > GetScreenHeight() + h -105;
}

Vector2 Enemy::GetPosition() const
{
    return {
        _pos.x + (s_texture.width * _scale) / 2.0f,
        _pos.y + (s_texture.height * _scale) / 2.0f
    };
}

float Enemy::GetRadius() const
{
    return (s_texture.width * _scale) / 2.0f;
}