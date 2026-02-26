#pragma once
#include "raylib.h"
class Ship
{
public:
	Ship(float x, float y, float speed);
	~Ship();

	void Update(float dt); // Lire les inputs et se déplacer
	void Draw() const; // Se dessiner à l'écran

	Vector2 GetPosition() const;

	int GetLives() const;
	void TakeDamage();

	void Reset(float x, float y);

private:
	Vector2 _pos;
	float _speed;
	Texture2D _texture;
	float _scale;
	int _lives;
};
	