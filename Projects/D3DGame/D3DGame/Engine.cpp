#include "Engine.h"

Game* Engine::game = nullptr;
Window* Engine::window = nullptr;
Graphics* Engine::graphics = nullptr;

Engine::Engine()
{
    this->window = new Window();
    this->graphics = new Graphics();
}

Engine::~Engine()
{
    delete this->game;
    delete this->graphics;
    delete this->window;
}

int Engine::Run(Game* level) {

    this->game = level;

    this->window->Create();
    this->graphics->Initialize(this->window);

    return Loop();
}

int Engine::Loop()
{
    MSG msg{};
    HDC hdc{};
    RECT clientRect{};

    hdc = GetDC(this->window->GetHandle());

    GetClientRect(this->window->GetHandle(), &clientRect);

    this->game->Init();

    do
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            this->game->Update();

            this->graphics->Clear();
            this->game->Render();
            this->graphics->Present();

            Sleep(1000 / 60); //60fps cap (workaround)
        }
    } while (msg.message != WM_QUIT);

    this->game->Finalize();

    ReleaseDC(this->window->GetHandle(), hdc);

    return (int)msg.wParam;
}
