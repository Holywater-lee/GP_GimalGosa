#define WIDTH (600)
#define HEIGHT (400)

#include "Game.h"
#include "InputHandler.h"

#include "Obstacle.h"
#include "Walker.h"

Game* Game::s_pInstance = 0;

bool Game::setup()
{
	bool result;

	result = init("Nature of Code", 100, 100, WIDTH, HEIGHT, false);

	_hider = new Walker(WIDTH / 2, HEIGHT / 2);
	_walker = new Walker(WIDTH / 2, HEIGHT / 2);

	obstacles.reserve(10);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			obstacles.push_back(new Obstacle((j + 1) * 100, (i + 1) * 100, 20));
		}
	}

	return result;
}

void Game::update()
{
	_hider->applyForce(_hider->hide(obstacles, _walker));
	_hider->update();

	_walker->applyForce(_walker->obstacleAvoidance(obstacles));
	_walker->update();
	_walker->edges();

	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
}

void Game::render()
{
	SDL_RenderClear(m_pRenderer);

	for (const auto& o : obstacles)
	{
		o->draw(m_pRenderer);
	}

	_hider->draw(m_pRenderer);
	_walker->draw(m_pRenderer);

	SDL_RenderPresent(m_pRenderer);
}

void Game::clean()
{
	delete _hider;
	delete _walker;

	for (const auto& o : obstacles)
	{
		delete o;
	}
	obstacles.clear();

	TheInputHandler::Instance()->clean();

	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);

	m_pWindow = NULL;
	m_pRenderer = NULL;

	SDL_Quit();
}

//==================================================

bool Game::init(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		return false;
	}

	m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, SDL_WINDOW_OPENGL);

	if (m_pWindow != NULL)
	{
		m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);

		SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
		SDL_RenderClear(m_pRenderer);
	}
	else
	{
		return false; // ?ˆë„???ì„¤ ?¤íŒ¨ l
	}

	m_bRunning = true;

	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);

	return true;
}

bool Game::running()
{
	return m_bRunning;
}

void Game::handleEvents()
{
	TheInputHandler::Instance()->update();
}