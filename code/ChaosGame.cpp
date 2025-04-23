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

int ChooseRandomVertice(int &prevIndex, int verticeSize);

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
	vector<Vertex> verticeLines;


	string message[] = { 
	"Click points to create a shape.\nHit \"Enter\" if you're satisfied with the number of points and Chaos Game starts!!",
	"Generating Sierpinski Shape get ready to see some magic! Press Esc to exit the game"
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

	// To prevent duplicate points
	int prevIndex = 0;
	// After clickDone = true, starts drawing a picture.
    bool clickDone = false;

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
		    // After a key is Pressed, 
            // Push last vertice to points
            if (event.type == sf::Event::KeyPressed)
            {
                clickDone = true;
                points.push_back(vertices.back());
                vertices.pop_back();
            }
		    if (event.type == sf::Event::MouseButtonPressed)
		    {
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					if(!clickDone)
					{
                        vertices.push_back(Vector2f(event.mouseButton.x, event.mouseButton.y));
						Vertex tempVertex(Vector2f(event.mouseButton.x, event.mouseButton.y), Color(255, 245, 203),
										  Vector2f(0, 10));
						verticeLines.push_back(tempVertex);
						
						shoot.play();
                        
                        std::cout << "the left button was pressed" << std::endl;
                        std::cout << "mouse x: " << event.mouseButton.x << std::endl;
                        std::cout << "mouse y: " << event.mouseButton.y << std::endl;
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
				int randIndex = ChooseRandomVertice(prevIndex, vertices.size());
				Vector2f vertex = vertices[randIndex];
				Vector2f lastPoint = points.back();

				Vector2f midpoint(
				(vertex.x + lastPoint.x) / 2,
				(vertex.y + lastPoint.y) / 2);

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
		if(!clickDone)
			instruction.setString(message[0]);
		else
			instruction.setString(message[1]);
		
		window.clear();
		window.draw(spritebackground);
		window.draw(spritemummy);
		window.draw(instruction);
		
		for(float i = 0; i < vertices.size(); i++)
		{
		    RectangleShape rect(Vector2f(30,30));
		    rect.setPosition(Vector2f(vertices[i].x, vertices[i].y));
		    rect.setFillColor(Color::Yellow);
		    window.draw(rect);
		}

		window.draw(&verticeLines[0], verticeLines.size(), LineStrip);

		/// Draw points
		for(const auto& p : points)
		{
			CircleShape dot(1);
			dot.setPosition(p);
			dot.setFillColor(Color(235, 215, 203));
			window.draw(dot);
		}
	
		window.display();
	}
}

/**  @param int &prevIndex - previousIndex to be updated and compared for duplicates
 *   @param int verticeSize
 *   @return int newRandomIndex
*/ 
int ChooseRandomVertice(int &prevIndex, int verticeSize)
{
    int newRandIndex = rand() % verticeSize;
    while (newRandIndex == prevIndex && verticeSize != 3)
    {
        newRandIndex = rand() % verticeSize;
    }
    prevIndex = newRandIndex;
    return newRandIndex;
}