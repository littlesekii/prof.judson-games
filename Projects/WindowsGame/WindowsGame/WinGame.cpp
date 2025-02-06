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

	//Implementação do LAB
	HDC hdc{};

	int x0{ 0 }, y0{ 0 }, x1{ 140 }, y1{ 140 };
	int x0Dir{ 1 }, y0Dir{ 1 }, x1Dir{ 1 }, y1Dir{ 1 };
	int lineSpeed{ 5 };

	COLORREF rainbow[7]{ 
		0x000000FF, 
		0x00007FFF, 
		0x0000FFFF, 
		0x003FBC00, 
		0x00FF6800, 
		0x00E5007A, 
		0x00C900D3 
	};

	void DrawLine(int, int, int, int, COLORREF) const;
	void DrawLineNew(int, int, int, int, COLORREF) const;
	
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

	//Implementação do LAB
	hdc = GetDC(window->GetHandle());
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


	//Implementação do LAB

	//simple line
	if (x0 < 0 || x0 > window->GetClientWidth())
		x0Dir *= -1;
	if (x1 < 0 || x1 > window->GetClientWidth())
		x1Dir *= -1;

	if (y0 < 0 || y0 > window->GetClientHeight())
		y0Dir *= -1;					   
	if (y1 < 0 || y1 > window->GetClientHeight())
		y1Dir *= -1;

	x0 += x0Dir * lineSpeed;
	y0 += y0Dir * lineSpeed;
	x1 += x1Dir * lineSpeed;
	y1 += y1Dir * lineSpeed;
}

void WinGame::Render()
{
	window->Print("Window Game Demo", 10, 10, RGB(0, 0, 0));
	window->Print(textMode.str().c_str(), 10, 50, RGB(0, 0, 0));
	window->Print(textSize.str().c_str(),     10, 70, RGB(0, 0, 0));
	window->Print(textClientSize.str().c_str(), 10, 90, RGB(0, 0, 0));
	window->Print(textMouse.str().c_str(),    10, 110, RGB(0, 0, 0));


	//Implementação do LAB
	// 
	//simple line
	//DrawLine(x0, y0, x1, y1, rainbow[4]);
	//DrawLineNew(x0, y0, x1, y1, rainbow[4]);
	DrawLineNew(0, 0, 100, 50, rainbow[4]);

	//rainbow line //não consegui fazer funcionar como eu queria
	//for (size_t i{ 0 }; i < 7; i++)
	//	DrawLine(
	//		i * x0Dir * 20 + x0,
	//		i * y0Dir * 20 + y0,
	//		(i + 1) * x1Dir * 20 + x0,
	//		(i + 1) * y1Dir * 20 + y0,
	//		rainbow[i]
	//);

}

void WinGame::Finalize()
{
	//Implementação do LAB
	ReleaseDC(window->GetHandle(), hdc);
}

//Implementação do LAB
void WinGame::DrawLine(int x0, int y0, int x1, int y1, COLORREF color) const
{
	
	MoveToEx(hdc, x0, y0, NULL);

	//mudo a cor da linha
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, color);

	LineTo(hdc, x1, y1);
}

void WinGame::DrawLineNew(int x0, int y0, int x1, int y1, COLORREF color) const
{
	//mudo a cor da linha
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, color);


	while (x0 != x1 || y0 != y1)
	{
		SetPixel(hdc, x0, y0, color);

		if (x0 - x1 < 0)
			x0++;

		if (y0 - y1 < 0)
			y0++;
	}



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