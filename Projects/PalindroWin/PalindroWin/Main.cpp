#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>

// Habilita estilos visuais modernos
#pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


// Definições 
#define GROUPBOX_ID   100
#define LABEL_TEXT_ID 200
#define EDIT_TEXT_ID  300 
#define BUTTON_ID	  401

//Variáveis globais
HINSTANCE* hInstancePointer{};

RECT clientRect{};
HWND hwndGroupBox{};
HWND hwndLabel{};
HWND hwndEdit{};
HWND hwndButton{};

bool resultClean{ true };

HFONT appFont{};

LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	hInstancePointer = &hInstance;

	//Criar classe da janela
	WNDCLASS windowClass{};

	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WinProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = "MainWindow";
	
	//Registrar classe da janela
	if (!RegisterClass(&windowClass)) {
		MessageBox(NULL, "Failed to register window class.", "Error", MB_ICONERROR);
	}

	//Pega o tamanho da tela
	struct {
		short width{ (short)GetSystemMetrics(SM_CXSCREEN) };
		short height{ (short)GetSystemMetrics(SM_CYSCREEN) };
	} screenSize;

	//Define o tamanho da janela
	struct {
		short width{ 400 };
		short height{ 220 };
	} windowSize;

	
	//Cria a janela
	HWND hwndMainWindow = CreateWindow(
		"MainWindow",
		"PalindroWin",
		WS_OVERLAPPED | WS_SYSMENU,
		screenSize.width / 2 - windowSize.width / 2,
		screenSize.height / 2 - windowSize.height / 2,
		windowSize.width,
		windowSize.height,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	//Mostra a janela
	ShowWindow(hwndMainWindow, nCmdShow);
	UpdateWindow(hwndMainWindow);

	//Trata as mensagens do sistema
	MSG msg{};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return int(msg.wParam);
}


LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	char debugMsg[16]{};
	sprintf_s(debugMsg, "%d\n", msg);
	//OutputDebugString(debugMsg);

	PAINTSTRUCT		ps;		// usado em WM_PAINT
	HDC				hdc;	// identificador de contexto do dispositivo

	switch (msg) {
		case WM_CREATE:
			// pega as dimensões da janela para o posicionamento de objetos
			GetClientRect(hwnd, &clientRect);

			//cria a groupbox
			hwndGroupBox = CreateWindow(
				"BUTTON",
				"Resultado",
				WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
				clientRect.left + 10,
				clientRect.top + 10,
				clientRect.right - 20,
				clientRect.bottom - 20,
				hwnd,
				(HMENU)GROUPBOX_ID,
				*hInstancePointer,
				NULL
			);			

			//Cria a label
			hwndLabel = CreateWindowEx(
				NULL,
				"STATIC",
				"Palavra:",
				WS_CHILD | WS_VISIBLE,
				40,
				140,
				140,
				24,
				hwnd,
				(HMENU)LABEL_TEXT_ID,
				*hInstancePointer,
				NULL
			);

			//cria o edit
			hwndEdit = CreateWindowEx(
				WS_EX_CLIENTEDGE,
				"EDIT",
				"",
				WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
				85,
				137,
				130,
				20,
				hwnd,
				HMENU(EDIT_TEXT_ID),
				*hInstancePointer,
				NULL
			);

			//cria o button
			hwndButton = CreateWindow(
				"BUTTON",
				"Resultado",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				255,
				135,
				100,
				24,
				hwnd,
				(HMENU)BUTTON_ID,
				*hInstancePointer,
				NULL
			);

			//Cria fonte
			appFont = CreateFont(15, 0, 0, 0, FW_NORMAL, false, false, false,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				CLEARTYPE_QUALITY, VARIABLE_PITCH | FF_SWISS, "MS Sans Serif");


			//altera a fonte dos componentes
			SendMessage(hwndGroupBox, WM_SETFONT, (WPARAM)appFont, true);
			SendMessage(hwndLabel, WM_SETFONT, (WPARAM)appFont, true);
			SendMessage(hwndEdit, WM_SETFONT, (WPARAM)appFont, true);
			SendMessage(hwndButton, WM_SETFONT, (WPARAM)appFont, true);

			break;
		case WM_COMMAND:
			//OutputDebugString(debugMsg);

			//sprintf_s(debugMsg, "%d\n", LOWORD(wParam));
			//OutputDebugString(debugMsg);

			if (HIWORD(wParam) == EN_CHANGE) {
				//limpa a área de impressão do resultado (se ela etiver suja)
				if(!resultClean) {
					//apaga o resultado anterior
					RECT strRect = { 
						clientRect.left + 20, 
						clientRect.top + 40, 
						clientRect.right - 20, 
						clientRect.top + 90 
					};
					InvalidateRect(hwnd, &strRect, true);
					UpdateWindow(hwnd);
					resultClean = true;
				}

			}

			//pega o id do objeto que foi realizado o comando
			switch(LOWORD(wParam)) {


				case BUTTON_ID:

					//pega o comprimento do texto armazenado na caixa de edição
					int strLen = GetWindowTextLength(hwndEdit);

					//pega o contexto do dispositivo
					hdc = GetDC(hwnd);

					//Cria fonte
					appFont = CreateFont(22, 0, 0, 0, FW_NORMAL, false, false, false,
						DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						CLEARTYPE_QUALITY, VARIABLE_PITCH | FF_SWISS, "MS Sans Serif");

					//define a nova fonte do hdc
					SelectObject(hdc, appFont);

					//define o fundo transparente
					SetBkMode(hdc, TRANSPARENT);

					//define a regição de exibição do resultado
					RECT strRect{ 
						clientRect.left + 20, 
						clientRect.top + 40, 
						clientRect.right - 20, 
						clientRect.top + 90 
					};

					if (!strLen)
						DrawText(hdc, "Digite uma palavra.", -1, &strRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					else {

						char* palavra = new char[strLen + 1];

						// pega o texto da caixa de edição
						GetWindowText(hwndEdit, palavra, strLen + 1);

						int i{ 0 };
						int j{ strLen - 1 };

						while (i < j && palavra[i] == palavra[j]) { 
							i++;
							j--;
						};

						delete[] palavra;

						//mostra o resultado
						if (i >= j)
							DrawText(hdc, "A palavra é um palíndromo.", -1, &strRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
						else
							DrawText(hdc, "A palavra não é um palíndromo.", -1, &strRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					}

					ReleaseDC(hwnd, hdc);

					//seta o resultado como sujo
					resultClean = false;

					break;
			}


			break;
		case WM_PAINT:
			// simplesmente valida a janela 
			//hdc = BeginPaint(hwnd, &ps);

			//// finaliza o desenho 
			//EndPaint(hwnd, &ps);
			break;
		//case WM_CLOSE:
		//	OutputDebugString(debugMsg);
		//	//PostQuitMessage(0);
		//	return 0;
		case WM_DESTROY:
			OutputDebugString(debugMsg);
			PostQuitMessage(0);

			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);

	}
	return 0;
}