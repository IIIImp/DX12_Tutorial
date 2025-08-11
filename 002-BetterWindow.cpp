#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include<DirectXColors.h>

#include <wrl.h>

using namespace Microsoft;
using namespace Microsoft::WRL;
using namespace DirectX;

static constexpr int SwapChainNum = 3;

class DX12Engine
{
public:
    // Global Interface
    static void Run(HINSTANCE HInstance);

    void Render();

    void RenderLoop();
    
    void InitWindow(HINSTANCE HInstance);

    // Create Debug Device. _Debug Only.
    void CreateDebugDevice();

    // Create Core Device Based on GPU.
    // Route: Factory->Adapter->Device
    bool CreateDevice();

    // Create Three Command Objects: CommandList, CommandAllocator, CommandQueue
    // CommandList: CPU Record Command, could be more than one.
    // CommandAllocator: Command Container, could be bound to multi CommandList.
    // CommandQueue: GPU Queue. Execute command from CPU in order.
    void CreateCommandComponents();

    // Create RenderTarget And Set it to window.
    // 1. Create some descriptors to describe what the resources are.
    // 2. (Bad) Create SwapChain to bind the RT and window.
    // 3. Bind Descriptors and RTs
    void CreateRenderTarget();

    // For CPU-GPU Sync.
    void CreateFenceAndBarrier();
    
    static LRESULT CALLBACK CallBackFunc(HWND HWnd, UINT Message, WPARAM WParam, LPARAM LParam);
    
private:
    int WindowWidth = 640;
    int WindowHeight = 480;
    HWND HWnd = nullptr;

    ComPtr<ID3D12Debug> mD3D12DebugDevice;

    UINT mDXGICreateFactoryFlag = 0;
    ComPtr<IDXGIFactory5> mDXGIFactory;
    ComPtr<IDXGIAdapter1> mDXGIAdapter;
    ComPtr<ID3D12Device4> mD3D12Device;

    ComPtr<ID3D12CommandQueue> mCommandQueue;
    ComPtr<ID3D12CommandAllocator> mCommandAllocator;
    ComPtr<ID3D12GraphicsCommandList> mCommandList;

    ComPtr<IDXGISwapChain3> mDXGISwapChain;
    ComPtr<ID3D12DescriptorHeap> mRTVHeap;
    ComPtr<ID3D12Resource> mRenderTarget[SwapChainNum];
    D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle = {};
    UINT RTVDescriptorSize = 0;
    UINT FrameIndex = 0;

    ComPtr<ID3D12Fence> mFence;
    UINT FenceValue = 0;
    HANDLE RenderEvent = nullptr;
    D3D12_RESOURCE_BARRIER BeginBarrier = {};
    D3D12_RESOURCE_BARRIER EndBarrier = {};
};

void DX12Engine::InitWindow(HINSTANCE HInstance)
{
    WNDCLASS WindowClassInfo = {};
    WindowClassInfo.hInstance = HInstance;
    WindowClassInfo.lpfnWndProc = CallBackFunc;
    WindowClassInfo.lpszClassName = L"002-BetterWindow";

    RegisterClass(&WindowClassInfo);

    HWnd = CreateWindow(
        WindowClassInfo.lpszClassName,
        L"002-BetterWindow",
        WS_SYSMENU | WS_OVERLAPPED,
        10, 10, WindowWidth, WindowHeight,
        nullptr, nullptr, HInstance, nullptr
        );

    ShowWindow(HWnd, SW_SHOW);
}

void DX12Engine::CreateDebugDevice()
{
    ::CoInitialize(nullptr);
    
#if defined(_DEBUG)
    D3D12GetDebugInterface(IID_PPV_ARGS(&mD3D12DebugDevice));
    mD3D12DebugDevice->EnableDebugLayer();
    mDXGICreateFactoryFlag = DXGI_CREATE_FACTORY_DEBUG;
#endif
}

bool DX12Engine::CreateDevice()
{
    CreateDXGIFactory2(mDXGICreateFactoryFlag, IID_PPV_ARGS(&mDXGIFactory));

    // Enum All DX12 Support Level
    constexpr D3D_FEATURE_LEVEL DX12SupportLevel[] =
        {
            D3D_FEATURE_LEVEL_12_2,
            D3D_FEATURE_LEVEL_12_1,
            D3D_FEATURE_LEVEL_12_0,
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0
        };

    // Loop All GPU and Create Latest Support Level
    // TODO: We should find a best GPU here.
    for (UINT Adapter = 0; mDXGIFactory->EnumAdapters1(Adapter, &mDXGIAdapter) != ERROR_NOT_FOUND; ++Adapter)
    {
        for (const auto& Level : DX12SupportLevel)
        {
            if (SUCCEEDED(D3D12CreateDevice(mDXGIAdapter.Get(), Level, IID_PPV_ARGS(&mD3D12Device))))
            {
                return true;
            }
        }
    }

    if (!mD3D12Device)
    {
        MessageBox(nullptr, L"No Suitable GPU for DX12", L"Error", MB_OK | MB_ICONERROR);
        return false;
    }

    return false;
}

void DX12Engine::CreateCommandComponents()
{
    // Create CommandQueue first
    D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
    QueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    mD3D12Device->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&mCommandQueue));

    // Create CommandAllocator
    mD3D12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator));

    // Create CommandList
    mD3D12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&mCommandList));

    // Initial State is Record. Should close to prevent when using Reset() causes error.
    mCommandList->Close();
}

void DX12Engine::CreateRenderTarget()
{
    // Create Descriptor Heap(RTV, Render Target View)
    D3D12_DESCRIPTOR_HEAP_DESC RTVHeapDesc = {};
    RTVHeapDesc.NumDescriptors = SwapChainNum;
    RTVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    mD3D12Device->CreateDescriptorHeap(&RTVHeapDesc, IID_PPV_ARGS(&mRTVHeap));

    // Create SwapChain
    DXGI_SWAP_CHAIN_DESC1 SwapChainDesc = {};
    SwapChainDesc.BufferCount = SwapChainNum;
    SwapChainDesc.Width = WindowWidth;
    SwapChainDesc.Height = WindowHeight;
    SwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc.SampleDesc.Count = 1;
    ComPtr<IDXGISwapChain1> TempSwapChain;

    mDXGIFactory->CreateSwapChainForHwnd(mCommandQueue.Get(), HWnd, &SwapChainDesc, nullptr, nullptr, &TempSwapChain);
    TempSwapChain.As(&mDXGISwapChain);

    // Modify Descriptor in Heap.
    RTVHandle = mRTVHeap->GetCPUDescriptorHandleForHeapStart();
    RTVDescriptorSize = mD3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    for (UINT Descriptor = 0; Descriptor < SwapChainNum; ++Descriptor)
    {
        // Create an RT
        mDXGISwapChain->GetBuffer(Descriptor, IID_PPV_ARGS(&mRenderTarget[Descriptor]));
        // Create Descriptor and bind RT
        mD3D12Device->CreateRenderTargetView(mRenderTarget[Descriptor].Get(), nullptr, RTVHandle);
        // Move to next
        RTVHandle.ptr += RTVDescriptorSize;
    }
}

void DX12Engine::CreateFenceAndBarrier()
{
    // Create a wait event on CPU
    RenderEvent = CreateEvent(nullptr, false, true, nullptr);
    mD3D12Device->CreateFence(FenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));

    // Set Resource barrier
    // Begin: Present -> Render Target
    // End: Render Target -> Present
    BeginBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    BeginBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    BeginBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    EndBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    EndBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    EndBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
}

void DX12Engine::Render()
{
    // Find the Descriptor
    RTVHandle = mRTVHeap->GetCPUDescriptorHandleForHeapStart();
    FrameIndex = mDXGISwapChain->GetCurrentBackBufferIndex();
    RTVHandle.ptr += FrameIndex * RTVDescriptorSize;

    // Set Command State
    mCommandAllocator->Reset();
    mCommandList->Reset(mCommandAllocator.Get(), nullptr);

    // Set Begin Barrier for current RT
    BeginBarrier.Transition.pResource = mRenderTarget[FrameIndex].Get();
    mCommandList->ResourceBarrier(1, &BeginBarrier);

    // Set RT
    mCommandList->OMSetRenderTargets(1, &RTVHandle, false, nullptr);

    // Clear RT
    mCommandList->ClearRenderTargetView(RTVHandle, DirectX::Colors::SkyBlue, 0, nullptr);

    // Set End Barrier for current RT
    EndBarrier.Transition.pResource = mRenderTarget[FrameIndex].Get();
    mCommandList->ResourceBarrier(1, &EndBarrier);

    mCommandList->Close();

    ID3D12CommandList* TempCommandLists[] = { mCommandList.Get() };

    // Execute
    mCommandQueue->ExecuteCommandLists(1, TempCommandLists);
    mDXGISwapChain->Present(1, NULL);

    FenceValue++;
    mCommandQueue->Signal(mFence.Get(), FenceValue);
    mFence->SetEventOnCompletion(FenceValue, RenderEvent);
}

void DX12Engine::RenderLoop()
{
    bool bShouldExit = false;
    MSG Message = {};
    
    while (!bShouldExit)
    {
        // Return what calls the event
        DWORD ActiveEvent = ::MsgWaitForMultipleObjects(1, &RenderEvent, false, INFINITE, QS_ALLINPUT);

        switch(ActiveEvent - WAIT_OBJECT_0)
        {
        // Render is finished.
        case 0:
            Render();
            break;
        // Render not finished. CPU should continue dealing with window.
        case 1:
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
                    break;
                }
            }
            break;
            
        default: break;
        }
    }
}

LRESULT DX12Engine::CallBackFunc(HWND HWnd, UINT Message, WPARAM WParam, LPARAM LParam)
{
    if (Message == WM_DESTROY)
    {
        PostQuitMessage(0);
    }
    else
    {
        return DefWindowProc(HWnd, Message, WParam, LParam);
    }
    return 0;
}

void DX12Engine::Run(HINSTANCE HInstance)
{
    DX12Engine Engine;
    Engine.InitWindow(HInstance);
    Engine.CreateDebugDevice();
    Engine.CreateDevice();
    Engine.CreateCommandComponents();
    Engine.CreateRenderTarget();
    Engine.CreateFenceAndBarrier();
    
    Engine.RenderLoop();
}

int WINAPI WinMain(HINSTANCE HInstance, HINSTANCE HPrev, LPSTR CmdLine, int CmdShow)
{
    DX12Engine::Run(HInstance);
}
