#include "Window.h"

bool Window::keyPressed[256]{};
int Window::mouseX{};
int Window::mouseY{};

Window::Window() 
{

    this->hInstance = GetModuleHandle(NULL);
    this->handle = 0;
    
	this->mode = FULLSCREEN;
	this->xPos = 0;
	this->yPos = 0;
	this->width = SCREEN_WIDTH;
	this->height = SCREEN_HEIGHT;
	this->clientWidth = this->width;
	this->clientHeight = this->width;

	this->SetTitle("Windows Game");
	this->icon = LoadIcon(NULL, IDI_APPLICATION);
	this->cursor = LoadCursor(NULL, IDC_ARROW);
	this->backgroundColor = RGB(0, 0, 0);
	this->style = WSTYLE_FULLSCREEN;
}

bool Window::Create()
{
	WNDCLASSEX windowClass{};
	
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = this->WinProc;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = this->hInstance;
    windowClass.hIcon = this->icon;
    windowClass.hCursor = this->cursor;
    windowClass.hbrBackground = (HBRUSH) CreateSolidBrush(this->backgroundColor);
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = "MainWindow";
    windowClass.hIconSm = this->icon;

	if (!RegisterClassEx(&windowClass))
	{
		MessageBox(NULL, "Failed to register \"MainWindow\" class.", "Error", MB_ICONERROR);
		return 0;
	}

	this->handle = CreateWindowEx(
		NULL,
		"MainWindow",
		this->title,
		(this->mode == FULLSCREEN ? WSTYLE_FULLSCREEN : WSTYLE_WINDOWED),
		this->xPos,
		this->yPos,
		this->width,
		this->height,
		NULL,
		NULL,
		this->hInstance,
		NULL
	);

	if (this->mode == WINDOWED)
	{
		RECT windowRect{ 0, 0, this->width, this->height };

		AdjustWindowRectEx(
			&windowRect,
			GetWindowStyle(this->handle),
			GetMenu(this->handle) != NULL,
			GetWindowExStyle(this->handle)
		);

		this->width = windowRect.right - windowRect.left;
		this->height = windowRect.bottom - windowRect.top;

		this->xPos = SCREEN_WIDTH / 2 - this->width / 2;
		this->yPos = SCREEN_HEIGHT / 2 - this->height / 2;

		MoveWindow(
			handle,
			this->xPos,
			this->yPos,
			this->width,
			this->height,
			true
		);
	}

	return (handle ? true : false);
}

void Window::Print(const char* text, int x, int y, COLORREF color) const
{
	// esta função exibe o texto na posição (x,y) da tela usando a cor especificada
	// ela usa a biblioteca do Windows (lenta) e deve ser usada apenas para depuração

	// pega o contexto do dispositivo gráfico
	HDC xdc = GetDC(handle);

	// define a cor do texto
	SetTextColor(xdc, color);

	// define o fundo do texto como transparente
	SetBkMode(xdc, TRANSPARENT);

	// mostra o texto
	TextOut(xdc, x, y, text, strlen(text));

	// libera o contexto do dispositivo
	ReleaseDC(handle, xdc);
}


LRESULT CALLBACK Window::WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
	case WM_KEYDOWN:
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
        keyPressed[wParam] = true;

        return 0;
    case WM_KEYUP:
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
        keyPressed[wParam] = false;

        return 0;
    case WM_MOUSEMOVE:
        mouseX = GET_X_LPARAM(lParam);
        mouseY = GET_Y_LPARAM(lParam);

        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}