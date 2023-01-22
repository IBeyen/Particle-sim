#include <vector>
#include <chrono>
#include <iostream>

#include "Display.h"
#include "Particle.h"

int main(int argc, char* argv[])
{
	int width = 1200;
	int height = 900;
	std::vector<Particle*> particles;
	for (int i = 0; i < 500; i++)
	{
		unsigned int size = rand() % 10 + 1;
		double x = rand() % width;
		double y = rand() % height;
		double vel_x = rand() % 5 - 2.5;
		double vel_y = rand() % 5 - 2.5;
		int density = rand() % 10 + 1;
		particles.push_back(new Particle(size, x, y, vel_x, vel_y, 0.99, 1));
	}
	
	particles.push_back(new Particle(9, 100, 100, 0, 0, 0.99, 1));
	particles.push_back(new Particle(10, 150, 100, 0, 0, 0.99, 1));
	
	Window window = Window(width, height);
	double timestep = 0;
	
	int CALCULATIONS_PER_TIMESTEP = 6;
	// main loop
	while (true)
	{
		auto start = std::chrono::high_resolution_clock::now();
		for (int step = 0; step < CALCULATIONS_PER_TIMESTEP; step++)
		{
			for (int i = 0; i < particles.size() - 1; i++)
			{
				for (int j = i + 1; j < particles.size(); j++)
				{
					particles[i]->apply_gravity(particles[j], 10, timestep / CALCULATIONS_PER_TIMESTEP);
					particles[i]->collision(particles[j]);
				}
			}
			for (int i = 0; i < particles.size(); i++)
			{
				particles[i]->move(timestep/CALCULATIONS_PER_TIMESTEP);
			}
		}
		window.update(&particles);
		auto end = std::chrono::high_resolution_clock::now();
		timestep = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 1e-9;
		std::cout << 1/timestep << std::endl;
	}
}