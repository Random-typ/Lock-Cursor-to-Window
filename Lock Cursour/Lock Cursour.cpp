#include <Windows.h>

bool Locked = false;
HWND ForegroundWindow;
void LockCursor(HWND _window) {
    RECT rect;
    GetClientRect(_window, &rect);

    POINT TL = { rect.left, rect.top },
        BR = { rect.right, rect.bottom };

    MapWindowPoints(_window, nullptr, &TL, 1);
    MapWindowPoints(_window, nullptr, &BR, 1);

    rect = { TL.x, TL.y, BR.x, BR.y };

    ClipCursor(&rect);
}

void UnlockCursor() {
    ClipCursor(NULL);
}

void CheckForInput()
{// List of all KeyCodes: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    for (; ; Sleep(10)) {// 10ms
        if (Locked && ForegroundWindow != GetForegroundWindow()) {
            Locked = false;
            for (size_t i = 0; i < 100; Sleep(1000), i++)
                if (ForegroundWindow == GetForegroundWindow())
                    LockCursor(ForegroundWindow),
                    Locked = true;
            if (!Locked)
                UnlockCursor();
        }
        if (GetKeyState(VK_CONTROL) & 0x8000 &&         // Control
            GetKeyState(VK_SHIFT) & 0x8000 &&           // Shift
            GetKeyState(VK_MENU) & 0x8000 &&            // Alt
            GetKeyState('L') & 0x8000) {                // L
            if (Locked)
                UnlockCursor(),
                Locked = false;
            else
                ForegroundWindow = GetForegroundWindow(),
                LockCursor(ForegroundWindow),
                Locked = true;
            // Await until hotkey isnt pressed anymore
            for (; GetKeyState(VK_CONTROL) & 0x8000 &&  // Control
                GetKeyState(VK_SHIFT) & 0x8000 &&       // Shift
                GetKeyState(VK_MENU) & 0x8000 &&        // Alt
                GetKeyState('L') & 0x8000; Sleep(10));  // L
        }
    }
}

int main()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE);// Hide this Window
    CheckForInput();
}