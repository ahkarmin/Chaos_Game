// Include important C++ libraries here
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <ctime>

//Make the code easier to type with "using namespace"
using namespace sf;
using namespace std;


int main()
{
	
	srand(static_cast<unsigned int> (time(0)));
	
	// Create a video mode object
	VideoMode vm(1920, 1080);
	// Create and open a window for the game
	RenderWindow window(vm, "Chaos Game!!", Style::Default);
    
	Font font;
   

	if (!font.loadFromFile("font/KOMIKAP_.ttf"))
	{
		cerr << "File did not load. \n";
		return 1;
	}
	
	Text instruction;
	instruction.setFont(font);
	instruction.setCharacterSize(25);
	instruction.setFillColor(Color::Red);
	instruction.setPosition(10.f, 10.f);



	vector<Vector2f> vertices;
	vector<Vector2f> points;


	string messages[] = { 
	"Click 3 points to create a triangle", 
	" Click a fourth point to start the Chaos Game", 
	"Generating Sierpinski Triangle get ready to see some magic! Press Esc to exit the game"
	};

	// load background
	Texture background;
	background.loadFromFile("graphics/backgrounds.png");
	Sprite spritebackground;
	spritebackground.setTexture(background);
	spritebackground.setPosition(0,0);
	
	
	//sound effects
	SoundBuffer shot;
	shot.loadFromFile("sound/shoot.wav");
	Sound shoot;
	shoot.setBuffer(shot);

	// mummy moving
	Texture mummy;
	mummy.loadFromFile("graphics/bloater.png");
	Sprite spritemummy;
	spritemummy.setTexture(mummy);
	spritemummy.setPosition(0,800);
	// is mummy moving
	bool mummyActive = false;
	float mummySpeed = 0.0f;

	Clock clock;

	while (window.isOpen())
	{
		/*
		****************************************
		Handle the players input
		****************************************
		*/
		Event event;
		while (window.pollEvent(event))
		{
		    if (event.type == Event::Closed)
		    {
					// Quit the game when the window is closed
					window.close();
		    }
		    if (event.type == sf::Event::MouseButtonPressed)
		    {
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					std::cout << "the left button was pressed" << std::endl;
					std::cout << "mouse x: " << event.mouseButton.x << std::endl;
					std::cout << "mouse y: " << event.mouseButton.y << std::endl;
					shoot.play();
		
					
					if(vertices.size() < 3)
					{
						vertices.push_back(Vector2f(event.mouseButton.x, event.mouseButton.y));
					}
					else if(points.size() == 0)
					{
					///fourth click
					
						points.push_back(Vector2f(event.mouseButton.x, event.mouseButton.y));
					}
				}
		    }
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		/*
		****************************************
		Update
		****************************************
		*/
		Time dt = clock.restart();
	
		if(points.size() > 0)
		{
			for(int i = 0; i < 100; i++)
			{
				int randIndex = rand() % 3;
				Vector2f vertex = vertices[randIndex];
				Vector2f lastPoint = points.back();

				Vector2f midpoint(
					(vertex.x + lastPoint.x) / 2,
					 (vertex.y + lastPoint.y) / 2
					 );
				points.push_back(midpoint);
			}
		}
		if(!mummyActive)
		{
			// how fast is moving
			srand((int)time(0));
			mummySpeed = (rand() % 20 ) + 200;

			// how high is the mummy
			srand((int) time(0) * 10);
			float height = (rand() % 500) + 500;
			spritemummy.setPosition(2000, height);
			mummyActive = true;
		}
		else
		{
			spritemummy.setPosition(spritemummy.getPosition().x - (mummySpeed * dt.asSeconds()), spritemummy.getPosition().y);
			// has the mummy run out of space
			if (spritemummy.getPosition().x < - 100)
			{
				mummyActive = false;
			}


		}
	
		/*
		****************************************
		Draw
		****************************************
		*/
		window.draw(spritebackground);
		if(vertices.size() < 3)
		instruction.setString(messages[0]);
		else if(points.empty())
		instruction.setString(messages[1]);
		else
		instruction.setString(messages[2]);
		
		window.clear();
		window.draw(spritebackground);
		window.draw(spritemummy);
		window.draw(instruction);
		
		for(float i = 0; i < vertices.size(); i++)
		{
		    RectangleShape rect(Vector2f(30,30));
		    rect.setPosition(Vector2f(vertices[i].x, vertices[i].y));
		    rect.setFillColor(Color::Blue);
		    window.draw(rect);
		}
		///TODO:  Draw points
		for(const auto& p : points)
		{
			CircleShape dot(1);
			dot.setPosition(p);
			dot.setFillColor(Color::Red);
			window.draw(dot);
		}
	
		window.display();
	}
}