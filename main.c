#include <stdio.h>

#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    (void)hPrevInstance;
    (void)lpCmdLine;

    // === Define a Window Class ===
    const char CLASS_NAME[] = "Renderer Window";

    WNDCLASS wc = {};
    wc.lpfnWndProc = DefWindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // === Window Creation ===
    HWND window_handle = CreateWindowEx(0,
                                        CLASS_NAME,
                                        "3D Renderer",
                                        WS_OVERLAPPEDWINDOW,
                                        CW_USEDEFAULT,
                                        CW_USEDEFAULT,
                                        CW_USEDEFAULT,
                                        CW_USEDEFAULT,
                                        NULL,      // parent window
                                        NULL,      // Menu
                                        hInstance, // Instance handle
                                        NULL       // additional application data
    );

    if (!window_handle)
    {
        return 0;
    }

    ShowWindow(window_handle, nCmdShow);

    return 0;
}