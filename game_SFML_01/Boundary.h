#pragma once

#include <SFML/Graphics.hpp>

struct Bound_box
{
	enum Bound_type { bl_wall, bl_rib, bl_green, bl_red, bl_blue, bl_r_door, bl_b_door };

	static sf::Color bound_type_2_color(Bound_type type, uint8_t alpha)
	{
		uint8_t r = 0;
		uint8_t g = 0;
		uint8_t b = 0;

		switch (type)
		{
		case bl_wall:
			r = 100;
			g = 100;
			break;
		case bl_rib:
			r = 255;
			b = 255;
			break;
		case bl_red:
			r = 255;
			break;
		case bl_green:
			g = 255;
			break;
		case bl_blue:
			b = 255;
			break;
		case bl_r_door:
			r = 100;
			break;
		case bl_b_door:
			b = 100;
			break;
		}

		return sf::Color(r, g, b, alpha);
	}

	Bound_box(sf::Vector2f p0, sf::Vector2f p1, Bound_type type_) :
		p{p0, p1}, type(type_)
	{
	}

	sf::Vector2f p[2]; // bottom-left, top-right corner
	// float rotation;
	Bound_type type;

	inline bool inside(sf::Vector2f point) const
	{
		return (p[0].x < point.x && point.x < p[1].x &&
				p[0].y < point.y && point.y < p[1].y);
	}

};

