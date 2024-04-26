#include <iostream>
#include "Vanessa_game.h"

/********************/
/* public functions */
/********************/

void Vanessa_game::init()
{
	// init players
	players.clear();
	players.push_back(std::move(Player()));
	players.push_back(std::move(Player()));
	players[0].init(sf::Vector2f(1.5f, 0.f), "data/water.png");
	players[1].init(sf::Vector2f(1.0f, 0.f), "data/fire.png");

	// keyboard - hardcoded for now
	key_shortcuts = { sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right,
			 sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::A, sf::Keyboard::D };
	for (auto& k : key_states)
	{
		k = Key_state::released;
	}
	add_event_handler(sf::Event::KeyPressed, static_cast<Game_base::event_handler_t>(&Vanessa_game::key_press_handler));
	add_event_handler(sf::Event::KeyReleased, static_cast<Game_base::event_handler_t>(&Vanessa_game::key_release_handler));

	// first map
	map.init("data/map01.png");
}

/***********************/
/* protected functions */
/***********************/

void Vanessa_game::do_calcs()
{
	// keyboard actions ->
	// player 0
	if (key_states[Key_function::p0_up] == pressed)
	{
		key_states[Key_function::p0_up] = comfirmed;
		players[0].jump();
	}
	
	if (key_states[Key_function::p0_right] == pressed &&
		key_states[Key_function::p0_left] != pressed)
	{
		players[0].horizontal_move(true, true);
	}
	else if (key_states[Key_function::p0_left] == pressed &&
		     key_states[Key_function::p0_right] != pressed)
	{
		players[0].horizontal_move(true, false);
	}
	else
	{
		players[0].horizontal_move(false);
	}

	// player 1
	if (key_states[Key_function::p1_up] == pressed)
	{
		key_states[Key_function::p1_up] = comfirmed;
		players[1].jump();
	}

	if (key_states[Key_function::p1_right] == pressed &&
		key_states[Key_function::p1_left] != pressed)
	{
		players[1].horizontal_move(true, true);
	}
	else if (key_states[Key_function::p1_left] == pressed &&
		     key_states[Key_function::p1_right] != pressed)
	{
		players[1].horizontal_move(true, false);
	}
	else
	{
		players[1].horizontal_move(false);
	}
	// <- keyboard actions

	// collision check
	// ........ by game_map class ?

	// kinematics
	Player::update(players, map.get_boundaries(), get_frame_time());

	// update viewport position
	auto win_size_m = sf::Vector2f(window->getSize()) / pixels_per_meter; // window size in meters
	viewport_pos = (players[0].get_position() + players[1].get_position()) * 0.5f;
	viewport_pos -= win_size_m * 0.5f;
	if (viewport_pos.x < 0.f) viewport_pos.x = 0.f;
	if (viewport_pos.y < 0.f) viewport_pos.y = 0.f;
	if (viewport_pos.x > (map.get_size().x - win_size_m.x)) viewport_pos.x = map.get_size().x - win_size_m.x;
	if (viewport_pos.y > (map.get_size().y - win_size_m.y)) viewport_pos.y = map.get_size().y - win_size_m.y;

}

void Vanessa_game::draw_game()
{
	map.draw(*window, viewport_pos);
	Player::draw(players, *window, viewport_pos);
}

/******************/
/* event handlers */
/******************/

void Vanessa_game::key_press_handler(sf::Event& ev_)
{
	for (int i = 0; i < key_shortcuts.size(); i++)
	{
		if (ev_.key.code == key_shortcuts[i])
		{
			if (key_states[i] == released)
			{
				key_states[i] = Key_state::pressed;
			}
			break; // for
		}
	}
}

void Vanessa_game::key_release_handler(sf::Event& ev_)
{
	for (int i = 0; i < key_shortcuts.size(); i++)
	{
		if (ev_.key.code == key_shortcuts[i])
		{
			key_states[i] = Key_state::released;
			break; // for
		}
	}
}
