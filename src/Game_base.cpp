#include "Game_base.h"

Game_base::Game_base() :
	video_mode(sf::Vector2u(800u, 600u)), window(nullptr), frame_time(0.0f)
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
	video_mode = sf::VideoMode(sf::Vector2u(width_, height_));
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

/***********************/
/* private functions */
/***********************/

void Game_base::poll_events()
{
    while (const auto event = window->pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window->close();
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->code == sf::Keyboard::Key::Escape)
                window->close();
        }

        event->visit([this](const auto& subtype) {
            auto it = handlers.find(std::type_index(typeid(subtype)));
            if (it != handlers.end())
            {
                sf::Event ev(subtype);
                (this->*(it->second))(ev);
            }
        });
    }
}
