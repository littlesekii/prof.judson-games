#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

struct {
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
} screenSize;

struct {
    int width = 960;
    int height = 540;
} windowSize;

struct { int x; int y; } windowPos;

constexpr bool FULLSCREEN = false;

char text[256]{ "Sample Text" };
int textIndex{ 11 };

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
    mainWindowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    mainWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    mainWindowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    mainWindowClass.lpszMenuName = NULL;
    mainWindowClass.lpszClassName = "MainWindow";

    if (!RegisterClass(&mainWindowClass)) {
        MessageBox(NULL, "Failed to register \"MainWindow\" window class.", "Error", MB_ICONERROR);
        return 0;
    }

    struct { int x; int y; } windowPos;
    windowPos = {
        screenSize.width / 2 - windowSize.width / 2,
        screenSize.height / 2 - windowSize.height / 2
    };

    if (FULLSCREEN) {
        windowSize = { screenSize.width, screenSize.height };
        windowPos = { 0, 0 };
    }



    windowHwnd = CreateWindowEx(
        NULL,
        "MainWindow",
        "Application",
        (FULLSCREEN) ? (WS_POPUP) : (WS_OVERLAPPED | WS_SYSMENU),
        windowPos.x,
        windowPos.y,
        windowSize.width,
        windowSize.height,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!FULLSCREEN) {
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
    }

    ShowWindow(windowHwnd, nCmdShow);
    UpdateWindow(windowHwnd);


    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;

}

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    HDC hdc{};
    PAINTSTRUCT ps;

    const RECT windowRect{ 0, 0, windowSize.width, windowSize.height };
    const RECT paddingRect{ 0, 0, 0, 43 };
    RECT drawRect{
        windowRect.left + paddingRect.left,
        windowRect.top + paddingRect.top,
        windowRect.right - paddingRect.right,
        windowRect.bottom - paddingRect.bottom,
    };

    switch (msg)
    {

    case WM_CHAR:

        switch (wParam)
        {
        case 0x08: //Backspace
            if (textIndex > 0)
                text[--textIndex] = '\0';

            break;
        case 0x09: //Tab
        case 0x0A: //Line Feed
        case 0x0D: //Carriage Return
        case 0x1B: //Esc
            MessageBeep((UINT)-1);

            break;
        default:
            if (textIndex < 255)
                text[textIndex++] = (char)wParam;
        }
        InvalidateRect(hwnd, NULL, true);

        break;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);

        DrawText(hdc, text, -1, &drawRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

        EndPaint(hwnd, &ps);

        break;
    case WM_DESTROY:
        PostQuitMessage(0);

        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}