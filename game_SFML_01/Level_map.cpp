#include <fstream>
#include "Level_map.h"
#include "constants.h"

void Level_map::init(std::string map_file)
{	
	// load from map file
	std::fstream bf;
	bf.open(map_file, std::ios_base::in);
	std::string texture_file;
	bf >> texture_file;

	// load texture
	texture.loadFromFile(texture_file);
	drawable_object.setTexture(&texture, true);
	auto tex_size = drawable_object.getTexture()->getSize();
	drawable_object.setSize(static_cast<sf::Vector2f>(tex_size)); // resize according to texture
	size.x = (float)tex_size.x / pixels_per_meter;
	size.y = (float)tex_size.y / pixels_per_meter;

	// load objects
	float x, y, a, b;
	char unit;
	std::string type;
	boundaries.clear();
	while (bf.good())
	{
		bf >> x >> y >> a >> b >> unit >> type;
		if (unit == 'p')
		{
			// units are texture pixels with 0,0
			// in top-left corner, convert to meters and flip Y
			x /= pixels_per_meter;
			y /= pixels_per_meter;
			a /= pixels_per_meter;
			b /= pixels_per_meter;
			y = size.y - y; // flip Y
			b = size.y - b;
		}
		Bound_box::Bound_type bt;
		if (type == "start") {
			start[0] = { x,y };
			start[1] = { a,b };
			continue; // not a boundary-box but starting positions for blue and red players
		}
		else if (type == "rib") bt = Bound_box::Bound_type::bl_rib;
		else if (type == "red") bt = Bound_box::Bound_type::bl_red;
		else if (type == "blue") bt = Bound_box::Bound_type::bl_blue;
		else if (type == "green") bt = Bound_box::Bound_type::bl_green;
		else if (type == "r_door") bt = Bound_box::Bound_type::bl_r_door;
		else if (type == "b_door") bt = Bound_box::Bound_type::bl_b_door;
		else bt = Bound_box::Bound_type::bl_wall;
		boundaries.emplace_back(
			sf::Vector2f(x,y),
			sf::Vector2f(a,b),
			bt );
	}
	boundaries.emplace_back(
		sf::Vector2f(-100.f, -100.f),
		sf::Vector2f(0.f, size.y),
		Bound_box::Bound_type::bl_wall
	);
	boundaries.emplace_back(
		sf::Vector2f(size.x, -100.f),
		sf::Vector2f(size.x + 100.f, size.y),
		Bound_box::Bound_type::bl_wall
	);
	boundaries.emplace_back(
		sf::Vector2f(-100.f, -100.f),
		sf::Vector2f(size.x + 100.f, 0.0f),
		Bound_box::Bound_type::bl_green		// you will die if you will fall
	);
}

void Level_map::draw(sf::RenderWindow& window, sf::Vector2f view_pos, bool debug)
{
	auto win_size_m = sf::Vector2f(window.getSize()) / pixels_per_meter; // window size in meters
	sf::Vector2f pos_on_screen = -view_pos;
	pos_on_screen.y += size.y;// bottom corner
	pos_on_screen.y = win_size_m.y - pos_on_screen.y; // flip Y coordinate
	drawable_object.setPosition(pos_on_screen * pixels_per_meter); // recalc to pixels & update drawable_object
	window.draw(drawable_object);

	if (debug)
	{
		// draw boundaries
		sf::RectangleShape bound_draw;
		for (auto& b : boundaries)
		{
			auto size = b.p[1] - b.p[0];
			auto pos_on_screen = b.p[0] - view_pos;
			pos_on_screen.y += size.y;// bottom corner
			pos_on_screen.y = win_size_m.y - pos_on_screen.y; // flip Y coordinate
			bound_draw.setSize(size * pixels_per_meter);
			bound_draw.setPosition(pos_on_screen * pixels_per_meter);
			bound_draw.setFillColor(Bound_box::bound_type_2_color(b.type, 128));
			window.draw(bound_draw);
		}
	}
}
