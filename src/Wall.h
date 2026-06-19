#pragma once

#include <SFML/Graphics.hpp>
#include "Defs.h"

struct Wall
{

	Wall(sf::Vector2f p0, sf::Vector2f p1) :
		p{p0, p1}
	{
	}

	sf::Vector2f p[2]; // bottom-left, top-right corner

	inline bool inside(sf::Vector2f point) const
	{
		return (p[0].x <= point.x && point.x <= p[1].x &&
				p[0].y <= point.y && point.y <= p[1].y);
	}

};

struct Wall_coloured : public Wall
{

    Wall_coloured(sf::Vector2f p0, sf::Vector2f p1, Color color) :
        Wall(p0, p1), color(color)
    {
    }

    Color color; // none - all players, red/blue - only player of this color, green - both players will die

};