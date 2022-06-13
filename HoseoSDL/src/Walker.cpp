#include "Walker.h"
#include "Obstacle.h"

const int width = 600;
const int height = 400;

Walker::Walker(float x, float y) : position(x, y), velocity(1, 1), acceleration(0, 0)
{

}

void Walker::update()
{
	velocity += acceleration;
	velocity.limit(maxSpeed);
	position += velocity;
	acceleration *= 0;

	heading = atan2(velocity.getY(), velocity.getX());
}

void Walker::draw(SDL_Renderer* renderer)
{
	Vector2D vertex1 = Vector2D(r, 0).Rotate(heading);
	Vector2D vertex2 = Vector2D(-r, r/2).Rotate(heading);
	Vector2D vertex3 = Vector2D(-r, -r/2).Rotate(heading);

	vertex1 += position;
	vertex2 += position;
	vertex3 += position;

	aatrigonRGBA(renderer, vertex1.getX(), vertex1.getY(), vertex2.getX(), vertex2.getY(), vertex3.getX(), vertex3.getY(), 255, 255, 255, 255);
}

void Walker::edges()
{
	if (position.getX() > width) position.setX(0);
	else if (position.getX() < 0) position.setX(width);
	if (position.getY() > height) position.setY(0);
	else if (position.getY() < 0) position.setY(height);
}

Vector2D Walker::seek(const Vector2D& target)
{
	Vector2D desire = (target - position).Normalized() * maxSpeed - velocity;
	desire.limit(maxForce);
	return desire;
}

Vector2D Walker::evade(Walker* hunter)
{
	Vector2D prediction = hunter->velocity;
	float dist = (position - hunter->position).length();
	prediction = prediction * dist / 30;
	return seek(hunter->position + prediction) * -1;
}

Vector2D Walker::arrive(const Vector2D& target)
{
	Vector2D direction = target - position;
	float dist = direction.length();

	if (dist > 0)
	{
		float speed = dist / 2;
		if (speed > maxSpeed) speed = maxSpeed;

		return direction / dist * speed - velocity;
	}

	return Vector2D(0, 0);
}

Vector2D Walker::hide(const std::vector<Obstacle*>& obstacles, Walker* hunter)
{
	float closest = 1000000;
	Vector2D bestHidingPos = position;

	for (const auto& o : obstacles)
	{
		Vector2D hidingPos = getHidingPos(o, hunter->position);
		float dist = (hidingPos - position).length();

		if (dist < closest)
		{
			closest = dist;
			bestHidingPos = hidingPos;
		}
	}

	if (closest == 1000000) return evade(hunter);

	return arrive(bestHidingPos);
}

Vector2D Walker::getHidingPos(Obstacle* obst, const Vector2D& hunterPos)
{
	Vector2D direction = obst->position - hunterPos;
	direction = direction.Normalized() * (obst->radius + 20);
	return obst->position + direction;
}

Vector2D Walker::obstacleAvoidance(const std::vector<Obstacle*>& obstacles)
{
	float closest = 1000000;
	Vector2D closestLocal(0, 0);
	Obstacle* closestObst = nullptr;

	float boxLength = 50 + (velocity.length() / maxSpeed) * 50;

	for (const auto& o : obstacles)
	{
		if ((o->position - position).lengthSqr() < boxLength * boxLength)
		{
			Vector2D localPos = o->IntoLocalSpace(position, heading);

			if (localPos.getX() >= 0)
			{
				float expanded = o->radius + r;
				
				if (abs(localPos.getY()) < expanded)
				{
					float cX = localPos.getX();
					float cY = localPos.getY();

					float sqrtPart = sqrt(expanded * expanded - cY * cY);
					float selected = cX - sqrtPart;
					if (selected < 0) selected = cX + sqrtPart;

					if (selected < closest)
					{
						closest = selected;
						closestLocal = localPos;
						closestObst = o;
					}
				}
			}
		}
	}

	Vector2D steering(0, 0);

	if (closestObst)
	{
		float mult = 1 + (boxLength - closestLocal.getX()) / boxLength;
		steering.setY((closestObst->radius - abs(closestLocal.getY())) * mult * (closestLocal.getY() > 0 ? 1 : -1));
		
		float braking = 0.2;
		steering.setX((closestObst->radius - closestLocal.getX()) * braking);
	}

	return steering.Rotate(heading);
}
