#include <stdio.h>
#include <Windows.h>
#include <time.h>

#pragma comment(lib, "Winmm.lib")


typedef enum Color
{
	COLOR_RED = 12,
	COLOR_GREEN = 2,
	COLOR_BLUE = 3,
	COLOR_PURPLE = 13,
	COLOR_YELLOW = 14,
	COLOR_WHITE = 15,
	COLOR_BLACK = 0,
	COLOR_DARKGRAY = 8,
	COLOR_DEFAULT = 7,
	COLOR_RANDOM = 777

} Color;


void Draw(HANDLE handleConsole, const char* str, int row, int col, Color color)
{
	static int colorSet[5] = { COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_PURPLE, COLOR_YELLOW };
	int colorValue = (color == COLOR_RANDOM) ? colorSet[rand() % 5] : color;

	SetConsoleTextAttribute(handleConsole, colorValue);

	if (col >= 0 && row >= 0)
	{
		COORD cursorPos = { col, row };

		SetConsoleCursorPosition(handleConsole, cursorPos);
	}

	printf("%s", str);
}


int main()
{
	int showWindow = 1;

	HWND handleWindow = GetConsoleWindow();
	HANDLE handleConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info = { 100, FALSE };

	SetConsoleCursorInfo(handleConsole, &info);
	ShowWindow(handleWindow, SW_SHOW);

	srand((unsigned int)time(NULL));

	for (int enabled = 1;;)
	{
		if (GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_SHIFT))
		{
			if (GetAsyncKeyState(VK_OEM_3))
			{
				PlaySound(L"c:/windows/media/Windows Shutdown.wav", NULL, SND_SYNC);
				break;
			}
			else if (GetAsyncKeyState('1'))
			{
				if (enabled)
				{
					PlaySound(L"c:/windows/media/Windows Hardware Remove.wav", NULL, SND_ASYNC);
					enabled = 0;
				}
				else
				{
					PlaySound(L"c:/windows/media/Windows Hardware Insert.wav", NULL, SND_ASYNC);
					enabled = 1;
				}
			}
			else if (GetAsyncKeyState('2'))
			{
				if (showWindow)
				{
					ShowWindow(handleWindow, SW_HIDE);
					showWindow = 0;
				}
				else
				{
					ShowWindow(handleWindow, SW_SHOW);
					showWindow = 1;
				}
			}
		}

		if (enabled)
			mouse_event(MOUSEEVENTF_MOVE, 0, 1, 0, 0);

		Draw(handleConsole, "Available Commands:", 1, 1, COLOR_YELLOW);

		Draw(handleConsole, "Program Quit", 3, 5, COLOR_WHITE);
		Draw(handleConsole, "Ctrl", 3, 25, GetAsyncKeyState(VK_CONTROL) ? COLOR_RED : COLOR_DARKGRAY);
		Draw(handleConsole, "Shift", 3, 31, GetAsyncKeyState(VK_SHIFT) ? COLOR_GREEN : COLOR_DARKGRAY);
		Draw(handleConsole, "`", 3, 38, GetAsyncKeyState(VK_OEM_3) ? COLOR_BLUE : COLOR_DARKGRAY);
		Draw(handleConsole, "Enable/Disable", 5, 5, COLOR_WHITE);
		Draw(handleConsole, "Ctrl", 5, 25, GetAsyncKeyState(VK_CONTROL) ? COLOR_RED : COLOR_DARKGRAY);
		Draw(handleConsole, "Shift", 5, 31, GetAsyncKeyState(VK_SHIFT) ? COLOR_GREEN : COLOR_DARKGRAY);
		Draw(handleConsole, "1", 5, 38, GetAsyncKeyState('1') ? COLOR_BLUE : COLOR_DARKGRAY);
		Draw(handleConsole, "Window Close/Open", 7, 5, COLOR_WHITE);
		Draw(handleConsole, "Ctrl", 7, 25, GetAsyncKeyState(VK_CONTROL) ? COLOR_RED : COLOR_DARKGRAY);
		Draw(handleConsole, "Shift", 7, 31, GetAsyncKeyState(VK_SHIFT) ? COLOR_GREEN : COLOR_DARKGRAY);
		Draw(handleConsole, "2", 7, 38, GetAsyncKeyState('2') ? COLOR_BLUE : COLOR_DARKGRAY);

		Draw(handleConsole, "Status:", 10, 1, COLOR_YELLOW);

		Draw(handleConsole, "Disable Lock Screen => ", 12, 5, enabled ? COLOR_WHITE : COLOR_DARKGRAY);
		Draw(handleConsole, enabled ? "ON " : "OFF", -1, -1, enabled ? COLOR_WHITE : COLOR_DARKGRAY);

		Sleep(100);
	}

	SetConsoleTextAttribute(handleConsole, COLOR_DEFAULT);

	return 0;
}