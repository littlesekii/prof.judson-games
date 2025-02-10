#define WIN32_LEAN_AND_MEAN
#include <sstream>

#include "Resource.h"
#include "Engine.h"
#include "Game.h"

class WinGame : public Game
{

private:

public:
	void Init();
	void Update();
	void Render();
	void Finalize();

};


void WinGame::Init()
{
}

void WinGame::Update()
{
	/*if (window->GetKeyDown(VK_LBUTTON))
		window->Close();*/
	if (
		window->GetKeyDown(VkKeyScan('c')) &&
		window->GetKeyDown(VkKeyScan('l')) &&
		window->GetKeyDown(VkKeyScan('o')) &&
		window->GetKeyDown(VkKeyScan('s')) &&
		window->GetKeyDown(VkKeyScan('e'))
		)
		window->Close();
}

void WinGame::Render()
{

}

void WinGame::Finalize()
{

}




int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	Engine* engine = new Engine();
	engine->window->SetMode(WINDOWED);
	engine->window->SetResolution(960, 540);
	engine->window->SetBackgroundColor(RGB(240, 240, 140));
	engine->window->SetTitle("Window Game");
	engine->window->SetIcon(IDI_DEFAULT);
	engine->window->SetCursor(IDC_DEFAULT);

	int exitCode = engine->Run(new WinGame());

	delete engine;
	return exitCode;
}