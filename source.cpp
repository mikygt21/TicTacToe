#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#define WIDTH 1280
#define HEIGHT 720

#define FRAME_WITH 96
#define FRAME_HEIGHT 101
#define NUM_FRAMES 10
#define ANIMATION_SPEED 0.1

void UpdateSprite(sf::Sprite& animatedSprite, int& currentFrame, float& deltatimeAnim)
{
	if (deltatimeAnim >= ANIMATION_SPEED)
	{
		currentFrame = (currentFrame + 1) % NUM_FRAMES;
		animatedSprite.setTextureRect(sf::IntRect({ currentFrame * FRAME_WITH, 0 }, { FRAME_WITH, FRAME_HEIGHT }));
	}
}

sf::Texture LoadSpriteSheet(const std::string& filepath)
{
	sf::Texture texture;
	if (!texture.loadFromFile(filepath))
	{
		std::cerr << "Error loading spritesheet" << std::endl;
	}
	return texture;
}

void Render(sf::RenderWindow& window, sf::RectangleShape& s)
{
	window.clear(sf::Color(0x000000FF));
	window.draw(s);
	window.display();
}

void Render(sf::RenderWindow& window, sf::Sprite& s)
{
	window.clear(sf::Color(0x000000FF));
	window.draw(s);
	window.display();
}

void HandleEvent(const sf::Event& event, sf::RenderWindow& window)
{
	if (event.is<sf::Event::Closed>())
	{
		window.close();
	}

	if (const sf::Event::KeyPressed* keyPressed = event.getIf<sf::Event::KeyPressed>())
	{
		switch (keyPressed->code)
		{
		case sf::Keyboard::Key::Escape:
			window.close();
			break;
		default:
			break;
		}
	}

	if (const sf::Event::MouseButtonPressed* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
	{
		switch (mousePressed->button)
		{
		case sf::Mouse::Button::Left:
			std::cout << "Click at: " << mousePressed->position.x << ", " << mousePressed->position.y << std::endl;
			break;
		default:
			break;
		}
	}
}


void main()
{
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ WIDTH, HEIGHT }), "SFML");

	sf::Clock deltaTimeClock;
	float deltaTimeAnimation = 0.f;
	int currentFrame = 0;


	/*sf::RectangleShape square;
	square.setSize({ 100.f, 100.f });
	square.setFillColor(sf::Color::Green);
	square.setPosition({WIDTH * 0.5F - square.getSize().x * 0.5f, HEIGHT * 0.5f - square.getSize().y * 0.5f});*/

	sf::Texture ssTexture = LoadSpriteSheet("Assets/spritesheets/S_Link.png");
	sf::Sprite animatedSprite = sf::Sprite(ssTexture);

	sf::SoundBuffer buffer("Assets/audio/FX_Quack.wav");
	sf::Sound sound(buffer);
	sound.play();

	while (window->isOpen())
	{
		float deltatime = deltaTimeClock.restart().asSeconds();
		deltaTimeAnimation += deltatime;

		while (const std::optional event = window->pollEvent())
		{
			HandleEvent(*event, *window);
		}
		UpdateSprite(animatedSprite, currentFrame, deltaTimeAnimation);
		//Render(*window, square);
		Render(*window, animatedSprite);
	}

	delete window;
}