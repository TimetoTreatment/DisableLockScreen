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


void draw(HANDLE handleOutput, const char* str, int row, int col, Color color)
{
	static int colorSet[5] = { COLOR_RED, COLOR_LIGHTGREEN, COLOR_LIGHTBLUE, COLOR_LIGHTMAGENTA, COLOR_YELLOW };
	int colorValue = (color == COLOR_RANDOM) ? colorSet[rand() % 5] : color;

	SetConsoleTextAttribute(handleOutput, colorValue);

	if (row >= 0 && col >= 0)
		SetConsoleCursorPosition(handleOutput, (COORD) { col, row });

	printf("%s", str);
}


int hasMouseEvent(HWND handleWindow)
{
	static POINT prevPos = { 0,0 };
	POINT pos;

	if (GetCursorPos(&pos))
	{
		if (ScreenToClient(handleWindow, &pos))
		{
			if (pos.x != prevPos.x || pos.y != prevPos.y)
			{
				prevPos = pos;
				return 1;
			}
		}

		if (GetAsyncKeyState(VK_LBUTTON) || GetAsyncKeyState(VK_RBUTTON))
			return 1;
	}

	return 0;
}


int hasKeyboardEvent()
{

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
	HANDLE handleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleCursorInfo(handleOutput, &(CONSOLE_CURSOR_INFO){ 100, FALSE });
	ShowWindow(handleWindow, showWindow ? SW_SHOW : SW_HIDE);

	srand((unsigned int)time(NULL));

	for (int exit = 0; !exit;)
	{
		int isControlPressed = GetAsyncKeyState(VK_CONTROL);
		int isShiftPressed = GetAsyncKeyState(VK_SHIFT);

		if (isControlPressed && isShiftPressed)
		{
			if (GetAsyncKeyState(VK_OEM_3))
			{
				PlaySound(TEXT("c:/windows/media/Windows Shutdown.wav"), NULL, SND_SYNC);
				exit = 1;
			}
			else if (GetAsyncKeyState('1'))
			{
				static clock_t prevPressedTime = 0;

				if (clock() - prevPressedTime > 500)
				{
					prevPressedTime = clock();

					if (disableLock)
					{
						PlaySound(TEXT("c:/windows/media/Windows Hardware Remove.wav"), NULL, SND_ASYNC);
						disableLock = 0;
					}
					else
					{
						PlaySound(TEXT("c:/windows/media/Windows Hardware Insert.wav"), NULL, SND_ASYNC);
						disableLock = 1;
					}
				}
			}
			else if (GetAsyncKeyState('2'))
			{
				static clock_t prevPressedTime = 0;

				if (clock() - prevPressedTime > 250)
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

		int interrupt = (isControlPressed || isShiftPressed || hasMouseEvent(handleWindow)) ? 1 : 0;

		if (interrupt || (disableLock && elapsedTime >= interval))
		{
			elapsedTime = 0;
			prevTime = clock();

			memset(progressBar, '.', sizeof(progressBar) - 1);

			mouse_event(MOUSEEVENTF_MOVE, 0, 0, 0, 0);
		}
		else
		{
			elapsedTime = clock() - prevTime;

			if (elapsedTime >= lockScreenTimeout * 1000)
				elapsedTime = lockScreenTimeout * 1000;

			int index = elapsedTime * ((int)sizeof(progressBar) - 1) / (lockScreenTimeout * 1000);

			if (index < sizeof(progressBar) - 1)
				progressBar[index] = '#';
		}

		draw(handleOutput, "Commands:", 1, 1, COLOR_YELLOW);

		draw(handleOutput, "Exit Program", 3, 5, COLOR_WHITE);
		draw(handleOutput, "Ctrl", 3, 29, GetAsyncKeyState(VK_CONTROL) ? COLOR_LIGHTCYAN : COLOR_DARKGRAY);
		draw(handleOutput, "Shift", 3, 35, GetAsyncKeyState(VK_SHIFT) ? COLOR_LIGHTCYAN : COLOR_DARKGRAY);
		draw(handleOutput, "`", 3, 42, GetAsyncKeyState(VK_OEM_3) ? COLOR_LIGHTCYAN : COLOR_DARKGRAY);

		draw(handleOutput, "Enable / Disable Lock", 5, 5, COLOR_WHITE);
		draw(handleOutput, "Ctrl", 5, 29, GetAsyncKeyState(VK_CONTROL) ? COLOR_LIGHTCYAN : COLOR_DARKGRAY);
		draw(handleOutput, "Shift", 5, 35, GetAsyncKeyState(VK_SHIFT) ? COLOR_LIGHTCYAN : COLOR_DARKGRAY);
		draw(handleOutput, "1", 5, 42, GetAsyncKeyState('1') ? COLOR_LIGHTCYAN : COLOR_DARKGRAY);

		draw(handleOutput, "Show / Hide Window", 7, 5, COLOR_WHITE);
		draw(handleOutput, "Ctrl", 7, 29, GetAsyncKeyState(VK_CONTROL) ? COLOR_LIGHTCYAN : COLOR_DARKGRAY);
		draw(handleOutput, "Shift", 7, 35, GetAsyncKeyState(VK_SHIFT) ? COLOR_LIGHTCYAN : COLOR_DARKGRAY);
		draw(handleOutput, "2", 7, 42, GetAsyncKeyState('2') ? COLOR_LIGHTCYAN : COLOR_DARKGRAY);

		draw(handleOutput, "Status:", 10, 1, COLOR_YELLOW);

		sprintf(buffer, "%-15s", disableLock ? "Disable Lock" : "Enable Lock");
		draw(handleOutput, buffer, 12, 5, disableLock ? COLOR_WHITE : COLOR_DARKGRAY);
		sprintf(buffer, "%3d / %d (sec)", elapsedTime / 1000, lockScreenTimeout);
		draw(handleOutput, buffer, 12, 28, disableLock ? COLOR_WHITE : COLOR_DARKGRAY);
		draw(handleOutput, progressBar + 1, 13, 5, disableLock ? COLOR_WHITE : COLOR_DARKGRAY);

		Sleep(100);
	}

	SetConsoleTextAttribute(handleOutput, COLOR_DEFAULT);

	return 0;
}
