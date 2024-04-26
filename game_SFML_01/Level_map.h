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
	 * \param texture_file	path to texture image file
	 */
	void init(std::string texture_file);

	/**
	 * Draws map texture to given position in window.
	 * 
	 * \param window	window	window instance
	 * \param view_pos	position of bottom-left corner of view-port 
	 *					(visible part of map) on the map in meters
	 */
	void draw(sf::RenderWindow& window, sf::Vector2f view_pos);

	/**
	 * \return			Total size of map in meters.
	 */
	inline sf::Vector2f get_size() const { return size; }

	/**
	 * \return			Read-only vector of all map boundary lines.
	 */
	inline const std::vector<Bound_box>& get_boundaries() const { return boundaries; }

private:

	std::vector<Bound_box> boundaries;	// vector of boundary-lines to be checked by collision-checker
	sf::Vector2f size;					// size in meters
	sf::Texture texture;
	sf::RectangleShape drawable_object;
};

