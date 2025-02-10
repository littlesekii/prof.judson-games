#ifndef _PROGJOGOS_GAME_H_
#define _PROGJOGOS_GAME_H_

#include "Window.h"

class Game
{

protected:
	static Window*& window;

public:
	Game();
	virtual ~Game();

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Finalize() = 0;
};

#endif //_PROGJOGOS_GAME_H_