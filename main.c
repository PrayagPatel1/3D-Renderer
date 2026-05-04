#define COBJMACROS // Alows us to use COM interface methods via macro functions.

#include <stdio.h>
#include <windows.h>
#include <initguid.h>
#include <d3d11.h>
#include <dxgi.h>

// === Global variables for Direct3D 11 ===
static ID3D11Device *device = NULL;
static ID3D11DeviceContext *context = NULL;
static IDXGISwapChain *swap_chain = NULL;
static ID3D11RenderTargetView *rtv = NULL; // allows the graphics pipeline to
// output rendered results to the swap chain's back buffer.

// === Initialize the Direct3D 11 API ===
int InitD3D11(HWND hwnd, u_int width, u_int height)
{
    // === Defining characteristic of the swap chain ===
    DXGI_SWAP_CHAIN_DESC swap_chain_desc;
    ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));

    swap_chain_desc.BufferCount = 1;
    swap_chain_desc.BufferDesc.Width = width;
    swap_chain_desc.BufferDesc.Height = height;
    swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_desc.OutputWindow = hwnd;
    swap_chain_desc.SampleDesc.Count = 1;
    swap_chain_desc.Windowed = TRUE;

    // === Create device and swap chain at the same time ===
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        0,
        NULL,
        0,
        D3D11_SDK_VERSION,
        &swap_chain_desc,
        &swap_chain,
        &device,
        NULL,
        &context);

    if (FAILED(hr))
    {
        return 0;
    }

    // === Get the back buffer ===
    ID3D11Texture2D *back_buffer = NULL;

    hr = IDXGISwapChain_GetBuffer(
        swap_chain,
        0,
        &IID_ID3D11Texture2D,
        (void **)&back_buffer);

    if (FAILED(hr))
    {
        return 0;
    }

    // === Create render target view (RTV) via back buffer ===
    hr = ID3D11Device_CreateRenderTargetView(
        device,
        (ID3D11Resource *)back_buffer,
        NULL,
        &rtv);

    ID3D11Texture2D_Release(back_buffer);

    if (FAILED(hr))
    {
        return 0;
    }

    ID3D11DeviceContext_OMSetRenderTargets(
        context,
        1,
        &rtv,
        NULL);

    // === Setup Viewport ===
    D3D11_VIEWPORT view_port;

    view_port.Width = (FLOAT)width;
    view_port.Height = (FLOAT)height;
    view_port.MaxDepth = 1.0f;
    view_port.MinDepth = 0.0f;
    view_port.TopLeftX = 0.0f;
    view_port.TopLeftY = 0.0f;

    ID3D11DeviceContext_RSSetViewports(context, 1, &view_port);
    return 1;
}

// === Render ===
void Render()
{
    // Clears the render target view with a cornflower blue color
    float color[4] = {0.39f, 0.58f, 0.93f, 1.0f};
    ID3D11DeviceContext_ClearRenderTargetView(context, rtv, color);
    IDXGISwapChain_Present(swap_chain, 1, 0);
}

// === Cleanup Direct3d 11 interfaces / objects / resources ===
void Cleanup()
{
    if (rtv)
    {
        ID3D11RenderTargetView_Release(rtv);
    }
    if (swap_chain)
    {
        IDXGISwapChain_Release(swap_chain);
    }
    if (context)
    {
        ID3D11DeviceContext_Release(context);
    }
    if (device)
    {
        ID3D11Device_Release(device);
    }
}

// === Window Procedure Function ===
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

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
    wc.lpfnWndProc = WindowProc;
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

    InitD3D11(window_handle, 800, 600);

    ShowWindow(window_handle, nCmdShow);

    // === Message Loop ===
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            Render(); // Happens every frame
        }
    }

    Cleanup();
    return 0;
}