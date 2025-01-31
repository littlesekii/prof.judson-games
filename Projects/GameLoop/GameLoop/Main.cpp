// DEFINES //
#define WIN32_LEAN_AND_MEAN
//


// INCLUDES //
#include <windows.h>
#include <windowsx.h>
#include <random>

#include "Resources.h"
//


// FUNCTION PROTOTYPES //
LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

void GameInit();
void GameUpdate();
void GameRender();
void GameFinalize();

void MovingBrushUpdate();
void MovingBrushRender();

void RandomPixelRender();
void RandomPixelUpdate();

void BitmapUpdate();
void BitmapRender();

//


// GLOBAL VARIABLES //
constexpr bool FULLSCREEN{ true };

const struct
{
	int width{ GetSystemMetrics(SM_CXSCREEN) };
	int height{ GetSystemMetrics(SM_CYSCREEN) };
}
screenSize;

const int clientWidth{ (FULLSCREEN) ? screenSize.width : 960 };
const int clientHeight{ (FULLSCREEN) ? screenSize.height : 540 };

struct 
{
	int width{ clientWidth };
	int height{ clientHeight };
} 
windowSize;

WNDCLASS windowClass{};
HWND windowHwnd{};
MSG msg{};
HDC hdc{};

bool vkKeys[256]{};

short x{};
short y{};

short r{};
short g{};
short b{};

//random
std::random_device rd{};
std::mt19937 mt(rd());
std::uniform_int_distribution<int> xRand(0, clientWidth);
std::uniform_int_distribution<int> yRand(0, clientHeight);
std::uniform_int_distribution<int> rgbRand(0, 255);


//bitmap
HBITMAP image;
BITMAP bitmap;
HDC imageHDC;

short bitmapXDir{};
short bitmapYDir{};

RECT clientRect{};

//


// MAIN FUNCTION //
int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdshow)
{

	windowClass.style			= CS_VREDRAW | CS_HREDRAW;
	windowClass.lpfnWndProc		= WinProc;
	windowClass.cbClsExtra	 	= 0;
	windowClass.cbWndExtra	 	= 0;
	windowClass.hInstance	 	= hInstance;
	windowClass.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEFAULT));
	windowClass.hCursor		 	= LoadCursor(hInstance, MAKEINTRESOURCE(IDC_DEFAULT));
	windowClass.hbrBackground	= (HBRUSH) GetStockObject(BLACK_BRUSH);
	windowClass.lpszMenuName	= NULL;
	windowClass.lpszClassName	= "MainWindow";

	if (!RegisterClass(&windowClass)) 
	{
		MessageBox(NULL, "Failed to register \"MainWindow\" class.", "Error", MB_ICONERROR);
		return 0;
	}

	windowHwnd = CreateWindow(
		"MainWindow",
		"Game",
		(FULLSCREEN) ? (WS_POPUP) : (WS_OVERLAPPED | WS_SYSMENU),
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		screenSize.width,
		screenSize.height,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!FULLSCREEN)
	{
		RECT windowRect{ 0, 0, windowSize.width, windowSize.height };

		AdjustWindowRectEx(
			&windowRect, 
			GetWindowStyle(windowHwnd), 
			GetMenu(windowHwnd) != NULL, 
			GetWindowExStyle(windowHwnd)
		);

		windowSize.width = windowRect.right - windowRect.left;
		windowSize.height = windowRect.bottom - windowRect.top;

		int windowPosX{ screenSize.width / 2 - windowSize.width / 2 };
		int windowPosY{ screenSize.height / 2 - windowSize.height / 2 };

		MoveWindow(
			windowHwnd, 
			windowPosX,
			windowPosY,
			windowSize.width, 
			windowSize.height, 
			true
		);
	}

	ShowWindow(windowHwnd, nCmdshow);
	UpdateWindow(windowHwnd);

	GameInit();

	do 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			GameUpdate();
			GameRender();
			Sleep(1000/60);
		}
	} 
	while (msg.message != WM_QUIT);

	GameFinalize();

	return (int)msg.wParam;
}
//


// FUNCTION DEFINITIONS //
LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	switch (msg)
	{
	case WM_KEYDOWN:
		vkKeys[wParam] = true;

		break;
	case WM_KEYUP:
		vkKeys[wParam] = false;

		break;
	case WM_DESTROY:
		PostQuitMessage(0);

		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

void GameInit()
{
	hdc = GetDC(windowHwnd);

	x = 0;
	y = 0;

	r = 255;
	g = 255;
	b = 255;

	clientRect = { 0, 0, clientWidth, clientHeight };

	bitmapXDir = 1;
	bitmapYDir = 1;

	//carrega a imagem bitmap
	image = (HBITMAP) LoadImage(
		NULL, 
		"res\\dilio.bmp", 
		IMAGE_BITMAP, 
		0, 
		0, 
		LR_LOADFROMFILE
	);

	//lê as propriedades do HBITMAP (image) e passa para o BITMAP (bitmap)
	GetObject(image, sizeof(BITMAP), &bitmap);

	//cria o hdc da imagem baseado no hdc da tela
	imageHDC = CreateCompatibleDC(hdc);
	//coloca a imagem no hdc da imagem
	SelectObject(imageHDC, image);

}
void GameUpdate()
{
	if (vkKeys[VK_ESCAPE])
		PostMessage(windowHwnd, WM_DESTROY, 0, 0);

	//game
	//MovingBrushUpdate();
	//RandomPixelUpdate();
	BitmapUpdate();
}
void GameRender()
{
	//game
	//MovingBrushRender();
	//RandomPixelRender();
	BitmapRender();
}
void GameFinalize()
{
	DeleteDC(imageHDC);
	DeleteObject((HBITMAP)image);
	ReleaseDC(windowHwnd, hdc);
}

// "Games"

void MovingBrushUpdate() 
{
	if (vkKeys[VK_LEFT])
		x--;
	if (vkKeys[VK_RIGHT])
		x++;
	if (vkKeys[VK_DOWN])
		y++;
	if (vkKeys[VK_UP])
		y--;

	if (vkKeys[VK_SPACE])
		InvalidateRect(windowHwnd, NULL, true);
}
void MovingBrushRender()
{
	SetPixel(hdc, x, y, RGB(255, 255, 255));
	SetPixel(hdc, x + 1, y, RGB(255, 255, 255));
	SetPixel(hdc, x, y + 1, RGB(255, 255, 255));
	SetPixel(hdc, x - 1, y, RGB(255, 255, 255));
	SetPixel(hdc, x, y - 1, RGB(255, 255, 255));
	SetPixel(hdc, x + 1, y + 1, RGB(255, 255, 255));
	SetPixel(hdc, x - 1, y - 1, RGB(255, 255, 255));
	SetPixel(hdc, x + 1, y - 1, RGB(255, 255, 255));
	SetPixel(hdc, x - 1, y + 1, RGB(255, 255, 255));
	SetPixel(hdc, x - 2, y, RGB(255, 255, 255));
	SetPixel(hdc, x + 2, y, RGB(255, 255, 255));
	SetPixel(hdc, x, y + 2, RGB(255, 255, 255));
	SetPixel(hdc, x, y - 2, RGB(255, 255, 255));
}

void RandomPixelUpdate()
{
	x = xRand(mt);
	y = yRand(mt);

	r = rgbRand(mt);
	g = rgbRand(mt);
	b = rgbRand(mt);

	//char str[4]{};
	//if (r > 255) 
	//{
	//	sprintf_s(str, "%d", r);
	//	OutputDebugString(str);
	//}
}
void RandomPixelRender()
{
	SetPixel(hdc, x, y, RGB(r, g, b));	
}

void BitmapUpdate()
{
	x += bitmapXDir;
	y += bitmapYDir;

	if (x <= 0 || x + bitmap.bmWidth >= clientWidth)
		bitmapXDir *= -1;
	if (y <= 0 || y + bitmap.bmHeight >= clientHeight)
		bitmapYDir *= -1;
}
void BitmapRender()
{
	FillRect(hdc, &clientRect, CreateSolidBrush(RGB(0, 0, 0)));

	BitBlt(hdc, x, y, bitmap.bmWidth, bitmap.bmHeight, imageHDC, 0, 0, SRCCOPY);
}
//
