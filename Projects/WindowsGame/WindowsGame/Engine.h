#ifndef _PROGJOGOS_ENGINE_H_
#define _PROGJOGOS_ENGINE_H_

#include "Game.h"
#include "Window.h"

class Engine
{

private:
	int Loop();

public:
	static Game* game;
	static Window* window;


	Engine();
	~Engine();

	int Run(Game* level);
};

#endif //_PROGJOGOS_ENGINE_H_
