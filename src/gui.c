#include <d3d9.h>

const int width = 500;
const int height = 300;

BOOL isExit = TRUE;

HWND window = NULL;
WNDCLASSEXA windowClass = { };

POINTS position = { };

PDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 device = NULL;
D3DPRESENT_PARAMETERS presentParameters = { };

void
ResetDevice()
{
    HRESULT result = device->lpVtbl->Reset(device, &presentParameters);
    //    if (result == D3DERR_INVALIDCALL);
}

void
DestroyHWindow()
{
    DestroyWindow(window);
    UnregisterClassA(windowClass.lpszClassName, windowClass.hInstance);
}

BOOL
CreateDevice()
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (!d3d) return FALSE;

    ZeroMemory(&presentParameters, sizeof(presentParameters));

    presentParameters.Windowed = TRUE;
    presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
    presentParameters.EnableAutoDepthStencil = TRUE;
    presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
    presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    HRESULT result = d3d->lpVtbl->CreateDevice(
            d3d, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentParameters, &device
    );
    if (result < 0)
    {
        return FALSE;
    }

    return TRUE;
}

void
DestroyDevice()
{
    if (device)
    {
        device->lpVtbl->Release(device);
        device = NULL;
    }

    if (d3d)
    {
        d3d->lpVtbl->Release(d3d);
        d3d = NULL;
    }
}

void
CreateGui()
{

}

void
DestroyGui()
{

}

void
BeginRender()
{
    MSG message;
    while (PeekMessageA(&message, 0, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
}

void
EndRender()
{
    device->lpVtbl->SetRenderState(device, D3DRS_ZENABLE, FALSE);
    device->lpVtbl->SetRenderState(device, D3DRS_ALPHABLENDENABLE, FALSE);
    device->lpVtbl->SetRenderState(device, D3DRS_SCISSORTESTENABLE, FALSE);

    device->lpVtbl->Clear(device, 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

    if (device->lpVtbl->BeginScene(device) >= 0)
    {
        device->lpVtbl->EndScene(device);
    }

    HRESULT result = device->lpVtbl->Present(0, 0, 0, 0);
    if (result == D3DERR_DEVICELOST && device->lpVtbl->TestCooperativeLevel(device) == D3DERR_DEVICENOTRESET) ResetDevice();
}

void
Render()
{

}

long long __stdcall
WindowProcess(HWND pWindow, UINT message, WPARAM wideParam, LPARAM longParam)
{

    switch (message)
    {
        case WM_SIZE:
        {
            if (device && wideParam != SIZE_MINIMIZED)
            {
                presentParameters.BackBufferWidth = LOWORD(longParam);
                presentParameters.BackBufferHeight = HIWORD(longParam);
                ResetDevice();
            }
        }
            return 0;

        case WM_SYSCOMMAND:
        {
            if ((wideParam & 0xfff0) == SC_KEYMENU)
            { return 0; }
        }
            break;

        case WM_DESTROY:
        {
            PostQuitMessage(0);
        }
            return 0;

        case WM_LBUTTONDOWN:
        {
            position = MAKEPOINTS(longParam);
        }
            return 0;

        case WM_MOUSEMOVE:
        {
            if (wideParam == MK_LBUTTON)
            {
                POINTS points = MAKEPOINTS(longParam);
                RECT rect;
                GetWindowRect(window, &rect);

                rect.left += points.x - position.x;
                rect.top += points.y - position.x;

                if (position.x >= 0 && position.x <= width && position.y >= 0 && position.y <= 19)
                {
                    SetWindowPos(window, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
                }
            }
        }
            return 0;
    }

    return DefWindowProcW(window, message, wideParam, longParam);
}

void
CreateHWindow(const char *windowName, const char *className)
{
    windowClass.cbSize = sizeof(WNDCLASSEXA);
    windowClass.style = CS_CLASSDC;
    windowClass.lpfnWndProc = WindowProcess;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = GetModuleHandleA(0);
    windowClass.hIcon = 0;
    windowClass.hCursor = 0;
    windowClass.hbrBackground = 0;
    windowClass.lpszMenuName = 0;
    windowClass.lpszClassName = className;
    windowClass.hIconSm = 0;

    RegisterClassExA(&windowClass);

    window = CreateWindowA(className, windowName, WS_POPUP, 100, 100, width, height, 0, 0, windowClass.hInstance, 0);

    ShowWindow(window, SW_SHOWDEFAULT);
    UpdateWindow(window);
}

int
main()
{

}