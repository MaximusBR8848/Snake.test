#include "Dependencies.hpp"

const char IDLE = 4;
const char DEAD = 5;
const int TILE_SIZE = 60;
const int WIDTH = 1920;
const int HEIGHT = 1080;
float MOVE_TIMER = 145;
unsigned int score = 0, hscore = 0;
unsigned int FOOD_CAPACITY = 3;
int direction = IDLE, SnakeSize = 1, lives = 3, new_direction = -1;

bool fullscreen = true;
bool is_paused = false;
bool gameover = false;
bool is_dead = false;
bool inserting_name = true;
bool has_won = false;
bool moved = false;

std::vector <sf::RectangleShape> Segments;
std::vector <sf::RectangleShape> Foods;
std::vector <sf::Text> PTEXT;

sf::Color outline(46, 46, 46);
sf::Color cyan(114, 137, 218);
sf::Color lime(0, 255, 0);

sf::RectangleShape Head(sf::Vector2f(TILE_SIZE, TILE_SIZE));
sf::RectangleShape segment(sf::Vector2f(TILE_SIZE, TILE_SIZE));
sf::RectangleShape Food(sf::Vector2f(TILE_SIZE, TILE_SIZE));
sf::RectangleShape BackOpac(sf::Vector2f(WIDTH, HEIGHT));

sf::Text Lives;
sf::Text Score;
sf::Text MiddleText;
sf::Text PauseText;
sf::Text HISCORE;
sf::Text Patterntext;

sf::RenderWindow window;
sf::Mouse Mouse;
sf::Event e;

class Input
{
protected:

	std::string InputText = "";
	char m_Char = 0;

public:

	void get_Input()
	{
		if (e.type == e.TextEntered)
		{
			if (e.text.unicode < 128)
			{
				if (e.text.unicode != 8 && InputText.size() < 4)
				{
					m_Char = e.text.unicode;
					InputText += std::to_string(m_Char);
				}
				else if (e.text.unicode == 8 && !InputText.empty())
				{
					InputText.pop_back();
				}
			}
		}

		PTEXT[0].setString(InputText);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			inserting_name = false;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			Input::Finish_Game();
		}
	}

	void Finish_Game()
	{
		Segments.clear();
		PTEXT.clear();

		window.close();

	}
};

class SnakeProprerties
{
private:

	sf::Sound GameOverSound, EatSound;
	sf::SoundBuffer EatBuffer, GameOverBuffer;

public:

	void init_Sounds()
	{
		EatBuffer.loadFromFile("stop-13692.ogg");
		GameOverBuffer.loadFromFile("GameOver.ogg");

		EatSound.setBuffer(EatBuffer);
		GameOverSound.setBuffer(GameOverBuffer);
	}

	void set_Direction()
	{

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && direction != 2 && moved == false ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::W) && direction != 2 && moved == false)
		{
			direction = 0;
			moved = true;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && direction != 3 && moved == false ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::D) && direction != 3 && moved == false)
		{
			direction = 1;
			moved = true;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && direction != 0 && moved == false ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::S) && direction != 0 && moved == false)
		{
			direction = 2;
			moved = true;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && direction != 1 && direction != 4 && moved == false ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::A) && direction != 1 && direction != 4 && moved == false)
		{
			direction = 3;
			moved = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && direction != 2 && moved == true ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::W) && direction != 2 && moved == true)
		{
			new_direction = 0;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && direction != 3 && moved == true ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::D) && direction != 3 && moved == true)
		{
			new_direction = 1;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && direction != 0 && moved == true ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::S) && direction != 0 && moved == true)
		{
			new_direction = 2;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && direction != 1 && direction != 4 && moved == true ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::A) && direction != 1 && direction != 4 && moved == true)
		{
			new_direction = 3;
		}
	}

	void Move()
	{
		moved = false;

		switch (direction)
		{

		case 0: Head.move(0, -TILE_SIZE); break;
		case 1: Head.move(TILE_SIZE, 0); break;
		case 2: Head.move(0, TILE_SIZE); break;
		case 3: Head.move(-TILE_SIZE, 0); break;
		default:break;

		direction = new_direction;

		}
	}

	void FoodRespawn()
	{
		if (FOOD_CAPACITY > 15) FOOD_CAPACITY = 1;

		for (int i = 0; i < FOOD_CAPACITY; i++)
		{
			Foods.push_back(Food);
		}

		std::random_device randomposx;
		std::random_device randomposy;

		std::uniform_int_distribution<int> posx(0, 31);
		std::uniform_int_distribution<int> posy(0, 17);

		for (int i = 0; i < FOOD_CAPACITY; i++)
		{
			Foods[i].setPosition(posx(randomposx) * TILE_SIZE, posy(randomposy) * TILE_SIZE);
		}

		/*
		PTEXT.clear();
		for (int i = 0; i < FOOD_CAPACITY; i++)
		{
			PTEXT.push_back(Patterntext);
			PTEXT[i].setString(std::to_string(i + 1));
			PTEXT[i].setPosition(Foods[i].getPosition());
		}
		*/
	}

	void Grow()
	{
		
		Foods.clear();
		FoodRespawn();
		SnakeSize++;
		score += 100;
		EatSound.play();

		Score.setString("SCORE: " + std::to_string(score));
		if (score > hscore)
		{
			hscore = score;
			HISCORE.setString(("HI SCORE: ") + std::to_string(hscore));
		}
	}

	void Collision()
	{
		for (int i = 0; i < SnakeSize; i++)
			if (Head.getPosition() == Segments[i].getPosition() && direction < IDLE)
			{
				direction = DEAD;
				new_direction = -1;
				score = 0;
				lives--;
				Lives.setString("LIVES: " + std::to_string(lives));
				Score.setString("SCORE: 000");
				is_dead = true;
				Foods.clear();
			}
	}

	void Update()
	{

		if (lives <= 0)
		{
			GameOverSound.play();
			gameover = true;
		}
		if (direction > 3 && direction != DEAD) Segments[0].setPosition(Head.getPosition() - sf::Vector2f(60, 0));
		if (score >= 345600) has_won = true;//345600
		if (direction > 3) MiddleText.setString("PRESS RIGHT, UP OR DOWN COMMANDS TO START");
		else MiddleText.setString("");

		//passing through the screen
		if (Head.getPosition().x < 0)
			Head.setPosition(sf::Vector2f(Head.getPosition().x + WIDTH, Head.getPosition().y));
		else if (Head.getPosition().x > WIDTH - TILE_SIZE)
			Head.setPosition(sf::Vector2f(Head.getPosition().x - WIDTH, Head.getPosition().y));
		else if (Head.getPosition().y > HEIGHT - TILE_SIZE)
			Head.setPosition(sf::Vector2f(Head.getPosition().x, Head.getPosition().y - HEIGHT));
		else if (Head.getPosition().y < 0)
			Head.setPosition(sf::Vector2f(Head.getPosition().x, Head.getPosition().y + HEIGHT));

		if (is_paused == true)
		{
			BackOpac.setPosition(sf::Vector2f(0, 0));
			PauseText.setString("PAUSED");
			window.setMouseCursorVisible(true);
			window.setMouseCursorGrabbed(false);
		}
		else
		{
			PauseText.setString("");
			BackOpac.setPosition(sf::Vector2f(WIDTH, HEIGHT));
			window.setMouseCursorVisible(false);
			window.setMouseCursorGrabbed(true);
		}

		//in case food spawns on a occupied tile
		for (int i = 0; i < SnakeSize; i++)
			for (int I = 0; I < FOOD_CAPACITY; I++)
			{
				{
					if (Segments[i].getPosition() == Foods[I].getPosition())
					{
						FoodRespawn();
					}
				}
			}
	}

	void Respawn()
	{
		for (int i = 0; i < FOOD_CAPACITY; i++)
		{
			Foods[i].setPosition(-TILE_SIZE, -TILE_SIZE);
		}
		SnakeSize = 1;
		score = 0;
		Score.setString("SCORE: 000");
		is_dead = false;
		Head.setPosition(SNAKEBASEPOS);
		Head.setFillColor(sf::Color(lime));
		Foods.push_back(Food);
		Foods[0].setPosition(FOODBASEPOS);
		MiddleText.setPosition(410, 100);
		MiddleText.setString("PRESS RIGHT, UP OR DOWN KEY TO START");
		Segments.clear();
		Segments.push_back(segment);
		direction = IDLE;
		MOVE_TIMER = 145;
	}

	void Restart()
	{
		Respawn();
		inserting_name = true;
		gameover = false;
		has_won = false;
		lives = 3;
		Lives.setString("LIVES: " + std::to_string(lives));
	}

};

//objects
SnakeProprerties Snake;
Input User;

int main() {

	Snake.init_Sounds();

	//Head
	Head.setPosition(SNAKEBASEPOS);
	Head.setFillColor(sf::Color(lime));
	Head.setOutlineThickness(-4);
	Head.setOutlineColor(sf::Color(outline));

	//window
	window.create(sf::VideoMode(WIDTH, HEIGHT), WINDOW, sf::Style::Fullscreen);
	window.setVerticalSyncEnabled(true);
	window.setMouseCursorVisible(false);
	window.setMouseCursorGrabbed(true);
	window.setKeyRepeatEnabled(false);

	//segments
	segment.setFillColor(sf::Color(lime));
	segment.setOutlineThickness(-4);
	segment.setOutlineColor(sf::Color(outline));
	Segments.push_back(segment);

	//background opacity
	BackOpac.setFillColor(sf::Color(1, 1, 1, 140));
	BackOpac.setPosition(sf::Vector2f(1920, 1080));

	//Food
	Food.setFillColor(sf::Color::Red);
	Food.setOutlineThickness(-4);
	Food.setOutlineColor(sf::Color(outline));
	Foods.push_back(Food);
	Foods[0].setPosition(FOODBASEPOS);
	Food.setPosition(sf::Vector2f(-TILE_SIZE, -TILE_SIZE));

	//Font
	sf::Font PixelFont;
	PixelFont.loadFromFile("dogicabold.ttf");

	//Text -> Font
	MiddleText.setFont(PixelFont);
	PauseText.setFont(PixelFont);
	Score.setFont(PixelFont);
	HISCORE.setFont(PixelFont);
	Lives.setFont(PixelFont);
	Patterntext.setFont(PixelFont);

	//strings
	Score.setString("SCORE: 000");
	HISCORE.setString("HI SCORE: 000");
	Lives.setString("LIVES: " + std::to_string(lives));

	//char size
	PauseText.setCharacterSize(30);
	Score.setCharacterSize(24);
	HISCORE.setCharacterSize(24);
	Lives.setCharacterSize(24);
	Patterntext.setCharacterSize(24);

	//text position
	MiddleText.setPosition(410, 100);
	PauseText.setPosition(sf::Vector2f(865, 200));
	Score.setPosition(LEFT_TOP_SPACING);
	HISCORE.setPosition(Score.getPosition() + sf::Vector2f(0, MIN_SPACING_BET_TEXT));
	Lives.setPosition(HISCORE.getPosition() + sf::Vector2f(0, MIN_SPACING_BET_TEXT));

	//arranging the scoreboard
	/*
	for (int i = 0; i < 11; i++)
	{
		PTEXT.push_back(Patterntext);
		if(i < 1) PTEXT[i].setPosition(WIDTH - 250, HEIGHT - 1045);
		else
		{
			PTEXT[i].setString("MAX - 000");
			PTEXT[i].setPosition(PTEXT[i - 1].getPosition().x,PTEXT[i - 1].getPosition().y + MIN_SPACING_BET_TEXT);
		}
	}

	PTEXT[0].setString("SCORES:");
	PTEXT[0].setPosition(sf::Vector2f(PTEXT[0].getPosition().x + 30, PTEXT[0].getPosition().y));
	*/

	//clock inicialization
	sf::Clock clock;
	sf::Clock menu_timer;

	//C++ window with exit and toggle screen mode
	while (window.isOpen())
	{
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				User.Finish_Game();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F11) && menu_timer.getElapsedTime().asMilliseconds() >= 2000)
			{

				menu_timer.restart();

				fullscreen = !fullscreen;

				switch (fullscreen)
				{

				case 0: 
					
					window.close();
					window.create(sf::VideoMode(WIDTH, HEIGHT), WINDOW, sf::Style::Fullscreen);
					window.setVerticalSyncEnabled(true);
					break;

				case 1: 
					
					window.close();
					window.create(sf::VideoMode(WIDTH, HEIGHT), WINDOW);
					window.setVerticalSyncEnabled(true);
					break;

				}
			}
		}

		//inicialization and name set
		/*
		while (inserting_name == true)
		{

			User.get_Input();

			window.clear(sf::Color::Black);
			for (auto& Patterntext : PTEXT)
			{
				window.draw(Patterntext);
			}
			window.display();
		}
		*/

		//pause system check
		if (is_paused == false)
		{

			if (!is_dead || !gameover) Snake.set_Direction();

			//movement key recognition
			if (clock.getElapsedTime().asMilliseconds() >= MOVE_TIMER)
			{

				segment.setPosition(Head.getPosition());
				Segments.push_back(segment);

				if (SnakeSize < Segments.size())
				{
					Segments.erase(Segments.begin());
				}

				Snake.Move();
				Snake.Collision();
				clock.restart();

			}

			//eat it
			for (int i = 0; i < Foods.size(); i++)
			{
				if (Head.getPosition() == Foods[i].getPosition())
				{
					if(MOVE_TIMER > 75) MOVE_TIMER -= 5;
					Snake.Grow();
				}
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && direction < IDLE && direction != -1)
		{
			is_paused = !is_paused;
			sf::sleep(sf::Time(sf::seconds(0.25)));
			Mouse.setPosition(sf::Vector2i(960, 540));
		}

		//dead, game over or won
		while (is_dead == true)
		{
			for (int i = 0; i < SnakeSize; i++)
			{
				Segments[i].setFillColor(sf::Color(cyan));
			}

			MiddleText.setPosition(820, 100);
			MiddleText.setString("TRY AGAIN!");

			window.clear();
			for (auto& segment : Segments)
			{
				window.draw(segment);
			}
			window.draw(MiddleText);
			window.display();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
			{
				Snake.Respawn();
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				User.Finish_Game();
			}
		}

		while (gameover == true)
		{
			MiddleText.setString("GAME OVER");
			MiddleText.setPosition(820, 516);
			direction = IDLE;

			window.clear();
			window.draw(MiddleText);
			window.display();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
			{
				if (clock.getElapsedTime().asMilliseconds() >= 4000)
				{
					Snake.Restart();
					clock.restart();
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				User.Finish_Game();
			}
		}

		while (has_won == true)
		{
			MiddleText.setString("YOU WON!");
			MiddleText.setPosition(852, 510);
			direction = IDLE;

			for (int i = 0; i < SnakeSize; i++)
			{
				Segments[i].setFillColor(sf::Color::Yellow);
			}
			Head.setFillColor(sf::Color::Yellow);

			window.clear();
			for (auto& segment : Segments)
			{
				window.draw(segment);
			}
			window.draw(Head);
			window.draw(MiddleText);
			window.display();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && clock.getElapsedTime().asMilliseconds() >= 4000)
			{
				Snake.Restart();
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				User.Finish_Game();
			}
		}

		Snake.Update();

		//space for tests

		window.clear();
		window.draw(Head);
		for (auto& segment : Segments)
		{
			window.draw(segment);
		}
		for (auto& Food : Foods)
		{
			window.draw(Food);
		}
		window.draw(MiddleText);
		window.draw(Score);
		window.draw(HISCORE);
		window.draw(Lives);
		window.draw(BackOpac);
		for (auto& Patterntext : PTEXT)
		{
			window.draw(Patterntext);
		}
		window.draw(PauseText);
		window.display();
	}
}