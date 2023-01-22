#pragma once
#include <SDL.h>
#include <vector>
#include "Particle.h"
#include "Circle.h"

class Window
{
public:
	Window(unsigned int height, unsigned int width) : h(height), w(width) 
	{
		SDL_Init(SDL_INIT_VIDEO);
		window = SDL_CreateWindow("Particle Simulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, h, w, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	}

	void update(std::vector<Particle*>* particle_vec)
	{
		this->clear();
		this->draw_circles(particle_vec);
		SDL_RenderPresent(renderer);
	}
	
private:
	unsigned int h, w;
	SDL_Window* window;
	SDL_Renderer* renderer;
	
	void clear()
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
	}

	void draw_circles(const std::vector<Particle*> *particle_vec)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		for (const Particle *p : *particle_vec) SDL_RenderFillCircle(renderer, p->pos_x, p->pos_y, p->size);
	}
};