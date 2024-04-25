#include "windows.h"

#include "renderer/vk_renderer.cpp"

static bool isRunning = false;

LRESULT CALLBACK PlatformWindowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CLOSE:
            isRunning = false;
            break;
    }
    
    return DefWindowProcA(window, message, wParam, lParam);
}

bool PlatformCreateWindow(HWND window)
{
    const HINSTANCE instance = GetModuleHandleA(nullptr);

    WNDCLASS windowClass {};
    windowClass.lpfnWndProc = PlatformWindowCallback;
    windowClass.hInstance = instance;
    windowClass.lpszClassName = "vulkan_engine_class";

    if (!RegisterClass(&windowClass))
    {
        MessageBoxA(window, "Failed registering window class", "Error", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    window = CreateWindowExA(WS_EX_APPWINDOW, "vulkan_engine_class", "PongVulkan",
        WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_OVERLAPPED,
        100, 100, 1280, 720, nullptr, nullptr, instance, nullptr);

    if (!window)
    {
        MessageBoxA(window, "Failed creating window", "Error", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    ShowWindow(window, SW_SHOW);
    isRunning = true;
    return true;
}

void PlatformUpdateWindow(HWND window)
{
    MSG message;

    while(PeekMessageA(&message, window, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
}

int main()
{
    VkContext vkContext {};
    
    const HWND window = nullptr;
    if (!PlatformCreateWindow(window))
    {
        return -1;
    }

    if (!vk_init(&vkContext))
    {
        return -1;
    }
    
    while (isRunning)
    {
        PlatformUpdateWindow(window);
    }
    
    return 0;
}