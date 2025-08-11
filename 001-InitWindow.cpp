#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <iostream>

#include <wrl.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

using namespace Microsoft;
using namespace Microsoft::WRL;

class DX12Engine
{
public:
    void InitWindow(HINSTANCE HInstance);
    void RenderLoop();

    static LRESULT CALLBACK CallBackFunc(HWND Hwnd, UINT Message, WPARAM WParam, LPARAM LParam);
    static void Run(HINSTANCE HInstance);
    
private:
    int WindowWidth = 640;
    int WindowHeight = 480;
    HWND HWnd = nullptr;
};

void DX12Engine::InitWindow(HINSTANCE HInstance)
{
    WNDCLASS WindowClassInfo = {};
    WindowClassInfo.hInstance = HInstance;
    WindowClassInfo.lpfnWndProc = CallBackFunc;
    WindowClassInfo.lpszClassName = L"001-InitWindow";

    RegisterClass(&WindowClassInfo);

    HWnd = CreateWindow(
        WindowClassInfo.lpszClassName,
        L"001-InitWindow",
        WS_SYSMENU | WS_OVERLAPPED,
        10, 10, WindowWidth, WindowHeight,
        nullptr, nullptr, HInstance, nullptr
        );

    ShowWindow(HWnd, SW_SHOW);
}

void DX12Engine::RenderLoop()
{
    bool bShouldExit = false;
    MSG Message = {};

    while (!bShouldExit)
    {
        while (PeekMessage(&Message, nullptr, 0, 0, PM_REMOVE))
        {
            if (Message.message != WM_QUIT)
            {
                TranslateMessage(&Message);
                DispatchMessage(&Message);
            }
            else
            {
                bShouldExit = true;
            }
        }
    }
}

LRESULT DX12Engine::CallBackFunc(HWND Hwnd, UINT Message, WPARAM WParam, LPARAM LParam)
{
    switch(Message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default: return DefWindowProc(Hwnd, Message, WParam, LParam);
    }
    return 0;
}

void DX12Engine::Run(HINSTANCE HInstance)
{
    DX12Engine Engine;
    Engine.InitWindow(HInstance);
    Engine.RenderLoop();
}

int WINAPI WinMain(HINSTANCE HInstance, HINSTANCE HPrev, LPSTR CmdLine, int CmdShow)
{
    DX12Engine::Run(HInstance);
    return 0;
}
