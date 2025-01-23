#define WIN32_LEAN_AND_MEAN
#include <windows.h>

LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{

	HWND hwnd;			// identificador da janela
	MSG msg;			// mensagem
	WNDCLASS wndClass;	// classe da janela


	// define as propriedades da classe da janela
	wndClass.style			= CS_HREDRAW | CS_VREDRAW;				
	wndClass.lpfnWndProc	= WinProc;								
	wndClass.cbClsExtra		= 0;
	wndClass.cbWndExtra		= 0;
	wndClass.hInstance		= hInstance;
	wndClass.hIcon			= LoadIcon(NULL, IDI_SHIELD);		
	wndClass.hCursor		= LoadCursor(NULL, IDC_CROSS);			
	wndClass.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName	= NULL;
	wndClass.lpszClassName  = "MainWindow";

	// registra a class "MainWindow"
	if (!RegisterClass(&wndClass)) {
		MessageBox(NULL, "Error while registering class.", "Application Error", MB_ICONERROR);
		return 0;
	}

	//cria a janela baseada na classe "MainWindow"
	hwnd = CreateWindow(
		"MainWindow",			// classe da janela
		"My First Window",      // titulo da janela
		WS_OVERLAPPEDWINDOW,    // estilo da habeka
		CW_USEDEFAULT,			// posicao x
		CW_USEDEFAULT,			// posicao y
		CW_USEDEFAULT,			// largura
		CW_USEDEFAULT,			// altura
		NULL,					// identificador da janela pai
		NULL,					// identificador do menu
		hInstance,				// identificador da aplicação
		NULL					// parametros de criaç~]ao
	);


	//mostra a janela
	ShowWindow(hwnd, nCmdShow);

	// envia uma mensagem WM_PAINT para a janela
	UpdateWindow(hwnd);

	//Trata as mensagens destinadas a janela da aplicação
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return int(msg.wParam);
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	switch (msg) {
		case WM_PAINT:
			HDC hdc;
			PAINTSTRUCT ps;
			RECT rect;

			hdc = BeginPaint(hwnd, &ps);

			GetClientRect(hwnd, &rect);
			DrawText(hdc, "littlesekii", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			EndPaint(hwnd, &ps);

			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}