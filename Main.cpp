#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/system.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <vector>

using namespace std;

int main() {

#define WIDTH 1920
#define HEIGHT 1080
#define IWAIT 40
#define MOVT 9
#define WINDOWN "Snake BETA 1.0"

	sf::RenderWindow window;
	window.create(sf::VideoMode(WIDTH, HEIGHT), WINDOWN, sf::Style::Fullscreen);
	window.setVerticalSyncEnabled(true);

	//color of outline
	sf::Color outline(46, 46, 46);

	std::vector <sf::RectangleShape> Segments;

	sf::RectangleShape segment(sf::Vector2f(60,60));
	segment.setFillColor(sf::Color::Green);
	segment.setOutlineThickness(-4);
	segment.setOutlineColor(sf::Color(outline));

	sf::RectangleShape SnakeHead(sf::Vector2f(60, 60));
	SnakeHead.setPosition(840, 540);
	SnakeHead.setFillColor(sf::Color::Green);
	SnakeHead.setOutlineThickness(-4);
	SnakeHead.setOutlineColor(sf::Color(outline));

	sf::RectangleShape BackOpac(sf::Vector2f(1920, 1080));
	BackOpac.setFillColor(sf::Color(1, 1, 1, 140));
	BackOpac.setPosition(sf::Vector2f(1920, 1080));

	sf::RectangleShape Food(sf::Vector2f(60, 60));
	Food.setPosition(1020, 540);
	Food.setFillColor(sf::Color::Red);
	Food.setOutlineThickness(-4);
	Food.setOutlineColor(sf::Color(outline));

	window.setMouseCursorVisible(false);
	window.setMouseCursorGrabbed(true);
	
	int direction = 5;
	int lives = 3;
	int score = 0, p_score = 0, hscore = 0, SnakeSize = 1;
	int I_wait = IWAIT;
	int mov_t = MOVT;
	bool fullscreen = true;
	bool is_paused = false;
	bool gameover = false;

	sf::Font PixelFont;
	PixelFont.loadFromFile("dogicabold.ttf");
	
	sf::Text MiddleText;
	MiddleText.setFont(PixelFont);
	MiddleText.setString("PRESS RIGHT, UP OR DOWN KEY TO START");
	MiddleText.setPosition(410, 100);

	sf::Text Lives;
	Lives.setFont(PixelFont);
	Lives.setCharacterSize(24);
	Lives.setPosition(10, 78);
	Lives.setString("LIVES: " + std::to_string(lives));

	sf::Text Score;
	Score.setFont(PixelFont);
	Score.setCharacterSize(24);
	Score.setPosition(10, 10);
	Score.setString("SCORE: 000");

	sf::Text PauseText;
	PauseText.setFont(PixelFont);
	PauseText.setCharacterSize(30);
	PauseText.setPosition(sf::Vector2f(865, 200));

	sf::Text HISCORE;
	HISCORE.setFont(PixelFont);
	HISCORE.setCharacterSize(24);
	HISCORE.setPosition(10, 44);
	HISCORE.setString("HI SCORE: 000");

	//avarage c++ window//////////////////////////////////////////////////////////////////////////
	while (window.isOpen()) {
		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F11)) {

			fullscreen = !fullscreen;
			if (fullscreen == true) {

				window.create(sf::VideoMode(WIDTH, HEIGHT), WINDOWN, sf::Style::Fullscreen);
				window.setVerticalSyncEnabled(true);

			}

			else if (fullscreen == false) {

				window.create(sf::VideoMode(WIDTH, HEIGHT), WINDOWN);
				window.setVerticalSyncEnabled(true);

			}
		}

		if (is_paused == false) {

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && direction != 2 && I_wait == IWAIT) {
				direction = 0;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && direction != 3 && I_wait == IWAIT) {
				direction = 1;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && direction != 0 && I_wait == IWAIT) {
				direction = 2;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && direction != 1 && direction != 4 && I_wait == IWAIT) {
				direction = 3;
			}

			if (I_wait < IWAIT) I_wait++;

			if (mov_t == MOVT) {

				mov_t = 0;

				segment.setPosition(SnakeHead.getPosition());
				Segments.push_back(segment);

				if (SnakeSize < Segments.size()) {
					Segments.erase(Segments.begin());
				}

				switch (direction) {

				case 0: SnakeHead.move(0, -60); break;
				case 1: SnakeHead.move(60, 0); break;
				case 2: SnakeHead.move(0, 60); break;
				case 3: SnakeHead.move(-60, 0); break;
				default: break;

				}
			}

			else if (mov_t < MOVT) mov_t++;

			if (SnakeHead.getPosition().x < 0 || SnakeHead.getPosition().x > WIDTH - 60 ||
				SnakeHead.getPosition().y < 0 || SnakeHead.getPosition().y > HEIGHT - 60)
			{
				SnakeHead.setPosition(840, 540);
				direction = 4;
				score = I_wait = 0;
				lives--;
				Lives.setString("LIVES: " + std::to_string(lives));
				Food.setPosition(1020, 540);
				Score.setString("SCORE: 000");
				Segments.clear();
				SnakeSize = 1;
			}

			if (direction < 4) {
				MiddleText.setString("");  
			}

			if(direction == 5) Segments[0].setPosition(SnakeHead.getPosition() - sf::Vector2f(60, 0));

			if (direction == 4) {

				Segments[0].setPosition(SnakeHead.getPosition() - sf::Vector2f(60, 0));

				MiddleText.setPosition(820, 100);
				if (p_score == 0) {
					MiddleText.setString("TRY AGAIN!\n\n\n\n    " + std::to_string(p_score));
				}
				else {
					MiddleText.setString("TRY AGAIN!\n\n\n\n   " + std::to_string(p_score));
				}
			}

			if (SnakeHead.getPosition() == Food.getPosition()) {

				SnakeSize++;

				std::random_device randomposx;
				std::uniform_int_distribution<int> posx(0, 31);

				std::random_device randomposy;
				std::uniform_int_distribution<int> posy(0, 17);

				Food.setPosition(posx(randomposx) * 60, posy(randomposy) * 60);
				score += 100;
				p_score = score;
				Score.setString("SCORE: " + std::to_string(score));
				if (score > hscore) {

					hscore = score;
					HISCORE.setString(("HI SCORE: ") + std::to_string(hscore));

				}
			}
		}

		//pause system
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && direction < 4) {

			is_paused = !is_paused;
			sf::sleep(sf::Time(sf::seconds(0.25)));

		}

		if (is_paused == true && direction < 4) {
			BackOpac.setPosition(sf::Vector2f(0, 0));
			PauseText.setString("PAUSED");
			window.setMouseCursorVisible(true);
			window.setMouseCursorGrabbed(false);
		}
		else {
			PauseText.setString("");
			BackOpac.setPosition(sf::Vector2f(1920, 1080));
			window.setMouseCursorVisible(false);
			window.setMouseCursorGrabbed(true);
		}

		if (lives == 0) gameover = true;

		while(gameover == true){

			MiddleText.setString("GAME OVER");
			MiddleText.setPosition(820, 516);
			direction = 5;
			
			window.clear();
			window.draw(MiddleText);
			window.display();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {

				sf::sleep(sf::Time(sf::seconds(0.3)));
				gameover = false;
				lives = 3;
				Lives.setString("LIVES: " + std::to_string(lives));
				MiddleText.setString("PRESS RIGHT, UP OR DOWN KEY TO START");
				MiddleText.setPosition(410, 100);

			}

		}

		window.clear();
		for (const auto& segment : Segments) {
			window.draw(segment);
		}
		window.draw(SnakeHead);
		window.draw(Food);
		window.draw(MiddleText);
		window.draw(Score);
		window.draw(HISCORE);
		window.draw(Lives);
		window.draw(BackOpac);
		window.draw(PauseText);
		window.display();

	}
}

		/* Plans/To do list:
		* UI updates such as controls, pause menu
		* (optional) Background grid
		* (optional) Player name and scoreboard
		*/