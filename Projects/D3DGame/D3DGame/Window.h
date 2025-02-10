#ifndef _PROGJOGOS_WINDOW_H_
#define _PROGJOGOS_WINDOW_H_

#include <windows.h>
#include <windowsx.h>
#include <string>

#include "Types.h"

constexpr int WSTYLE_FULLSCREEN = WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE;
constexpr int WSTYLE_WINDOWED = WS_SYSMENU | WS_OVERLAPPED | WS_VISIBLE;

const int SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN);
const int SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN);

enum WindowMode { FULLSCREEN, WINDOWED };


class Window
{

private:

	//identifiers
	HINSTANCE hInstance;
	HWND handle;

	//dimension and positions
	int mode;
	int xPos;
	int yPos;
	int width;
	int height;
	int clientWidth;
	int clientHeight;

	//appeareance
	char title[255];
	HICON icon;
	HCURSOR cursor;
	COLORREF backgroundColor;
	DWORD style;

	//input
	static bool keyPressed[256];
	static int mouseX;
	static int mouseY;

public:
	Window();

	//getters
	//identifiers
	HINSTANCE GetInstance() const;
	HWND GetHandle() const;

	//dimension and positions
	int GetMode() const;
	int GetXPos() const;
	int GetYPos() const;
	int GetWidth() const;
	int GetHeight() const;
	int GetClientWidth() const;
	int GetClientHeight() const;

	//appearance
	const char* GetTitle() const;
	//HICON getIcon();
	//HCURSOR getCursor();
	COLORREF GetBackgroundColor() const;
	//DWORD getStyle();

	//input
	bool GetKeyDown(int) const;
	bool GetKeyUp(int) const;
	int GetMouseX() const;
	int GetMouseY() const;

	//setters
	void SetTitle(const char*);
	void SetIcon(const uint);
	void SetCursor(const uint);
	void SetBackgroundColor(COLORREF color);


	void SetMode(int);
	void SetResolution(int, int);

	//main
	bool Create();
	void Close() const;

	//other
	void Print(const char*, int, int, COLORREF color) const;


	static LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

};

//INLINE DEFINITIONS

//Getters
inline HINSTANCE Window::GetInstance() const
{
	return this->hInstance;
}

inline HWND Window::GetHandle() const
{
	return this->handle;
}


inline int Window::GetMode() const
{
	return this->mode;
}

inline int Window::GetXPos() const
{
	return this->xPos;
}

inline int Window::GetYPos() const
{
	return this->yPos;
}

inline int Window::GetWidth() const
{
	return this->width;
}

inline int Window::GetHeight() const
{
	return this->height;
}

inline int Window::GetClientWidth() const
{
	return this->clientWidth;
}

inline int Window::GetClientHeight() const
{
	return this->clientHeight;
}


inline const char* Window::GetTitle() const
{
	return this->title;
}

inline COLORREF Window::GetBackgroundColor() const
{
	return this->backgroundColor;
}


inline bool Window::GetKeyDown(int vkCode) const
{
	return this->keyPressed[vkCode];
}

inline bool Window::GetKeyUp(int vkCode) const
{
	return !this->keyPressed[vkCode];
}

inline int Window::GetMouseX() const
{
	return this->mouseX;
}

inline int Window::GetMouseY() const
{
	return this->mouseY;
}

//Setters
inline void Window::SetTitle(const char* title)
{
	strcpy_s(this->title, title);
}

inline void Window::SetIcon(const uint icon)
{
	this->icon = LoadIcon(hInstance, MAKEINTRESOURCE(icon));
}

inline void Window::SetCursor(const uint cursor)
{
	this->cursor = LoadCursor(hInstance, MAKEINTRESOURCE(cursor));
}

inline void Window::SetBackgroundColor(COLORREF bgColor)
{
	this->backgroundColor = bgColor;
}


inline void Window::SetMode(int mode)
{
	this->mode = mode;
}

inline void Window::SetResolution(int width, int height)
{
	this->width = this->clientWidth = width;
	this->height = this->clientHeight = height;
}


inline void Window::Close() const
{
	PostMessage(this->handle, WM_DESTROY, 0, 0);
}

#endif //_PROGJOGOS_WINDOW_H_
