#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Boundaries.h"

class Level_map
{
public:

	/**
	 * Initializes map. Loads all data from files (textures, boundaries).
	 * 
	 * \param map_file	path to map file in text format
	 */
	void init(std::string map_file);

	/**
	 * Draws map texture to given position in window.
	 * 
	 * \param window	window	window instance
	 * \param view_pos	position of bottom-left corner of view-port 
	 *					(visible part of map) on the map in meters
	 * \param debug		true to draw boundary  boxes
	 */
	void draw(sf::RenderWindow& window, sf::Vector2f view_pos, bool debug = false);

	/**
	 * \return			Total size of map in meters.
	 */
	inline sf::Vector2f get_size() const { return size; }

	inline sf::Vector2f get_start(bool red) const { return start[red ? 1 : 0]; }

	/**
	 * \return			Read-only reference to map boundaries.
	 */
	inline const Boundaries& get_boundaries() const { return bounds; }

private:

	Boundaries bounds;					///< map boundaries e.g. walls, doors, ribs
	sf::Vector2f size;					///< size in meters
	sf::Vector2f start[2];				///< starting position of blue and red players
	sf::Texture texture;				///< background texture of map
	sf::RectangleShape background_img;

	/**
	 * Draws boundary box for debug purposes.
	 * \param window	window instance
	 * \param view_pos	position of bottom-left corner of view-port 
	 *					(visible part of map) on the map in meters
	 * \param p			bottom-left and top-right corner of box
	 * \param color		color of box
	 */
	void _debug_draw_bound(sf::RenderWindow& window, sf::Vector2f view_pos, sf::Vector2f p[2], sf::Color color) const;
};

