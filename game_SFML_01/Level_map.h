#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Boundary.h"

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
	 * \param view_pos	position of bottom-left			c	ddddorner of view-port 
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
	 * \return			Read-only vector of all map boundary lines.
	 */
	inline const std::vector<Bound_box>& get_boundaries() const { return boundaries; }

private:

	std::vector<Bound_box> boundaries;	// vector of boundary-lines to be checked by collision-checker
	sf::Vector2f size;					// size in meters
	sf::Vector2f start[2];				// starting position of blue and red players
	sf::Texture texture;
	sf::RectangleShape drawable_object;
};

