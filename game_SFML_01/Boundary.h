#pragma once

#include <SFML/Graphics.hpp>

struct Bound_box
{
	enum Bound_type { bl_wall, bl_rib, bl_green, bl_red, bl_blue };

	sf::Vector2f p[2]; // top-left, bottom-right corner
	// float rotation;
	Bound_type type;

	inline bool inside(sf::Vector2f point) const
	{
		return (p[0].x < point.x && point.x < p[1].x &&
				p[1].y < point.y && point.y < p[0].y);
	}

};

