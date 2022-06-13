#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <Vector2D.h>

class Obstacle
{
public:
	Obstacle(float x, float y, float r) : position(x, y), radius(r) {}
	void draw(SDL_Renderer* renderer)
	{
		aacircleRGBA(renderer, position.getX(), position.getY(), radius, 255, 255, 255, 255);
	}

	Vector2D IntoLocalSpace(const Vector2D& vehiclePosition, float vehicleHeading)
	{
		return (position - vehiclePosition).Rotate(-vehicleHeading);
	}

	Vector2D position;
	float radius;
};