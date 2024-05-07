#include "Engine.h"
  
using namespace std;

Engine::Engine()
{
	//get screen resolution
	// create sfml window and view
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;
	m_Window.create(VideoMode(resolution.x, resolution.y),
		"Particles",
		Style::Fullscreen);

}

void Engine::run()
{
	//timing
	Clock clock;
	// local particle for unit tests.
	cout << "Starting Particle unit tests..." << endl;
	Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2 });
	p.unitTests();
	cout << "Unit tests complete.  Starting engine..." << endl;

	while (m_Window.isOpen())
	{
		Time dt = clock.restart();
		// Update the total game time
		m_GameTimeTotal += dt;
		// make a decimal fraction from the delta time
		float dtAsSeconds = dt.asSeconds();
		//call each part of the game loop in turn
		input();
		update(dtAsSeconds);
		draw();
	}
}

void Engine::input()
{
	Event event;
	while (m_Window.pollEvent(event))
	{
		if (event.type == Event::Closed)//Handle Event::Closed event to close the window
		{
			m_Window.close();
		}
		//Check if Keyboard::isKeyPressed(Keyboard::Escape) to close the window
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			m_Window.close();
		}
		else if (event.type == Event::MouseButtonPressed)//Handle Event::MouseButtonPressed
		{
			if (event.mouseButton.button == Mouse::Left)
			{
				for (int i = 0; i <= 5; i++) // create 5 particles
				{
					int x = rand() % 26 + 25;
					Particle p(m_Window, x, { event.mouseButton.x, event.mouseButton.y });
					m_particles.push_back(p);
				}
				cout << "Size of m_particles: " << m_particles.size() << endl; // Debug print statement
			}
		}
	}
}

void Engine::update(float dtAsSeconds)
{
	for (int i = 0; i < m_particles.size(); i++)
	//for (auto it = m_particles.begin(); it != m_particles.end(); ) //iterate over m_particles
	{
		if (m_particles[i].getTTL() > 0.0)
		{
			m_particles[i].update(dtAsSeconds); //update particle and increment
			i++;
		}
		else
		{
			//erase element iterator points to
			m_particles.erase(m_particles.begin() + i);
			i--;
		}
	}
}


void Engine::draw()
{
	///clear the window
	m_Window.clear();
	for (int i = 0; i < m_particles.size(); i++ ) //iterate over m_particles
	{
		// pass each element into draw
		m_Window.draw(m_particles[i]);
	}
	//display the window
	m_Window.display();
	
}