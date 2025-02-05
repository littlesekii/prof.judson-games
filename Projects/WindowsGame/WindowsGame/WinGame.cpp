#define WIN32_LEAN_AND_MEAN
#include <sstream>

#include "Resource.h"
#include "Engine.h"
#include "Game.h"

class WinGame : public Game
{

private:
	std::stringstream textSize;
	std::stringstream textClientSize;
	std::stringstream textMode;
	std::stringstream textMouse;
	
public:
	void Init();
	void Update();
	void Render();
	void Finalize();

};


void WinGame::Init()
{
	textSize << "Tamanho: " << window->GetWidth() << "x" << window->GetHeight();
	textClientSize << "Tamanho Cliente: " << window->GetClientWidth() << "x" << window->GetClientHeight();
	textMode << "Formato: " << (window->GetMode() == WINDOWED ? "Em Janela" : "Tela Cheia");
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

	textMouse.str("");
	textMouse << window->GetMouseX() << " x " << window->GetMouseY();
}

void WinGame::Render()
{
	window->Print("Window Game Demo", 10, 10, RGB(0, 0, 0));
	window->Print(textMode.str().c_str(), 10, 50, RGB(0, 0, 0));
	window->Print(textSize.str().c_str(),     10, 70, RGB(0, 0, 0));
	window->Print(textClientSize.str().c_str(), 10, 90, RGB(0, 0, 0));
	window->Print(textMouse.str().c_str(),    10, 110, RGB(0, 0, 0));

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

	engine->Run(new WinGame());
}