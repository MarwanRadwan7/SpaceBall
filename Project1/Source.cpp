#include <SFML/Graphics.hpp>
#include "SFML/Audio.hpp"
#include "Collision.hpp"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;
using namespace sf;

const int N = 720;
const int M = 720;

bool check(Sprite sprite)
{
	if (sprite.getPosition().x < 0 || sprite.getPosition().x >(N - 100))
		return false;
	return true;
}


int main()
{
	RenderWindow window(VideoMode(720, 720), "SapceY");
	window.setFramerateLimit(60);


	/*    getting High Score from file txt   */
	string highScoreSt;
	ifstream highScoreFile("HighScore.txt");
	int highScoreInt = 0;
	getline(highScoreFile, highScoreSt);
	if (highScoreSt != "") highScoreInt = stoi(highScoreSt);
	highScoreFile.close();


	/*     Game Photos    */
	Texture t1, t2, t3, t4, t5;
	if (!t1.loadFromFile("img/green.png"))
	{
		cout << "Error, Green ball path not found !!!!!!!!" << endl;
	}
	if (!t2.loadFromFile("img/blue.png"))
	{
		cout << "Error, Blue ball path not found !!!!!!!!!" << endl;
	}
	if (!t3.loadFromFile("img/purple.png"))
	{
		cout << "Error, Blue ball path not found !!!!!!!!!" << endl;
	}
	if (!t4.loadFromFile("img/background.png"))
	{
		cout << "Error, Background path not found !!!!!!!!!" << endl;
	}
	if (!t5.loadFromFile("img/space.png"))
	{
		cout << "Error, Space path not found !!!!!!!!!!!" << endl;
	}

	Sprite green, blue, purple, background, space;

	green.setTexture(t1);
	blue.setTexture(t2);
	purple.setTexture(t3);
	background.setTexture(t4);
	space.setTexture(t5);

	green.setPosition(360, 10);
	blue.setPosition(300, 10);

	green.setScale(0.1, 0.1);
	blue.setScale(0.1, 0.1);

	space.setPosition(310, 690);


	/*     Game Music    */
	Music music;
	if (!music.openFromFile("sounds/music.ogg")) {
		cout << "Sound Error\n";
	}
	music.play();
	music.setVolume(35);


	SoundBuffer crashSpace;
	if (!crashSpace.loadFromFile("sounds/crash.wav")) {
		cout << "Sound Error\n";
	}
	Sound sound;
	sound.setBuffer(crashSpace);


	SoundBuffer border;

	if (!border.loadFromFile("sounds/borderCrash.wav")) {
		cout << "Sound Error\n";
	}
	Sound bordercrash;
	bordercrash.setBuffer(border);

	SoundBuffer gamedone;

	if (!gamedone.loadFromFile("sounds/gameOver.wav")) {
		cout << "Sound Error\n";
	}
	Sound gameOver;
	gameOver.setBuffer(gamedone);

	SoundBuffer over;

	if (!over.loadFromFile("sounds/gameovermusic.ogg")) {
		cout << "Sound Error\n";
	}
	Sound gOver;
	gOver.setBuffer(over);

	
	/*     Basic Coordinates of moving Balls    */
	float xstep = 5, ystep = 5;
	float xbstep = 5, ybstep = 5;



	/*     Collision with white space    */
	FloatRect shape1 = green.getGlobalBounds();
	FloatRect shape2 = space.getGlobalBounds();
	float dx = (green.getPosition().x + (shape1.width / 2)) - (space.getPosition().x + (shape2.width / 2));
	float dy = (green.getPosition().y + (shape1.height / 2)) - (space.getPosition().y + (shape2.height / 2));

	if (Collision::PixelPerfectTest(green, space, 0))
	{
		cout << "Colision detected\n";
	}
	else
	{
		cout << " No Colision detected\n";
	}


	/*        Font and Texts      */
	Font font;
	if (!font.loadFromFile("fonts/PoorStory-Regular.ttf"))
	{
		cout << "Font Error" << endl;
	}
	Text Score;
	int score = 0;  // Current score
	int balls = 3; // Number of tries
	Score.setString("Your Score: " + to_string(score) + " Balls : " + to_string(balls));
	Score.setFont(font);
	Score.setString("Your Score: " + to_string(score) + " Balls : " + to_string(balls));
	Score.setPosition(245, 0);
	Score.setFillColor(Color::Cyan);


	/*       Game Window      */
	while (window.isOpen())
	{
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();
			if (e.type == Event::Closed)
				window.close();
			if (e.key.code == Keyboard::D)
			{
				space.move(25, 0);
				if (!check(space))
					space.setPosition(Vector2f(620, 690));

			}
			if (e.key.code == Keyboard::A)
			{
				space.move(-25, 0);
				if (!check(space))
					space.setPosition(Vector2f(0, 690));
			}
		}


		/*     Ball Moving    */
		if (green.getPosition().x > 670) {
			xstep -= 5;
			bordercrash.setVolume(110);
			bordercrash.play();
		}
		else if (green.getPosition().x < 0) {
			xstep = 7;
			bordercrash.setVolume(110);
			bordercrash.play();
		}
		else if (green.getPosition().y > 680) {
			balls--;
			Score.setString("Your Score: " + to_string(score) + " Balls : " + to_string(balls));
			if (balls <= 0) {
				Score.setString("Game Over");

				/*     Game Over    */
				window.close();

				if (score > highScoreInt) {

					ofstream fout("HighScore.txt");
					fout << score << endl;
					highScoreInt = score;
					fout.close();
				}
				music.stop();
				gameOver.play();
				int tmp = 1;

				gOver.play();
				Score.setString("Your Score: " + to_string(score) + " High Score is : " + to_string(highScoreInt));

				Score.setPosition(175, 600);
				Score.setFillColor(Color::Cyan);
				RenderWindow Gameover(VideoMode(720, 720), "Game Over");
				Texture gamedone;
				gamedone.loadFromFile("gameover.png");
				Sprite  gamee(gamedone);



				while (Gameover.isOpen())
				{
					Event event;
					while (Gameover.pollEvent(event))
					{
						if (event.type == Event::Closed)
							Gameover.close();
					}
					Gameover.clear();
					Gameover.draw(gamee);
					Gameover.draw(Score);
					Gameover.display();
				}
			}
			green.setPosition(360, 10);


		}
		else if (green.getPosition().y < 0) {
			ystep = 6;
			bordercrash.setVolume(110);
			bordercrash.play();

		}

		else if ((green.getPosition().y >= 720 - 30 - green.getGlobalBounds().height) && (green.getPosition().x >= space.getPosition().x - green.getGlobalBounds().width / 2 && green.getPosition().x - 20 <= space.getPosition().x + space.getGlobalBounds().width - green.getGlobalBounds().width + 20 ))
		{
			cout << "collision with space " << endl;
			sound.play();
			ystep -= 6;
			score += 10;
			Score.setString("Your Score: " + to_string(score) + " Balls : " + to_string(balls));
		}
		green.move(xstep, ystep);


		/*     Blue Moving      */
		if (blue.getPosition().x > 670) {
			xbstep -= 5;
			bordercrash.setVolume(110);
			bordercrash.play();
		}
		else if (blue.getPosition().x < 0) {
			xbstep = 7;
			bordercrash.setVolume(110);
			bordercrash.play();
		}
		else if (blue.getPosition().y > 680) {
			balls--;
			Score.setString("Your Score: " + to_string(score) + " Balls : " + to_string(balls));
			if (balls <= 0) {
				Score.setString("Game Over");


				/*     Game Over    */
				window.close();

				if (score > highScoreInt) {

					ofstream fout("HighScore.txt");
					fout << score << endl;
					highScoreInt = score;
					fout.close();
				}
				music.stop();
				gameOver.play();
				int tmp = 1;

				gOver.play();
				Score.setString("Your Score: " + to_string(score) + " High Score is : " + to_string(highScoreInt));

				Score.setPosition(175, 600);
				Score.setFillColor(Color::Cyan);
				RenderWindow Gameover(VideoMode(720, 720), "Game Over");
				Texture gamedone;
				gamedone.loadFromFile("gameover.png");
				Sprite  gamee(gamedone);


				while (Gameover.isOpen())
				{
					Event event;
					while (Gameover.pollEvent(event))
					{
						if (event.type == Event::Closed)
							Gameover.close();
					}
					Gameover.clear();
					Gameover.draw(gamee);
					Gameover.draw(Score);
					Gameover.display();
				}
			}
			blue.setPosition(300, 10);


		}
		else if (blue.getPosition().y < 0) {
			ybstep = 6;
			bordercrash.setVolume(110);
			bordercrash.play();

		}


		//Bouns ball when it hits 

		else if ((blue.getPosition().y >= 720 - 30 - blue.getGlobalBounds().height) && (blue.getPosition().x >= space.getPosition().x - blue.getGlobalBounds().width / 2 && blue.getPosition().x - 20 <= space.getPosition().x + space.getGlobalBounds().width - blue.getGlobalBounds().width + 20 ))
		{
			cout << "collision with space " << endl;
			sound.play();
			ybstep -= 6;
			score += 10;
			Score.setString("Your Score: " + to_string(score) + " Balls : " + to_string(balls));
		}
		blue.move(xbstep, ybstep);





		/*     Game Render    */
		window.clear();
		window.draw(background);
		window.draw(space);
		window.draw(green);
		window.draw(blue);
		window.draw(Score);
		window.display();
	}

	return 0;
}