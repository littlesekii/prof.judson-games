#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include "Resources.h"

constexpr bool FULLSCREEN = false;

LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    MSG msg{};
    HWND windowHwnd{};
    WNDCLASS mainWindowClass{};

    mainWindowClass.style = CS_HREDRAW | CS_VREDRAW;
    mainWindowClass.lpfnWndProc = WinProc;
    mainWindowClass.cbClsExtra = 0;
    mainWindowClass.cbWndExtra = 0;
    mainWindowClass.hInstance = hInstance;
    mainWindowClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPDEF));
    mainWindowClass.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_APPDEF));
    mainWindowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    mainWindowClass.lpszMenuName = NULL;
    mainWindowClass.lpszClassName = "MainWindow";

    if (!RegisterClass(&mainWindowClass)) {
        MessageBox(NULL, "Failed to register \"MainWindow\" window class.", "Error", MB_ICONERROR);
        return 0;
    }

    struct { 
        int width = GetSystemMetrics(SM_CXSCREEN); 
        int height = GetSystemMetrics(SM_CYSCREEN); 
    } screenSize;

    struct { 
        int width = 960; 
        int height = 540; 
    } windowSize;

    struct { int x; int y; } windowPos;
    windowPos = { 
        screenSize.width / 2 - windowSize.width / 2, 
        screenSize.height / 2 - windowSize.height / 2
    };

    windowHwnd = CreateWindowEx(
        NULL,
        "MainWindow",
        "Application",
        (FULLSCREEN) ? (WS_POPUP)           : (WS_OVERLAPPED | WS_SYSMENU),
        (FULLSCREEN) ? (CW_USEDEFAULT)      : (windowPos.x),
        (FULLSCREEN) ? (CW_USEDEFAULT)      : (windowPos.y),
        (FULLSCREEN) ? (screenSize.width)   : (windowSize.width),
        (FULLSCREEN) ? (screenSize.height)  : (windowSize.height),
        NULL,
        NULL,
        hInstance,
        NULL
    );

    // reajusta o rect de acordo com a janela para pegar o tamanho da tela 
    // na qual ela fique com o tamanho do cliente igual ao definido
    RECT windowRect{ 0, 0, windowSize.width, windowSize.height };
    AdjustWindowRectEx(&windowRect,
        GetWindowStyle(windowHwnd),
        GetMenu(windowHwnd) != NULL,
        GetWindowExStyle(windowHwnd)
    );

    windowSize = {
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top
    };

    //redimensiona a tela
    MoveWindow(
        windowHwnd,
        windowPos.x,
        windowPos.y,
        windowSize.width,
        windowSize.height,
        true
    );    

    ShowWindow(windowHwnd, nCmdShow);
    UpdateWindow(windowHwnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;

}

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);

        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}