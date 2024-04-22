#ifndef GAME_BASE_H_
#define GAME_BASE_H

#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

/**
 * Class that acts as game engine.
 * Wrapper for SFML-based game
 */
class Game_base
{
public:

	typedef void (Game_base::* event_handler_t)(sf::Event&);

	Game_base();
	virtual ~Game_base();

	// getters
	inline bool running() const { return window->isOpen(); }

	/**
	 * \return	\b [seconds] Duration of last frame,
	 *			or time from instance construction if no one frame was drawn.
	 */
	inline float get_frame_time() const { return frame_time; }

	// public functions
	void init_window(int width_ = 800, int height_ = 600, std::string label_ = "game");
	void update();
	void render();

protected:

	// protected variables
	sf::VideoMode video_mode;
	sf::RenderWindow* window;
	sf::Clock clock;

	// protected functions
	bool add_event_handler(sf::Event::EventType ev_type_, event_handler_t h_);

	// virtual protected functions
	virtual void do_calcs() = 0;
	virtual void draw_game() = 0;

private:

	// private variables
	float frame_time;
	std::map<sf::Event::EventType, event_handler_t> handlers;

	// private functions
	void poll_events();

};

#endif // GAME_H

