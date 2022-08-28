#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <Windows.h>

#pragma comment(lib, "Winmm.lib")


typedef enum Color
{
	COLOR_BLACK = 0,
	COLOR_BLUE,
	COLOR_GREEN,
	COLOR_CYAN,
	COLOR_RED,
	COLOR_MAGENTA,
	COLOR_BROWN,
	COLOR_LIGHTGRAY,
	COLOR_DARKGRAY,
	COLOR_LIGHTBLUE,
	COLOR_LIGHTGREEN,
	COLOR_LIGHTCYAN,
	COLOR_LIGHTRED,
	COLOR_LIGHTMAGENTA,
	COLOR_YELLOW,
	COLOR_WHITE,
	COLOR_RANDOM,
	COLOR_DEFAULT = 7

} Color;


void Draw(HANDLE handleConsole, const char* str, int row, int col, Color color)
{
	static int colorSet[5] = { COLOR_RED, COLOR_LIGHTGREEN, COLOR_LIGHTBLUE, COLOR_LIGHTMAGENTA, COLOR_YELLOW };
	int colorValue = (color == COLOR_RANDOM) ? colorSet[rand() % 5] : color;

	SetConsoleTextAttribute(handleConsole, colorValue);

	if (col >= 0 && row >= 0)
		SetConsoleCursorPosition(handleConsole, (COORD) { col, row });

	printf("%s", str);
}


int main()
{
	int lockScreenTimeout = 300;
	int interval = lockScreenTimeout * 800;
	int elapsedTime = 0;
	clock_t prevTime = clock();

	char buffer[64] = { 0 };
	char progressBar[] = ".......................................";

	int disableLock = 1;
	int showWindow = 1;
	HWND handleWindow = GetConsoleWindow();
	HANDLE handleConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleCursorInfo(handleConsole, &(CONSOLE_CURSOR_INFO){ 100, FALSE });
	ShowWindow(handleWindow, showWindow ? SW_SHOW : SW_HIDE);

	srand((unsigned int)time(NULL));

	for (int exit = 0; !exit;)
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
				static clock_t prevPressedTime = 0;

				if (clock() - prevPressedTime > 500)
				{
					prevPressedTime = clock();

					if (disableLock)
					{
						PlaySound(L"c:/windows/media/Windows Hardware Remove.wav", NULL, SND_ASYNC);
						disableLock = 0;
					}
					else
					{
						PlaySound(L"c:/windows/media/Windows Hardware Insert.wav", NULL, SND_ASYNC);
						disableLock = 1;
					}
				}
			}
			else if (GetAsyncKeyState('2'))
			{
				static clock_t prevPressedTime = 0;

				if (clock() - prevPressedTime > 500)
				{
					prevPressedTime = clock();

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
		}

		elapsedTime = clock() - prevTime;

		if (disableLock && elapsedTime >= interval)
		{
			elapsedTime = 0;
			prevTime = clock();

			memset(progressBar, '.', sizeof(progressBar) - 1);

			mouse_event(MOUSEEVENTF_MOVE, 0, 0, 0, 0);
		}

		if (elapsedTime / (lockScreenTimeout * 1000) < 1)
		{
			int index = elapsedTime * ((int)sizeof(progressBar) - 1) / (lockScreenTimeout * 1000);

			if (progressBar[index] == '.')
				progressBar[index] = '#';
		}

		Draw(handleConsole, "Commands:", 1, 1, COLOR_YELLOW);

		Draw(handleConsole, "Exit Program", 3, 5, COLOR_WHITE);
		Draw(handleConsole, "Ctrl", 3, 29, GetAsyncKeyState(VK_CONTROL) ? COLOR_LIGHTCYAN : COLOR_DARKGRAY);
		Draw(handleConsole, "Shift", 3, 35, GetAsyncKeyState(VK_SHIFT) ? COLOR_LIGHTCYAN : COLOR_DARKGRAY);
		Draw(handleConsole, "`", 3, 42, GetAsyncKeyState(VK_OEM_3) ? COLOR_LIGHTCYAN : COLOR_DARKGRAY);

		Draw(handleConsole, "Enable / Disable Lock", 5, 5, COLOR_WHITE);
		Draw(handleConsole, "Ctrl", 5, 29, GetAsyncKeyState(VK_CONTROL) ? COLOR_LIGHTCYAN : COLOR_DARKGRAY);
		Draw(handleConsole, "Shift", 5, 35, GetAsyncKeyState(VK_SHIFT) ? COLOR_LIGHTCYAN : COLOR_DARKGRAY);
		Draw(handleConsole, "1", 5, 42, GetAsyncKeyState('1') ? COLOR_LIGHTCYAN : COLOR_DARKGRAY);

		Draw(handleConsole, "Show / Hide Window", 7, 5, COLOR_WHITE);
		Draw(handleConsole, "Ctrl", 7, 29, GetAsyncKeyState(VK_CONTROL) ? COLOR_LIGHTCYAN : COLOR_DARKGRAY);
		Draw(handleConsole, "Shift", 7, 35, GetAsyncKeyState(VK_SHIFT) ? COLOR_LIGHTCYAN : COLOR_DARKGRAY);
		Draw(handleConsole, "2", 7, 42, GetAsyncKeyState('2') ? COLOR_LIGHTCYAN : COLOR_DARKGRAY);

		Draw(handleConsole, "Status:", 10, 1, COLOR_YELLOW);

		sprintf(buffer, "%-15s", disableLock ? "Disable Lock" : "Enable Lock");
		Draw(handleConsole, buffer, 12, 5, disableLock ? COLOR_WHITE : COLOR_DARKGRAY);
		sprintf(buffer, "%3d / %d (sec)", elapsedTime / 1000, lockScreenTimeout);
		Draw(handleConsole, buffer, 12, 28, disableLock ? COLOR_WHITE : COLOR_DARKGRAY);
		Draw(handleConsole, progressBar + 1, 13, 5, disableLock ? COLOR_WHITE : COLOR_DARKGRAY);

		Sleep(100);
	}

	SetConsoleTextAttribute(handleConsole, COLOR_DEFAULT);

	return 0;
}
