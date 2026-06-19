#include <utility>
#include <iostream>
#include "Player.h"
#include "constants.h"

Player::Player() :
	color(Color::red), alive(true), win(false), on_ground(false), mass(1.0f), size(0.0, 0.0),
	position(0.0, 0.0), speed(0.0, 0.0), accel(0.0, 0.0)//, jerk(0.0, 0.0)
{
}

void Player::init(sf::Vector2f position, std::string texture_file, Color color)
{
	texture.loadFromFile(texture_file);
	drawable_object.setTexture(&texture, true);

	auto tex_size = drawable_object.getTexture()->getSize();
	drawable_object.setSize(static_cast<sf::Vector2f>(tex_size)); // resize according to texture
	size.x = (float)tex_size.x / c_pixels_per_meter;
	size.y = (float)tex_size.y / c_pixels_per_meter;

	this->position = position;
	this->color = color == Color::blue ? Color::blue : Color::red; // defaults to red
}

void Player::update(std::vector<Player>& players, const Boundaries& map_bounds, float delta_time)
{
	for (auto& p : players)
	{

		if (!p.alive || p.win)
		{
			continue;
		}
		
		p.accel.y = -c_gravity_acc;
		p.accel.x = 0.0f; // for now we are setting horizontal speed directly by "<-", "->" arrow key_shortcuts
		p.speed += p.accel * delta_time;

		// horizontal move - for simplicity directly set the speed, infinity acceleration :)
		p.speed.x = p.move_flags[left] ? -c_hmove_speed : (p.move_flags[right] ? c_hmove_speed : 0.f);

		// can & want jump ?
		if (p.on_ground && p.move_flags[jump]) p.speed.y += c_jump_speed;
		p.move_flags[jump] = false; // clean up

		auto delta = p.speed * delta_time;
		auto old_pos = p.position;
		auto new_pos = p.position + delta;
		p.on_ground = false;

		// check map boundaries - walls, doors, ribs ...
		for (auto& b : map_bounds.walls)
		{
			if (b.inside(new_pos))
			{

				if (b.color != Color::none && b.color != p.color)
				{
					std::cout << "Player " << p.color << " died\n";
					p.alive = false;
					break; // for
				}

				else
				{
					// collision, check in which coordinate
					bool xc = b.inside({ new_pos.x, old_pos.y });
					bool yc = b.inside({ old_pos.x, new_pos.y });
					// decide what to do
					if (xc)
					{
						new_pos.x = old_pos.x;
						p.speed.x = 0.f;
					}
					if (yc)
					{
						new_pos.y = old_pos.y;
						if (p.speed.y < 0.f)
						{
							p.on_ground = true;
						}
						p.speed.y = 0;
					}
				}

			}
		}

		for (auto& b : map_bounds.ribs)
		{
			if (b.inside(new_pos))
			{
				p.speed.y = p.move_flags[up] ? c_vmove_speed : (p.move_flags[down] ? -c_vmove_speed : 0.f);
				new_pos = p.position + (p.speed * delta_time);
			}
		}

		for (auto& b : map_bounds.doors)
		{
			if (b.inside(new_pos) && b.color == p.color)
			{
				// player in his door
				std::cout << "Player " << p.color << " win!\n";
				p.win = true;
				break; // for
			}
		}

		// check colision with other players - stand on top of each other, but not go through each other
		if (p.speed.y < 0.f) // only if player is falling down
		{
			for (auto& other : players)
			{
				if (&other == &p || !other.alive) continue; // skip self and dead players

				if ((other.position.x - other.size.x * 0.5f) < new_pos.x && new_pos.x < (other.position.x + other.size.x * 0.5f) &&
					(other.position.y + (other.size.y * 0.9f)) < new_pos.y && new_pos.y < (other.position.y + other.size.y))
				{
					new_pos.y = old_pos.y;
					if (p.speed.y < 0.f)
					{
						p.on_ground = true;
					}
					p.speed.y = 0;
				}
			}
		}

		p.position = new_pos;
	}
}

void Player::draw(std::vector<Player>& players, sf::RenderWindow& window, sf::Vector2f view_pos)
{
	auto win_size_m = sf::Vector2f(window.getSize()) / c_pixels_per_meter; // window size in meters

	for (auto& p : players)
	{
		auto pos_on_screen = p.position - view_pos;
		pos_on_screen.x -= p.size.x * 0.5f; // center ->
		pos_on_screen.y += p.size.y; // <- bottom corner
		pos_on_screen.y = win_size_m.y - pos_on_screen.y; // flip Y coordinate
		p.drawable_object.setPosition(pos_on_screen * c_pixels_per_meter); // recalc to pixels & update drawable_object
		window.draw(p.drawable_object); // draw it
	}
}
