#include "Player.h"
#include "constants.h"

Player::Player() :
	on_ground(false), mass(1.0f), size(0.0, 0.0),
	position(0.0, 0.0), speed(0.0, 0.0), accel(0.0, 0.0)//, jerk(0.0, 0.0)
{
}

void Player::init(sf::Vector2f position, std::string texture_file)
{
	texture.loadFromFile(texture_file);
	drawable_object.setTexture(&texture, true);
	auto tex_size = drawable_object.getTexture()->getSize();
	drawable_object.setSize(static_cast<sf::Vector2f>(tex_size)); // resize according to texture
	size.x = (float)tex_size.x / pixels_per_meter;
	size.y = (float)tex_size.y / pixels_per_meter;

	this->position = position;
}

void Player::update(std::vector<Player>& players, float delta_time)
{
	for (auto& p : players)
	{
		p.accel.y = p.on_ground ? 0.f : gravity_acc;
		p.accel.x = 0.0f; // for now we are setting horizontal speed directly by "<-", "->" arrow key_shortcuts
		p.speed += p.accel * delta_time; // add gravity as offset to acceleration
		if (p.on_ground && p.speed.y < 0.f)
		{
			p.speed.y = 0.0f;
		}
		p.position += p.speed * delta_time;
		if (p.position.y <= 0.f)
		{
			p.position.y = 0.f;
			p.on_ground = true;
		}
	}
}

void Player::draw(std::vector<Player>& players, sf::RenderWindow& window, sf::Vector2f view_pos)
{
	auto win_size_m = sf::Vector2f(window.getSize()) / pixels_per_meter; // window size in meters

	for (auto& p : players)
	{
		auto pos_on_screen = p.position - view_pos;
		pos_on_screen.x -= p.size.x * 0.5f; // center ->
		pos_on_screen.y += p.size.y; // <- bottom corner
		pos_on_screen.y = win_size_m.y - pos_on_screen.y; // flip Y coordinate
		p.drawable_object.setPosition(pos_on_screen * pixels_per_meter); // recalc to pixels & update drawable_object
		window.draw(p.drawable_object); // draw it
	}
}

void Player::jump()
{
	if (on_ground)
	{ 
		speed.y += jump_speed;
		on_ground = false;
	}
}

void Player::horizontal_move(bool move, bool right)
{
	if (move)
	{
		speed.x = right ? hmove_speed : -hmove_speed;
	}
	else
	{
		speed.x = 0.0f;
	}
}

