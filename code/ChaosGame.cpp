// Include important C++ libraries here
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <vector>

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

	if(!font.loadFromFile("font/KOMIKAP_.ttf"))
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

	string message[] = { "Click 3 points to create a triangle", " Click a fourth point to start the Chaos Game", 
						"Generating Sierpinski Triangle get ready to see some magic! Press Esc to exit the game"};

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
	
		if(points.size() > 0)
		{
			for(int i = 0; i < 100; i++)
			{
				int randIndex = rand() % 3;
				Vector2f vertex = vertices[randIndex];
				Vector2f lastPoint = points.back();

				Vector2f midpoint((vertex.x + lastPoint.x) / 2,
					 			  (vertex.y + lastPoint.y) / 2);
				points.push_back(midpoint);
			}
		}
	
		/*
		****************************************
		Draw
		****************************************
		*/
		if(vertices.size() < 3)
			instruction.setString(message[0]);
		else if(points.size() <= 0)
			instruction.setString(message[1]);
		else
			instruction.setString(message[2]);

		window.clear();
		window.draw(instruction);
		for(int i = 0; i < vertices.size(); i++)
		{
		    RectangleShape rect(Vector2f(10,10));
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
