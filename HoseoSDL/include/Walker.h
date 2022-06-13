#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <Vector2D.h>
#include <vector>
class Obstacle;

class Walker
{
public:
	Walker(float x, float y);
	void draw(SDL_Renderer* renderer);
	void update();

	void applyForce(const Vector2D& force)
	{
		acceleration += force;
	}

	void edges();
	Vector2D seek(const Vector2D& target);
	Vector2D evade(Walker* hunter);
	Vector2D arrive(const Vector2D& target);
	Vector2D hide(const std::vector<Obstacle*>& obstacles, Walker* hunter);
	Vector2D getHidingPos(Obstacle* obst, const Vector2D& hunterPos);
	Vector2D obstacleAvoidance(const std::vector<Obstacle*>& obstacles);

	Vector2D position;
	Vector2D velocity;
	Vector2D acceleration;

	float r = 16;
	float heading = 0;
	float maxSpeed = 4;
	float maxForce = 0.2;
};