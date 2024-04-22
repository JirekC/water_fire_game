#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

class Player
{
public:

	Player();

	/**
	 * Initializes player and loads texture.
	 * 
	 * \param position		position on map in metters
	 * \param texture_file	path to texture image file
	 */
	void init(sf::Vector2f position, std::string texture_file);

	/**
	 * Updates position and speed of all elements in vector.
	 *
	 * \note	Uses vector, rather than calling upsate method on each element.
	 *			Doing this saves lot of branches and probably better fits cache.
	 * \param players		all players to be updated
	 * \param delta_time	time elapsed from last update
	 */
	static void update(std::vector<Player>& players, float delta_time);

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
	 * For simplicity - skip acceleration phase and give some vertical speed to the object.
	 */
	void jump();

	/**
	 * Set by collision-checker.
	 * \param on_ground_ true if player is standing on the ground, false = free fall.
	 */
	void set_on_ground(bool on_ground_) { on_ground = on_ground_; }

	/**
	 * For simplicity - skip acceleration / deceleration phase and give constant speed.
	 * \param move		false = stop horizontal move
	 * \param right		true = move to the right, false = move to the left
	 */
	void horizontal_move(bool move, bool right = true);

private:

	bool on_ground;			// true if standing on groud, false = free fall
	float mass;				// in kilograms
	sf::Vector2f size;		// size in meters
	sf::Vector2f position;  // position of bottom-center in merers
	sf::Vector2f speed;		// m * s^-1
	sf::Vector2f accel;		// m * s^-2
	//sf::Vector2f jerk;		// m * s^-3

	sf::Texture texture;
	sf::RectangleShape drawable_object;

};

