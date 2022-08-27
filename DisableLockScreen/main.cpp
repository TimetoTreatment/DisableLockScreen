#include <stdio.h>
#include <Windows.h>

#pragma comment(lib, "Winmm.lib")


int main()
{
	bool showWindow = true;
	HWND hWnd = GetConsoleWindow();
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

	ShowWindow(hWnd, SW_SHOW);

	for (bool enabled = true;;)
	{
		if (GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_SHIFT))
		{
			if (GetAsyncKeyState(VK_OEM_3))
			{
				PlaySound(L"c:/windows/media/Windows Shutdown.wav", NULL, SND_ASYNC);
				break;
			}
			else if (GetAsyncKeyState('1'))
			{
				if (enabled)
				{
					PlaySound(L"c:/windows/media/Windows Hardware Remove.wav", NULL, SND_ASYNC);
					enabled = false;
				}
				else
				{
					PlaySound(L"c:/windows/media/Windows Hardware Insert.wav", NULL, SND_ASYNC);
					enabled = true;
				}
			}
			else if (GetAsyncKeyState('2'))
			{
				if (showWindow)
				{
					ShowWindow(hWnd, SW_HIDE);
					showWindow = false;
				}
				else
				{
					ShowWindow(hWnd, SW_SHOW);
					showWindow = true;
				}
			}
		}

		if (enabled)
			mouse_event(MOUSEEVENTF_MOVE, 0, 0, 0, 0);

		SetConsoleCursorPosition(output, { 0, 1 });
		printf("  CTRL  : %d\n", GetAsyncKeyState(VK_CONTROL) ? 1 : 0);
		printf("  SHIFT : %d\n", GetAsyncKeyState(VK_SHIFT) ? 1 : 0);
		printf("  BTICK : %d\n", GetAsyncKeyState(VK_OEM_3) ? 1 : 0);
		printf("  NUM1  : %d\n", GetAsyncKeyState('1') ? 1 : 0);
		printf("  NUM2  : %d\n\n", GetAsyncKeyState('2') ? 1 : 0);
		printf("  Enabled : %d\n", enabled ? 1 : 0);

		SetConsoleCursorPosition(output, { 0, 14 });
		printf("  [ Exit ]\n\n");
		printf("   Ctrl + Shift + `\n\n\n");
		printf("  [ Turn On/Off ]\n\n");
		printf("   Ctrl + Shift + 1\n\n\n");
		printf("  [ Show/Hide Console ]\n\n");
		printf("   Ctrl + Shift + 2\n\n\n");

		Sleep(100);
	}

	return 0;
}