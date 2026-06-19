#include <fstream>
#include <iostream>
#include "Level_map.h"
#include "constants.h"

void Level_map::init(std::string map_file)
{	
	// load from map file
	std::fstream bf;
	bf.open(map_file, std::ios_base::in);
	std::string texture_file;
	bf >> texture_file;

	// load map background texture
	if (!texture.loadFromFile("assets/" + texture_file)) {
		throw std::runtime_error("Failed to load map texture from file: assets/" + texture_file);
	}
	background_img.setTexture(&texture, true);
	auto tex_size = background_img.getTexture()->getSize();
	background_img.setSize(static_cast<sf::Vector2f>(tex_size)); // resize according to texture
	size.x = (float)tex_size.x / c_pixels_per_meter;
	size.y = (float)tex_size.y / c_pixels_per_meter;

	// load objects / boundaries
	bounds.walls.clear();
	bounds.doors.clear();
	bounds.ribs.clear();

	float x, y, a, b;
	char unit;
	std::string type;

	while (bf.good())
	{
		bf >> x >> y >> a >> b >> unit >> type;

		if (unit == 'p')
		{
			// units are texture pixels with 0,0
			// in top-left corner, convert to meters and flip Y
			x /= c_pixels_per_meter;
			y /= c_pixels_per_meter;
			a /= c_pixels_per_meter;
			b /= c_pixels_per_meter;
			y = size.y - y; // flip Y
			b = size.y - b;
		}

		if (type == "start") {
			start[0] = { x,y };
			start[1] = { a,b };
		}

		else if (type == "rib")
		{
			bounds.ribs.emplace_back(
				sf::Vector2f(x, y),
				sf::Vector2f(a, b));
		}

		else if (type == "r_wall" || type == "b_wall" || type == "g_wall" || type == "wall")
		{
			bounds.walls.emplace_back(
				sf::Vector2f(x, y),
				sf::Vector2f(a, b),
				type == "r_wall" ? Color::red :
				type == "b_wall" ? Color::blue :
				type == "g_wall" ? Color::green : Color::none);
		}

		else if (type == "r_door" || type == "b_door")
		{
			bounds.doors.emplace_back(
				sf::Vector2f(x, y),
				sf::Vector2f(a, b),
				type == "r_door" ? Color::red : Color::blue);
		}

		else
		{
			std::cout << "Unknown object type in map file: " << type << std::endl;
		}

	}

	// add walls around map to prevent players from falling out of map
	bounds.walls.emplace_back(
		sf::Vector2f(-100.f, -100.f),
		sf::Vector2f(0.1f, size.y),
		Color::none
	);
	bounds.walls.emplace_back(
		sf::Vector2f(size.x - 0.1f, -100.f),
		sf::Vector2f(size.x + 100.f, size.y),
		Color::none
	);
	bounds.walls.emplace_back(
		sf::Vector2f(-100.f, -100.f),
		sf::Vector2f(size.x + 100.f, 0.0f),
		Color::green // you will die if you will fall
	);

}

void Level_map::draw(sf::RenderWindow& window, sf::Vector2f view_pos, bool debug)
{
	sf::Vector2f pos_on_screen = -view_pos;

	auto win_size_m = sf::Vector2f(window.getSize()) / c_pixels_per_meter; // window size in meters
	pos_on_screen.y += size.y;// bottom corner
	pos_on_screen.y = win_size_m.y - pos_on_screen.y; // flip Y coordinate
	
	background_img.setPosition(pos_on_screen * c_pixels_per_meter); // recalc to pixels & update background_img
	window.draw(background_img);

	if (debug)
	{
		// draw boundaries
		for (auto& b : bounds.walls)
		{
			_debug_draw_bound(window, view_pos, b.p,
				b.color == Color::red ? sf::Color(255, 0, 0, 100) :
				b.color == Color::blue ? sf::Color(0, 0, 255, 128) :
				b.color == Color::green ? sf::Color(0, 255, 0, 128) :
				sf::Color(128, 64, 0, 128)
			);
		}

		for (auto& b : bounds.ribs)
		{
			_debug_draw_bound(window, view_pos, b.p, { 255, 0, 255, 128 });
		}

		for (auto& b : bounds.doors)
		{
			_debug_draw_bound(window, view_pos, b.p,
				b.color == Color::red ? sf::Color(255, 0, 0, 128) :
				b.color == Color::blue ? sf::Color(0, 0, 255, 128) :
				sf::Color(128, 64, 0, 128)
			);
		}
	}
}

void Level_map::_debug_draw_bound(sf::RenderWindow &window, sf::Vector2f view_pos, sf::Vector2f p[2], sf::Color color) const
{
	sf::RectangleShape bound_draw;
	auto win_size_m = sf::Vector2f(window.getSize()) / c_pixels_per_meter; // window size in meters

	auto size = p[1] - p[0];
	auto pos_on_screen = p[0] - view_pos;
	pos_on_screen.y += size.y;// bottom corner
	pos_on_screen.y = win_size_m.y - pos_on_screen.y; // flip Y coordinate
	bound_draw.setSize(size * c_pixels_per_meter);
	bound_draw.setPosition(pos_on_screen * c_pixels_per_meter);
	bound_draw.setFillColor(color);
	window.draw(bound_draw);
}
