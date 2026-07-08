#ifndef VANESSA_GAME_H_
#define VANESSA_GAME_H_

#include <array>
#include <memory>
#include <vector>
#include "constants.h"
#include "Defs.h"
#include "Game_base.h"
#include "Player.h"
#include "Level_map.h"

class Vanessa_game : public Game_base
{
public:

	enum Key_state
	{
		released,	///< key is not pressed
		pressed,	///< key just was pressed
		comfirmed	///< key is still pressed but this state was read & used
	};

	enum Key_function
	{
		p0_up = 0, p0_down, p0_left, p0_right,
		p1_up, p1_down, p1_left, p1_right,
		key_function_length
	};

	/** \note std::array even with for loops in ev.handlers are probably faster than std::map */
	std::array<sf::Keyboard::Key, key_function_length> key_shortcuts;
	std::array<Key_state, key_function_length> key_states;

	void init();

protected:

	// protected variables
	sf::Vector2f viewport_pos;
	std::vector<Player> players;
	std::unique_ptr<Level_map> map;
	int current_map;


	// protected functions
	void do_calcs() override;
	void draw_game() override;

	// loads map by number, reinits players
	void load_map(int num);

	// event handlers
	void key_press_handler(sf::Event& ev_);
	void key_release_handler(sf::Event& ev_);
};

#endif // VANESSA_GAME_H_