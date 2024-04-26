#include "Game_base.h"

Game_base::Game_base() :
	video_mode(800, 600), window(nullptr), frame_time(0.0f)
{
}

Game_base::~Game_base()
{
	if (window) delete window;
}

/***********/
/* Getters */
/***********/

//////////////////////////

/********************/
/* public functions */
/********************/

void Game_base::init_window(int width_, int height_, std::string label_)
{
	video_mode.width = width_;
	video_mode.height = height_;
	window = new sf::RenderWindow(video_mode, label_, sf::Style::Titlebar | sf::Style::Close);
    window->setFramerateLimit(144);
    clock.restart();
}

void Game_base::update()
{
    poll_events();

    frame_time = clock.getElapsedTime().asSeconds();
    clock.restart();
    do_calcs();
}

void Game_base::render()
{
    window->clear(); // clear old stuff

    draw_game();

    window->display();
}

/***********************/
/* protected functions */
/***********************/

bool Game_base::add_event_handler(sf::Event::EventType ev_type_, event_handler_t h_)
{
    handlers[ev_type_] = h_; // TODO: some checks

    return true;
}

/***********************/
/* private functions */
/***********************/

void Game_base::poll_events()
{
    sf::Event ev;
    while (window->pollEvent(ev)) // if we have event, store it to "ev" and return true
    {
        if (ev.type == sf::Event::Closed) // click on "X" in window
        {
            window->close();
        }
        else if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape)
        {
            window->close(); 
        }
        else
        {
            //event_reaction(ev);
            auto search = handlers.find(ev.type);
            if (search != handlers.end())
            {
                auto ptr = search->second; // iterator is pointing to <key, value> pair
                (this->*ptr)(ev);
            }
        }
    }
}
