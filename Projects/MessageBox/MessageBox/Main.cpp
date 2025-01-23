#define WIN32_LEAN_AND_MEAN //Incluir apenas funções do windows 32bits+
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <string>

constexpr char APP_NAME[256]{ "Application" };


int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) 
{
	
	char mbResponseText[256]{};

	int mbResult{};
	mbResult = MessageBox(NULL, "Let's calculate your screen resolution?", APP_NAME, MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2);

	if (mbResult == IDYES) {
		int screenWidth{ GetSystemMetrics(SM_CXSCREEN) };
		int screenHeight { GetSystemMetrics(SM_CYSCREEN) };

		sprintf(mbResponseText, "Your screen resolution is %dx%d.", screenWidth, screenHeight);
	}
	else {
		sprintf(mbResponseText, "Okay then...");
	}
	
	MessageBox(NULL, mbResponseText, APP_NAME, MB_ICONINFORMATION);

	return 0;
}