#include "Level_map.h"
#include "constants.h"

void Level_map::init(std::string texture_file)
{
	texture.loadFromFile(texture_file);
	drawable_object.setTexture(&texture, true);
	auto tex_size = drawable_object.getTexture()->getSize();
	drawable_object.setSize(static_cast<sf::Vector2f>(tex_size)); // resize according to texture
	size.x = (float)tex_size.x / pixels_per_meter;
	size.y = (float)tex_size.y / pixels_per_meter;

	boundaries.clear();
	boundaries.push_back(Bound_box{
		sf::Vector2f(-100.f,size.y),
		sf::Vector2f(0.f,-100.f),
		Bound_box::Bound_type::bl_wall
		});
	boundaries.push_back(Bound_box{
		sf::Vector2f(size.x,size.y),
		sf::Vector2f(size.x+100.f,-100.f),
		Bound_box::Bound_type::bl_wall
		});
	boundaries.push_back(Bound_box{
		sf::Vector2f(-100.f,0.f),
		sf::Vector2f(size.x+100.f,-100.0f),
		Bound_box::Bound_type::bl_wall
		});
	// todo: load others from file
	// check units (pixels vs meters) and converrt to meters
}

void Level_map::draw(sf::RenderWindow& window, sf::Vector2f view_pos)
{
	auto win_size_m = sf::Vector2f(window.getSize()) / pixels_per_meter; // window size in meters
	sf::Vector2f pos_on_screen = -view_pos;
	pos_on_screen.y += size.y;// bottom corner
	pos_on_screen.y = win_size_m.y - pos_on_screen.y; // flip Y coordinate
	drawable_object.setPosition(pos_on_screen * pixels_per_meter); // recalc to pixels & update drawable_object
	window.draw(drawable_object);
}
