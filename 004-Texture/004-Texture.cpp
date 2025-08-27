#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <wincodec.h>

#include <wrl.h>
#include <string>
#include <sstream>

using namespace Microsoft;
using namespace Microsoft::WRL;
using namespace DirectX;

static constexpr int SwapChainNum = 3;

	struct WICTranslate
	{
		GUID wic;
		DXGI_FORMAT format;
	};
	

	
	struct WICConvert
	{
		GUID source;
		GUID target;
	};


	

	bool GetTargetPixelFormat(const GUID* pSourceFormat, GUID* pTargetFormat)
	{
	const WICConvert g_WICConvert[54] =
	{
		{ GUID_WICPixelFormatBlackWhite,            GUID_WICPixelFormat8bppGray },			// DXGI_FORMAT_R8_UNORM
		{ GUID_WICPixelFormat1bppIndexed,           GUID_WICPixelFormat32bppRGBA },			// DXGI_FORMAT_R8G8B8A8_UNORM
		{ GUID_WICPixelFormat2bppIndexed,           GUID_WICPixelFormat32bppRGBA },			// DXGI_FORMAT_R8G8B8A8_UNORM
		{ GUID_WICPixelFormat4bppIndexed,           GUID_WICPixelFormat32bppRGBA },			// DXGI_FORMAT_R8G8B8A8_UNORM
		{ GUID_WICPixelFormat8bppIndexed,           GUID_WICPixelFormat32bppRGBA },			// DXGI_FORMAT_R8G8B8A8_UNORM
		{ GUID_WICPixelFormat2bppGray,              GUID_WICPixelFormat8bppGray },			// DXGI_FORMAT_R8_UNORM
		{ GUID_WICPixelFormat4bppGray,              GUID_WICPixelFormat8bppGray },			// DXGI_FORMAT_R8_UNORM
		{ GUID_WICPixelFormat16bppGrayFixedPoint,   GUID_WICPixelFormat16bppGrayHalf },		// DXGI_FORMAT_R16_FLOAT
		{ GUID_WICPixelFormat32bppGrayFixedPoint,   GUID_WICPixelFormat32bppGrayFloat },	// DXGI_FORMAT_R32_FLOAT
		{ GUID_WICPixelFormat16bppBGR555,           GUID_WICPixelFormat16bppBGRA5551 },		// DXGI_FORMAT_B5G5R5A1_UNORM
		{ GUID_WICPixelFormat32bppBGR101010,        GUID_WICPixelFormat32bppRGBA1010102 },	// DXGI_FORMAT_R10G10B10A2_UNORM
		{ GUID_WICPixelFormat24bppBGR,              GUID_WICPixelFormat32bppRGBA },			// DXGI_FORMAT_R8G8B8A8_UNORM
		{ GUID_WICPixelFormat24bppRGB,              GUID_WICPixelFormat32bppRGBA },			// DXGI_FORMAT_R8G8B8A8_UNORM
		{ GUID_WICPixelFormat32bppPBGRA,            GUID_WICPixelFormat32bppRGBA },			// DXGI_FORMAT_R8G8B8A8_UNORM
		{ GUID_WICPixelFormat32bppPRGBA,            GUID_WICPixelFormat32bppRGBA },			// DXGI_FORMAT_R8G8B8A8_UNORM
		{ GUID_WICPixelFormat48bppRGB,              GUID_WICPixelFormat64bppRGBA },			// DXGI_FORMAT_R16G16B16A16_UNORM
		{ GUID_WICPixelFormat48bppBGR,              GUID_WICPixelFormat64bppRGBA },			// DXGI_FORMAT_R16G16B16A16_UNORM
		{ GUID_WICPixelFormat64bppBGRA,             GUID_WICPixelFormat64bppRGBA },			// DXGI_FORMAT_R16G16B16A16_UNORM
		{ GUID_WICPixelFormat64bppPRGBA,            GUID_WICPixelFormat64bppRGBA },			// DXGI_FORMAT_R16G16B16A16_UNORM
		{ GUID_WICPixelFormat64bppPBGRA,            GUID_WICPixelFormat64bppRGBA },			// DXGI_FORMAT_R16G16B16A16_UNORM
		{ GUID_WICPixelFormat48bppRGBFixedPoint,    GUID_WICPixelFormat64bppRGBAHalf },		// DXGI_FORMAT_R16G16B16A16_FLOAT
		{ GUID_WICPixelFormat48bppBGRFixedPoint,    GUID_WICPixelFormat64bppRGBAHalf },		// DXGI_FORMAT_R16G16B16A16_FLOAT
		{ GUID_WICPixelFormat64bppRGBAFixedPoint,   GUID_WICPixelFormat64bppRGBAHalf },		// DXGI_FORMAT_R16G16B16A16_FLOAT
		{ GUID_WICPixelFormat64bppBGRAFixedPoint,   GUID_WICPixelFormat64bppRGBAHalf },		// DXGI_FORMAT_R16G16B16A16_FLOAT
		{ GUID_WICPixelFormat64bppRGBFixedPoint,    GUID_WICPixelFormat64bppRGBAHalf },		// DXGI_FORMAT_R16G16B16A16_FLOAT
		{ GUID_WICPixelFormat48bppRGBHalf,          GUID_WICPixelFormat64bppRGBAHalf },		// DXGI_FORMAT_R16G16B16A16_FLOAT
		{ GUID_WICPixelFormat64bppRGBHalf,          GUID_WICPixelFormat64bppRGBAHalf },		// DXGI_FORMAT_R16G16B16A16_FLOAT
		{ GUID_WICPixelFormat128bppPRGBAFloat,      GUID_WICPixelFormat128bppRGBAFloat },	// DXGI_FORMAT_R32G32B32A32_FLOAT
		{ GUID_WICPixelFormat128bppRGBFloat,        GUID_WICPixelFormat128bppRGBAFloat },	// DXGI_FORMAT_R32G32B32A32_FLOAT
		{ GUID_WICPixelFormat128bppRGBAFixedPoint,  GUID_WICPixelFormat128bppRGBAFloat },	// DXGI_FORMAT_R32G32B32A32_FLOAT
		{ GUID_WICPixelFormat128bppRGBFixedPoint,   GUID_WICPixelFormat128bppRGBAFloat },	// DXGI_FORMAT_R32G32B32A32_FLOAT
		{ GUID_WICPixelFormat32bppRGBE,             GUID_WICPixelFormat128bppRGBAFloat },	// DXGI_FORMAT_R32G32B32A32_FLOAT
		{ GUID_WICPixelFormat32bppCMYK,             GUID_WICPixelFormat32bppRGBA },			// DXGI_FORMAT_R8G8B8A8_UNORM
		{ GUID_WICPixelFormat64bppCMYK,             GUID_WICPixelFormat64bppRGBA },			// DXGI_FORMAT_R16G16B16A16_UNORM
		{ GUID_WICPixelFormat40bppCMYKAlpha,        GUID_WICPixelFormat64bppRGBA },			// DXGI_FORMAT_R16G16B16A16_UNORM
		{ GUID_WICPixelFormat80bppCMYKAlpha,        GUID_WICPixelFormat64bppRGBA },			// DXGI_FORMAT_R16G16B16A16_UNORM
		{ GUID_WICPixelFormat32bppRGB,              GUID_WICPixelFormat32bppRGBA },			// DXGI_FORMAT_R8G8B8A8_UNORM
		{ GUID_WICPixelFormat64bppRGB,              GUID_WICPixelFormat64bppRGBA },			// DXGI_FORMAT_R16G16B16A16_UNORM
		{ GUID_WICPixelFormat64bppPRGBAHalf,        GUID_WICPixelFormat64bppRGBAHalf },		// DXGI_FORMAT_R16G16B16A16_FLOAT

		{ GUID_WICPixelFormat128bppRGBAFloat,       GUID_WICPixelFormat128bppRGBAFloat },	// DXGI_FORMAT_R32G32B32A32_FLOAT
		{ GUID_WICPixelFormat64bppRGBAHalf,         GUID_WICPixelFormat64bppRGBAHalf },		// DXGI_FORMAT_R16G16B16A16_FLOAT
		{ GUID_WICPixelFormat64bppRGBA,             GUID_WICPixelFormat64bppRGBA },			// DXGI_FORMAT_R16G16B16A16_UNORM
		{ GUID_WICPixelFormat32bppRGBA,             GUID_WICPixelFormat32bppRGBA },			// DXGI_FORMAT_R8G8B8A8_UNORM
		{ GUID_WICPixelFormat32bppBGRA,             GUID_WICPixelFormat32bppBGRA },			// DXGI_FORMAT_B8G8R8A8_UNORM
		{ GUID_WICPixelFormat32bppBGR,              GUID_WICPixelFormat32bppBGR },			// DXGI_FORMAT_B8G8R8X8_UNORM
		{ GUID_WICPixelFormat32bppRGBA1010102XR,    GUID_WICPixelFormat32bppRGBA1010102XR },// DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM
		{ GUID_WICPixelFormat32bppRGBA1010102,      GUID_WICPixelFormat32bppRGBA1010102 },	// DXGI_FORMAT_R10G10B10A2_UNORM
		{ GUID_WICPixelFormat16bppBGRA5551,         GUID_WICPixelFormat16bppBGRA5551 },		// DXGI_FORMAT_B5G5R5A1_UNORM
		{ GUID_WICPixelFormat16bppBGR565,           GUID_WICPixelFormat16bppBGR565 },		// DXGI_FORMAT_B5G6R5_UNORM
		{ GUID_WICPixelFormat32bppGrayFloat,        GUID_WICPixelFormat32bppGrayFloat },	// DXGI_FORMAT_R32_FLOAT
		{ GUID_WICPixelFormat16bppGrayHalf,         GUID_WICPixelFormat16bppGrayHalf },		// DXGI_FORMAT_R16_FLOAT
		{ GUID_WICPixelFormat16bppGray,             GUID_WICPixelFormat16bppGray },			// DXGI_FORMAT_R16_UNORM
		{ GUID_WICPixelFormat8bppGray,              GUID_WICPixelFormat8bppGray },			// DXGI_FORMAT_R8_UNORM
		{ GUID_WICPixelFormat8bppAlpha,             GUID_WICPixelFormat8bppAlpha }			// DXGI_FORMAT_A8_UNORM
	};
	
		*pTargetFormat = *pSourceFormat;
		for (size_t i = 0; i < _countof(g_WICConvert); ++i)
		{
			GUID temp = GUID_WICPixelFormat16bppGray;
			if (IsEqualGUID(g_WICConvert[i].source, *pSourceFormat))
			{
				*pTargetFormat = g_WICConvert[i].target;
				return true;
			}
		}
		return false;
	}
	
	DXGI_FORMAT GetDXGIFormatFromPixelFormat(const GUID* pPixelFormat)
	{
		const WICTranslate g_WICFormats[15] =
{
			{ GUID_WICPixelFormat128bppRGBAFloat,       DXGI_FORMAT_R32G32B32A32_FLOAT },
			{ GUID_WICPixelFormat64bppRGBAHalf,         DXGI_FORMAT_R16G16B16A16_FLOAT },
			{ GUID_WICPixelFormat64bppRGBA,             DXGI_FORMAT_R16G16B16A16_UNORM },
			{ GUID_WICPixelFormat32bppRGBA,             DXGI_FORMAT_R8G8B8A8_UNORM },
			{ GUID_WICPixelFormat32bppBGRA,             DXGI_FORMAT_B8G8R8A8_UNORM },
			{ GUID_WICPixelFormat32bppBGR,              DXGI_FORMAT_B8G8R8X8_UNORM },
			{ GUID_WICPixelFormat32bppRGBA1010102XR,    DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM },
			{ GUID_WICPixelFormat32bppRGBA1010102,      DXGI_FORMAT_R10G10B10A2_UNORM },
			{ GUID_WICPixelFormat16bppBGRA5551,         DXGI_FORMAT_B5G5R5A1_UNORM },
			{ GUID_WICPixelFormat16bppBGR565,           DXGI_FORMAT_B5G6R5_UNORM },
			{ GUID_WICPixelFormat32bppGrayFloat,        DXGI_FORMAT_R32_FLOAT },
			{ GUID_WICPixelFormat16bppGrayHalf,         DXGI_FORMAT_R16_FLOAT },
			{ GUID_WICPixelFormat16bppGray,             DXGI_FORMAT_R16_UNORM },
			{ GUID_WICPixelFormat8bppGray,              DXGI_FORMAT_R8_UNORM },
			{ GUID_WICPixelFormat8bppAlpha,             DXGI_FORMAT_A8_UNORM }
};
		for (size_t i = 0; i < _countof(g_WICFormats); ++i)
		{
			if (InlineIsEqualGUID(g_WICFormats[i].wic, *pPixelFormat))
			{
				return g_WICFormats[i].format;
			}
		}
		return DXGI_FORMAT_UNKNOWN;
	}

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

    // Load Texture
    bool LoadTextureFromFile();

	// Create SRV Descriptor Heap
	void CreateSRVHeap();

	// Create UploadResource and DefaultResource
	void CreateUploadAndDefaultResource();

	// Copy Texture to Default Resource
	void CopyTextureDataToDefaultResource();

	// SRV
	void CreateSRV();

    // Create Root-Signature
    void CreateRootSignature();

    // Create PSO
    void CreatePSO();

    // Create Vertex Resources
    void CreateVertexResource();
    
    static LRESULT CALLBACK CallBackFunc(HWND HWnd, UINT Message, WPARAM WParam, LPARAM LParam);
    
private:
    int WindowWidth = 640;
    int WindowHeight = 640;
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

    std::wstring TextureFileName = L"diamond_ore.png";
    ComPtr<IWICImagingFactory> mWICFactory;
    ComPtr<IWICBitmapDecoder> mWICBitmapDecoder;
    ComPtr<IWICBitmapFrameDecode> mWICBitmapFrameDecode;
    ComPtr<IWICFormatConverter> mWICFormatConverter;
    ComPtr<IWICBitmapSource> mWICBitmapSource;
    UINT TextureWidth = 0;
    UINT TextureHeight = 0;
    UINT BitsPerPixel = 0;
    UINT BytePerRowSize = 0;
    DXGI_FORMAT TextureFormat = DXGI_FORMAT_UNKNOWN;

    ComPtr<ID3D12DescriptorHeap> mSRVHeap;
    D3D12_CPU_DESCRIPTOR_HANDLE SRV_CPUHandle;
    D3D12_GPU_DESCRIPTOR_HANDLE SRV_GPUHandle;

    ComPtr<ID3D12Resource> mUplaodTextureResource;
    ComPtr<ID3D12Resource> mDefaultTextureResource;
    UINT TextureSize = 0;
    UINT UploadResourceRowSize = 0;
    UINT UploadResourceSize = 0;

    ComPtr<ID3D12RootSignature> mRootSignature;
    ComPtr<ID3D12PipelineState> mPipelineStateObject;

    ComPtr<ID3D12Resource> mVertexResource;
    struct VERTEX
    {
        XMFLOAT4 Position;
        XMFLOAT2 Color;
    };
    D3D12_VERTEX_BUFFER_VIEW mVertexBufferView = {};

    D3D12_VIEWPORT mViewport = D3D12_VIEWPORT{0, 0, float(WindowWidth), float(WindowHeight), D3D12_MIN_DEPTH, D3D12_MAX_DEPTH};
    D3D12_RECT mScissorRect = D3D12_RECT{0, 0, WindowWidth, WindowHeight};
};

void DX12Engine::InitWindow(HINSTANCE HInstance)
{
    WNDCLASS WindowClassInfo = {};
    WindowClassInfo.hInstance = HInstance;
    WindowClassInfo.lpfnWndProc = CallBackFunc;
    WindowClassInfo.lpszClassName = L"004-Texture";

    RegisterClass(&WindowClassInfo);

    HWnd = CreateWindow(
        WindowClassInfo.lpszClassName,
        L"004-Texture",
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

bool DX12Engine::LoadTextureFromFile()
{
    if (mWICFactory == nullptr)
    {
        CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&mWICFactory));
    }

    HRESULT HResult = mWICFactory->CreateDecoderFromFilename(TextureFileName.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &mWICBitmapDecoder);

    std::wostringstream Output_Str;
    switch (HResult)
    {
        case S_OK: break;
        case HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND):
            Output_Str << L"File not found " << TextureFileName << L" Check file path";
            MessageBox(nullptr, Output_Str.str().c_str(), L"Error", MB_OK | MB_ICONERROR);
            return false;
        case HRESULT_FROM_WIN32(ERROR_FILE_CORRUPT):
            Output_Str << L"File " << TextureFileName << L" is being used";
            MessageBox(nullptr, Output_Str.str().c_str(), L"Error", MB_OK | MB_ICONERROR);
            return false;
        case WINCODEC_ERR_COMPONENTNOTFOUND:
            Output_Str << L"File " << TextureFileName << L" is invalid";
            MessageBox(nullptr, Output_Str.str().c_str(), L"Error", MB_OK | MB_ICONERROR);
            return false;
        default:
            Output_Str << L"File " << TextureFileName << L" decodes error " << HResult;
            MessageBox(nullptr, Output_Str.str().c_str(), L"Error", MB_OK | MB_ICONERROR);
            return false;
    }

    mWICBitmapDecoder->GetFrame(0, &mWICBitmapFrameDecode);

    WICPixelFormatGUID SourceFormat = {};
    GUID TargetFormat = {};
    
    mWICBitmapFrameDecode->GetPixelFormat(&SourceFormat);
    if (GetTargetPixelFormat(&SourceFormat, &TargetFormat))
    {
	    TextureFormat = GetDXGIFormatFromPixelFormat(&TargetFormat);
    }
    else
    {
	    ::MessageBox(nullptr, L"Invalid Texture Format", L"Hint", MB_OK);
    	return false;
    }

	mWICFactory->CreateFormatConverter(&mWICFormatConverter);
	mWICFormatConverter->Initialize(mWICBitmapFrameDecode.Get(), TargetFormat, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom);
	mWICFormatConverter.As(&mWICBitmapSource);

	mWICBitmapSource->GetSize(&TextureWidth, &TextureHeight);

	ComPtr<IWICComponentInfo> TempWICComponentInfo = {};
	ComPtr<IWICPixelFormatInfo> TempWICPixelInfo = {};
	mWICFactory->CreateComponentInfo(TargetFormat, &TempWICComponentInfo);
	TempWICComponentInfo.As(&TempWICPixelInfo);
	TempWICPixelInfo->GetBitsPerPixel(&BitsPerPixel);

	return true;
}

void DX12Engine::CreateSRVHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC SRVHeapDesc = {};
	SRVHeapDesc.NumDescriptors = 1;
	SRVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	SRVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	mD3D12Device->CreateDescriptorHeap(&SRVHeapDesc, IID_PPV_ARGS(&mSRVHeap));
}

inline UINT Ceil(UINT A, UINT B)
{
	return (A + B - 1) / B;
}

void DX12Engine::CreateUploadAndDefaultResource()
{
	BytePerRowSize = TextureWidth * BitsPerPixel / 8;
	TextureSize = BytePerRowSize * TextureHeight;

	UploadResourceRowSize = Ceil(BytePerRowSize, 256) * 256;
	UploadResourceSize = UploadResourceRowSize * (TextureHeight - 1) + BytePerRowSize;

	D3D12_RESOURCE_DESC UploadResouceDesc = {};
	UploadResouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	UploadResouceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	UploadResouceDesc.Width = UploadResourceSize;
	UploadResouceDesc.Height = 1;
	UploadResouceDesc.Format = DXGI_FORMAT_UNKNOWN;
	UploadResouceDesc.DepthOrArraySize = 1;
	UploadResouceDesc.MipLevels = 1;
	UploadResouceDesc.SampleDesc.Count = 1;

	D3D12_HEAP_PROPERTIES UploadHeapDesc = { D3D12_HEAP_TYPE_UPLOAD };

	mD3D12Device->CreateCommittedResource(&UploadHeapDesc, D3D12_HEAP_FLAG_NONE, &UploadResouceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mUplaodTextureResource));

	D3D12_RESOURCE_DESC DefaultResourceDesc = {};
	DefaultResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	DefaultResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	DefaultResourceDesc.Width = TextureWidth;
	DefaultResourceDesc.Height = TextureHeight;
	DefaultResourceDesc.Format = TextureFormat;
	DefaultResourceDesc.DepthOrArraySize = 1;
	DefaultResourceDesc.MipLevels = 1;
	DefaultResourceDesc.SampleDesc.Count = 1;

	D3D12_HEAP_PROPERTIES DefaultHeapDesc = { D3D12_HEAP_TYPE_DEFAULT };

	mD3D12Device->CreateCommittedResource(&DefaultHeapDesc, D3D12_HEAP_FLAG_NONE, &DefaultResourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&mDefaultTextureResource));
}

void DX12Engine::CopyTextureDataToDefaultResource()
{
	BYTE* TextureData = (BYTE*)malloc(TextureSize);
	mWICBitmapSource->CopyPixels(nullptr, BytePerRowSize, TextureSize, TextureData);
	BYTE* TransferPointer = nullptr;

	mUplaodTextureResource->Map(0, nullptr, reinterpret_cast<void**>(&TransferPointer));

	for (UINT i = 0; i < TextureHeight; ++i)
	{
		memcpy(TransferPointer, TextureData, BytePerRowSize);
		TextureData += BytePerRowSize;
		TransferPointer += UploadResourceRowSize;
	}
	mUplaodTextureResource->Unmap(0, nullptr);

	TextureData -= TextureSize;
	free(TextureData);

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT PlacedFootPrint = {};
	D3D12_RESOURCE_DESC DefaultResourceDesc = mDefaultTextureResource->GetDesc();

	mD3D12Device->GetCopyableFootprints(&DefaultResourceDesc, 0, 1, 0, &PlacedFootPrint, nullptr, nullptr, nullptr);

	D3D12_TEXTURE_COPY_LOCATION DstLocation = {};
	DstLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	DstLocation.SubresourceIndex = 0;
	DstLocation.pResource = mDefaultTextureResource.Get();

	D3D12_TEXTURE_COPY_LOCATION SrcLocation = {};
	SrcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	SrcLocation.PlacedFootprint = PlacedFootPrint;
	SrcLocation.pResource = mUplaodTextureResource.Get();

	mCommandAllocator->Reset();
	mCommandList->Reset(mCommandAllocator.Get(), nullptr);

	mCommandList->CopyTextureRegion(&DstLocation, 0, 0, 0, &SrcLocation, nullptr);
	mCommandList->Close();

	ID3D12CommandList* TempCommandLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(1, TempCommandLists);

	FenceValue++;
	mCommandQueue->Signal(mFence.Get(), FenceValue);
	mFence->SetEventOnCompletion(FenceValue, RenderEvent);
}

void DX12Engine::CreateSRV()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC SRVDescriptorDesc = {};
	SRVDescriptorDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	SRVDescriptorDesc.Format = TextureFormat;
	SRVDescriptorDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	SRVDescriptorDesc.Texture2D.MipLevels = 1;

	SRV_CPUHandle = mSRVHeap->GetCPUDescriptorHandleForHeapStart();
	mD3D12Device->CreateShaderResourceView(mDefaultTextureResource.Get(), &SRVDescriptorDesc, SRV_CPUHandle);
	SRV_GPUHandle = mSRVHeap->GetGPUDescriptorHandleForHeapStart();
}

void DX12Engine::CreateRootSignature()
{
    ComPtr<ID3DBlob> SignatureBlob;
    ComPtr<ID3DBlob> ErrorBlob;

	D3D12_DESCRIPTOR_RANGE SRVDescriptorRangeDesc = {};
	SRVDescriptorRangeDesc.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	SRVDescriptorRangeDesc.NumDescriptors = 1;
	SRVDescriptorRangeDesc.BaseShaderRegister = 0;
	SRVDescriptorRangeDesc.RegisterSpace = 0;
	SRVDescriptorRangeDesc.OffsetInDescriptorsFromTableStart = 0;

	D3D12_ROOT_DESCRIPTOR_TABLE RootDescriptorTableDesc = {};
	RootDescriptorTableDesc.pDescriptorRanges = &SRVDescriptorRangeDesc;
	RootDescriptorTableDesc.NumDescriptorRanges = 1;

	D3D12_ROOT_PARAMETER RootParameter = {};
	RootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	RootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	RootParameter.DescriptorTable = RootDescriptorTableDesc;

	D3D12_STATIC_SAMPLER_DESC StaticSamplerDesc= {};
	StaticSamplerDesc.ShaderRegister = 0;
	StaticSamplerDesc.RegisterSpace = 0;
	StaticSamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	StaticSamplerDesc.Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	StaticSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	StaticSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	StaticSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	StaticSamplerDesc.MinLOD = 0;
	StaticSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	StaticSamplerDesc.MipLODBias = 0;
	StaticSamplerDesc.MaxAnisotropy = 1;
	StaticSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;

    // Create Struct
    D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc = {};
    RootSignatureDesc.NumParameters = 1;
    RootSignatureDesc.pParameters = &RootParameter;
    RootSignatureDesc.NumStaticSamplers = 1;
    RootSignatureDesc.pStaticSamplers = &StaticSamplerDesc;
    RootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &SignatureBlob, &ErrorBlob);
    if (ErrorBlob)
    {
        OutputDebugStringA((const char*)ErrorBlob->GetBufferPointer());
        OutputDebugStringA("\n");
    }

    mD3D12Device->CreateRootSignature(0, SignatureBlob->GetBufferPointer(), SignatureBlob->GetBufferSize(), IID_PPV_ARGS(&mRootSignature));
}

void DX12Engine::CreatePSO()
{
    D3D12_GRAPHICS_PIPELINE_STATE_DESC PSODesc = {};

    // Prepare Input Elements
    D3D12_INPUT_ELEMENT_DESC InputElementDescs[2] = {};
    InputElementDescs[0].SemanticName = "POSITION";
    InputElementDescs[0].SemanticIndex = 0;
    InputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    InputElementDescs[0].InputSlot = 0;
    InputElementDescs[0].AlignedByteOffset = 0;
    InputElementDescs[0].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
    InputElementDescs[0].InstanceDataStepRate = 0;

    InputElementDescs[1].SemanticName = "TEXCOORD";
    InputElementDescs[1].SemanticIndex = 0;
    InputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    InputElementDescs[1].InputSlot = 0;
    InputElementDescs[1].AlignedByteOffset = 16;
    InputElementDescs[1].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
    InputElementDescs[1].InstanceDataStepRate = 0;

    // Prepare Layouts
    D3D12_INPUT_LAYOUT_DESC InputLayoutDesc = {};
    InputLayoutDesc.NumElements = 2;
    InputLayoutDesc.pInputElementDescs = InputElementDescs;
    PSODesc.InputLayout = InputLayoutDesc;

    // Prepare shaders
    ComPtr<ID3DBlob> VertexShaderBlob;
    ComPtr<ID3DBlob> PixelShaderBlob;
    ComPtr<ID3DBlob> ErrorBlob;
    D3DCompileFromFile(L"004-Shader.hlsl", nullptr, nullptr, "VSMain", "vs_5_1", 0, 0, &VertexShaderBlob, &ErrorBlob);
    if (ErrorBlob)
    {
        OutputDebugStringA((const char*)ErrorBlob->GetBufferPointer());
        OutputDebugStringA("\n");
    }
    D3DCompileFromFile(L"004-Shader.hlsl", nullptr, nullptr, "PSMain", "ps_5_1", 0, 0, &PixelShaderBlob, &ErrorBlob);
    if (ErrorBlob)
    {
        OutputDebugStringA((const char*)ErrorBlob->GetBufferPointer());
        OutputDebugStringA("\n");
    }

    PSODesc.VS.pShaderBytecode = VertexShaderBlob->GetBufferPointer();
    PSODesc.VS.BytecodeLength = VertexShaderBlob->GetBufferSize();
    PSODesc.PS.pShaderBytecode = PixelShaderBlob->GetBufferPointer();
    PSODesc.PS.BytecodeLength = PixelShaderBlob->GetBufferSize();

    PSODesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
    PSODesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;

    PSODesc.pRootSignature = mRootSignature.Get();

    PSODesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    PSODesc.NumRenderTargets = 1;
    PSODesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    PSODesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    PSODesc.SampleDesc.Count = 1;
    PSODesc.SampleMask = UINT_MAX;

    mD3D12Device->CreateGraphicsPipelineState(&PSODesc, IID_PPV_ARGS(&mPipelineStateObject));
}

void DX12Engine::CreateVertexResource()
{
	VERTEX vertexes[6] =
	{
		{{-0.75f, 0.75f, 0.0f, 1.0f}, {0.0f, 0.0f}},
		{{0.75f, 0.75f, 0.0f, 1.0f}, {1.0f, 0.0f}},
		{{0.75f, -0.75f, 0.0f, 1.0f}, {1.0f, 1.0f}},
		{{-0.75f, 0.75f, 0.0f, 1.0f}, {0.0f, 0.0f}},
		{{0.75f, -0.75f, 0.0f, 1.0f}, {1.0f, 1.0f}},
		{{-0.75f, -0.75f, 0.0f, 1.0f}, {0.0f, 1.0f}}
	};

    D3D12_RESOURCE_DESC VertexBufferDesc = {};
    VertexBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    VertexBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    VertexBufferDesc.Width = sizeof(vertexes);
    VertexBufferDesc.Height = 1;
    VertexBufferDesc.Format = DXGI_FORMAT_UNKNOWN;
    VertexBufferDesc.DepthOrArraySize = 1;
    VertexBufferDesc.MipLevels = 1;
    VertexBufferDesc.SampleDesc.Count = 1;

    D3D12_HEAP_PROPERTIES UploadHeapDesc = { D3D12_HEAP_TYPE_UPLOAD };

    mD3D12Device->CreateCommittedResource(&UploadHeapDesc, D3D12_HEAP_FLAG_NONE, &VertexBufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mVertexResource));

    BYTE* TransferPointer = nullptr;
    mVertexResource->Map(0, nullptr, reinterpret_cast<void**>(&TransferPointer));
    memcpy(TransferPointer, vertexes, sizeof(vertexes));
    mVertexResource->Unmap(0, nullptr);

    mVertexBufferView.BufferLocation = mVertexResource->GetGPUVirtualAddress();
    mVertexBufferView.SizeInBytes = sizeof(vertexes);
    mVertexBufferView.StrideInBytes = sizeof(VERTEX);
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

    // Check RootSignature
    mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

    // Set PSO
    mCommandList->SetPipelineState(mPipelineStateObject.Get());

    // Rasterize
    mCommandList->RSSetViewports(1, &mViewport);
    mCommandList->RSSetScissorRects(1, &mScissorRect);

    // Set RT
    mCommandList->OMSetRenderTargets(1, &RTVHandle, false, nullptr);

    // Clear RT
    mCommandList->ClearRenderTargetView(RTVHandle, DirectX::Colors::SkyBlue, 0, nullptr);


	ID3D12DescriptorHeap* _temp_DescriptorHeaps[] = { mSRVHeap.Get() };
	mCommandList->SetDescriptorHeaps(1, _temp_DescriptorHeaps);
	mCommandList->SetGraphicsRootDescriptorTable(0, SRV_GPUHandle);
	
    // Draw Command
    mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    mCommandList->IASetVertexBuffers(0, 1, &mVertexBufferView);
    mCommandList->DrawInstanced(6, 1, 0, 0);
    
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
            Sleep(10);
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

	Engine.LoadTextureFromFile();
	Engine.CreateSRVHeap();
	Engine.CreateUploadAndDefaultResource();
	Engine.CopyTextureDataToDefaultResource();
	Engine.CreateSRV();
	
    Engine.CreateRootSignature();
    Engine.CreatePSO();
    Engine.CreateVertexResource();
    
    Engine.RenderLoop();
}

int WINAPI WinMain(HINSTANCE HInstance, HINSTANCE HPrev, LPSTR CmdLine, int CmdShow)
{
    DX12Engine::Run(HInstance);
}
