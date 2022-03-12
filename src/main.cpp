#include <raylib-cpp.hpp>
#include <vector>
#include <cmath>
#include <iostream>

struct Player
{
	int width;
	int height;
	std::pair<float, float> pos;
	std::pair<float, float> pos_old;
	std::pair<float, float> centre;
	std::pair<float, float> speed;
	float acceleration = 5.0f;
	float friction = 0.98f;
	raylib::Color colour = BLACK;
	bool can_jump = false;
};

struct Box
{
	int width;
	int height;
	std::pair<float, float> pos;
	raylib::Color colour;
};

int Check_collision(std::pair<float, float> p,
					std::pair<float, float> b,
					int w1,
					int w2,
					int h1,
					int h2);

std::pair<float, float> Calculate_aabb_distance_to(Player p, Box b);
void Resolve_collision(std::pair<float, float> distance, Player &p, Box &b);

int main()
{
	// Initialization
	int screen_width = 1280;
	int screen_height = 720;
	float fps = 60.0f;
	float delta_time = 0.0f;

	raylib::Window w(screen_width, screen_height, "Collision playground");

	struct Player p = {64, 64, {0.0f, 0.0f}, p.pos, {(p.pos.first + p.width / 2), (p.pos.second + p.height / 2)}, {0.0f, 0.0f}};
	struct Box b = {64, 64, {100.0f, 100.0f}, GREEN};
	float x_axis_time_to_collide;
	float y_axis_time_to_collide;

	SetTargetFPS(fps);

	SetWindowPosition(1080, 400);

	// Main game loop
	while (!w.ShouldClose()) // Detect window close button or ESC key
	{
		// Update
		delta_time = GetFrameTime();
		p.pos_old = p.pos;

		if (IsKeyDown('A'))
		{
			p.speed.first -= p.acceleration * delta_time;
		}
		else if (IsKeyDown('D'))
		{
			p.speed.first += p.acceleration * delta_time;
		}
		else
		{
			p.speed.first *= p.friction;
		}
		if (IsKeyDown('W'))
		{
			p.speed.second -= p.acceleration * delta_time;
		}
		else if (IsKeyDown('S'))
		{
			p.speed.second += p.acceleration * delta_time;
		}
		else
		{
			p.speed.second *= p.friction;
		}

		// do collision here to start with
		std::pair<float, float> distance = Calculate_aabb_distance_to(p, b);
		std::cout << "disX: " << distance.first << "\n";
		std::cout << "disY: " << distance.second << "\n";
		if (!Check_collision(p.pos, b.pos, p.width, b.width, p.height, b.height))
		{
			Resolve_collision(distance, p, b);
			Resolve_collision(distance, p, b);
		}
		else {
			p.pos.first += p.speed.first;
			p.pos.second += p.speed.second;
		}

		// if (p.speed.first != 0)
		// {
		// 	x_axis_time_to_collide = std::abs(distance.first / p.speed.first);
		// }
		// else
		// {
		// 	x_axis_time_to_collide = 0;
		// }
		// if (p.speed.second != 0)
		// {
		// 	y_axis_time_to_collide = std::abs(distance.second / p.speed.second);
		// }
		// else
		// {
		// 	y_axis_time_to_collide = 0;
		// }

		// float shortestTime = 0;

		// if (p.speed.first != 0 && p.speed.second == 0)
		// {
		// 	// Collison on X-axis only
		// 	shortestTime = x_axis_time_to_collide;
		// 	p.speed.first = shortestTime * p.speed.first;
		// }
		// else if (p.speed.first == 0 && p.speed.second != 0)
		// {
		// 	// Collision on Y-axis only
		// 	shortestTime = y_axis_time_to_collide;
		// 	p.speed.second = shortestTime * p.speed.second;
		// }
		// else
		// {
		// 	// Collision on X and Y axis (eg. slide up against a wall)
		// 	shortestTime = std::min(std::abs(x_axis_time_to_collide), std::abs(y_axis_time_to_collide));
		// 	p.speed.first = shortestTime * p.speed.first;
		// 	p.speed.second = shortestTime * p.speed.second;
		// }

		// Draw
		BeginDrawing();
		ClearBackground(LIGHTGRAY);

		DrawRectangle(p.pos.first, p.pos.second, p.width, p.height, p.colour);
		DrawRectangle(b.pos.first, b.pos.second, b.width, b.height, b.colour);

		EndDrawing();
	}
	return 0;
}

// https://www.deengames.com/blog/2020/a-primer-on-aabb-collision-resolution.html

int Check_collision(std::pair<float, float> p,
					std::pair<float, float> b,
					int w1,
					int w2,
					int h1,
					int h2)
{
	if (p.first < b.first + w2 &&
		p.first + w1 > b.first &&
		p.second < b.second + h2 &&
		p.second + h1 > b.second)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

std::pair<float, float> Calculate_aabb_distance_to(Player p, Box b)
{
	float dx = 0.0f;
	float dy = 0.0f;
	std::pair<float, float> return_value;

	if (p.pos.first < b.pos.first)
	{
		dx = b.pos.first - (p.pos.first + p.width);
	}
	else if (p.pos.first > b.pos.first)
	{
		dx = p.pos.first - (b.pos.first + b.width);
	}

	if (p.pos.second < b.pos.second)
	{
		dy = b.pos.second - (p.pos.second + p.height);
	}
	else if (p.pos.second > b.pos.second)
	{
		dy = p.pos.second - (b.pos.second + b.height);
	}
	return_value.first = dx;
	return_value.second = dy;

	return return_value;
}

void Resolve_collision(std::pair<float, float> distance, Player &p, Box &b)
{
	// more work to anticipate step value required
	// right and bottom side has issues when editing for more realism
	if (p.pos.first < b.pos.first)
	{
		p.pos.first = p.pos_old.first - 0.2;
		p.speed.first = -p.speed.first / 2;
	}
	else if (p.pos.first > b.pos.first)
	{
		p.pos.first = p.pos_old.first + 0.2;
		p.speed.first = -p.speed.first / 2;
	}
	if (p.pos.second < b.pos.second)
	{
		p.pos.second = p.pos_old.second - 0.2;
		p.speed.second = -p.speed.second / 2;
	}
	else if (p.pos.second > b.pos.second)
	{
		p.pos.second = p.pos_old.second + 0.2;
		p.speed.second = -p.speed.second / 2;
	}
}