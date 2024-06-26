#pragma once

#include <string>
#include <vector>
#include <bitset>
#include <SFML/Graphics.hpp>
#include "Boundary.h"

class Player
{
public:

	enum Color {blue, red};

	Player();

	/**
	 * Initializes player and loads texture.
	 * 
	 * \param position		position on map in metters
	 * \param texture_file	path to texture image file
	 */
	void init(sf::Vector2f position, std::string texture_file, Color color);

	/**
	 * Updates position and speed of all elements in vector.
	 *
	 * \note	Uses vector, rather than calling upsate method on each element.
	 *			Doing this saves lot of branches and probably better fits cache.
	 * \param players		all players to be updated
	 * \param delta_time	time elapsed from last update
	 */
	static void update(std::vector<Player>& players, const std::vector<Bound_box>& map_bounds, float delta_time);

	/**
	 * Draws all elements in vector.
	 * 
	 * \param player	all players to draw
	 * \param window	window instance
	 * \param view_pos	position of bottom-left corner of view-port 
	 *					(visible part of map) on the map in meters
	 */
	static void draw(std::vector<Player>& players, sf::RenderWindow& window, sf::Vector2f view_pos);

	/**
	 * Set by collision-checker.
	 * 
	 * \param on_ground_ true if player is standing on the ground, false = free fall.
	 */
	void set_on_ground(bool on_ground_) { on_ground = on_ground_; }

	/**
	 * Sets flags for horizontal movement.
	 * Actual moves are done in update() regarding to boundaries (wall, rib ...)
	 * 
	 * \param move		false = stop horizontal move
	 * \param right		true = move to the right, false = move to the left
	 */
	void horizontal_move(bool move, bool right = true)
	{
		move_flags[Move_flag_idxs::left] = move && not right;
		move_flags[Move_flag_idxs::right] = move && right;
	}

	/**
	 * Sets flags for vertical movement.
	 * Actual moves are done in update() regarding to boundaries (wall, rib ...)
	 * 
	 * \param move		false = stop vertical movement / acceleration
	 * \param up		true = move up, false = move down
	 * \param jump		true only when "up" key was released and now is pressed (rising-edge)
	 */
	void vertical_move(bool move, bool up = true, bool jump = false)
	{
		move_flags[Move_flag_idxs::up] = move && up;
		move_flags[Move_flag_idxs::down] = move && not up;
		move_flags[Move_flag_idxs::jump] = jump;
	}

	/**
	 * \return			position in meters relative to map's bottom left corner.
	 */
	inline sf::Vector2f get_position() const { return position; }

	/**
	 * \return			true if player is still alive.
	 */
	inline bool get_alive() const { return alive; }

private:

	Color color;
	bool alive;				// by default its true, set to false if player die
	bool on_ground;			// true if standing on groud (not on rib) -> used for jumping
	float mass;				// in kilograms
	sf::Vector2f size;		// size in meters
	sf::Vector2f position;  // position of bottom-center in merers
	sf::Vector2f speed;		// m * s^-1
	sf::Vector2f accel;		// m * s^-2
	//sf::Vector2f jerk;		// m * s^-3

	enum Move_flag_idxs
	{
		up = 0, down, jump, left, right, Move_flag_idxs_size
	};
	std::bitset<Move_flag_idxs_size> move_flags;

	sf::Texture texture;
	sf::RectangleShape drawable_object;

};

