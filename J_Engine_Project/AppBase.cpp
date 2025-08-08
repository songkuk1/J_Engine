#include "AppBase.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <dxgi.h>    // DXGIFactory
#include <dxgi1_4.h> // DXGIFactory4

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam);

namespace JEngine
{

	using namespace std;

    AppBase* g_appBase = nullptr;

    LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
    {

        // g_appBase를 이용해서 간접적으로 멤버 함수 호출
        return g_appBase->MsgProc(hWnd, msg, wParam, lParam);
    }

    // 생성자
	AppBase::AppBase() : m_screenHeight(720),
		m_screenWidth(1280),
		m_mainWindow(0), m_screenViewport(D3D11_VIEWPORT())
    {

        g_appBase = this;
    }

    AppBase::~AppBase() {
        g_appBase = nullptr;

        // Cleanup(GUI관련)
        //ImGui_ImplDX11_Shutdown();
        //ImGui_ImplWin32_Shutdown();
        //ImGui::DestroyContext();

        DestroyWindow(m_mainWindow);
        // UnregisterClass(wc.lpszClassName, wc.hInstance);//생략

        // COMPtr에서 알아서 release
        // ComPtr automatically maintains a reference count for the underlying
        // interface pointer and releases the interface when the reference count
        // goes to zero.
        // https:learn.microsoft.com/en-us/cpp/cppcx/wrl/comptr-class?view=msvc-170
        // 예시: m_d3dDevice.Reset(); 생략
    }

    bool AppBase::InitMainWindow()
    {

        WNDCLASSEX wc = { sizeof(WNDCLASSEX),
                         CS_CLASSDC,
                         WndProc,
                         0L,
                         0L,
                         GetModuleHandle(NULL),
                         NULL,
                         NULL,
                         NULL,
                         NULL,
                         L"JEngineWindow", // lpszClassName, L-string
                         NULL };

        // The RegisterClass function has been superseded by the RegisterClassEx
        // function.
        // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassa?redirectedfrom=MSDN
        if (!RegisterClassEx(&wc)) {
            cout << "RegisterClassEx() failed." << endl;
            return false;
        }

        //해상도
        RECT wr = { 0, 0, m_screenWidth, m_screenHeight };

        // 필요한 윈도우 크기(해상도) 계산
        // wr의 값이 바뀜
        AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, false);

        // 윈도우를 만들때 위에서 계산한 wr 사용
        m_mainWindow = CreateWindow(wc.lpszClassName, L"JEngineWindow Example",
            WS_OVERLAPPEDWINDOW,
            100, // 윈도우 좌측 상단의 x 좌표
            100, // 윈도우 좌측 상단의 y 좌표
            wr.right - wr.left, // 윈도우 가로 방향 해상도
            wr.bottom - wr.top, // 윈도우 세로 방향 해상도
            NULL, NULL, wc.hInstance, NULL);

        if (!m_mainWindow) {
            cout << "CreateWindow() failed." << endl;
            return false;
        }

        ShowWindow(m_mainWindow, SW_SHOWDEFAULT);
        UpdateWindow(m_mainWindow);

        return true;
    }
    LRESULT AppBase::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
    {

        switch (msg) 
        {
            //창의 크기가 변경되었을때
        case WM_SIZE:
            // Reset and resize swapchain
            break;
            //alt키를 누르거나 시스템 메뉴같은 시스템 명령이 발생했을때
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
            break;
            //마우스가 윈도우 내부에서 이동 할 때마다 발생
        case WM_MOUSEMOVE:
             cout << "Mouse " << LOWORD(lParam) << " " << HIWORD(lParam) << endl;
            break;
            //마우스왼쪽 떼기
        case WM_LBUTTONUP:
             cout << "WM_LBUTTONUP Left mouse button" << endl;
            break;
            //마우스오른쪽버튼 떼기
        case WM_RBUTTONUP:
             cout << "WM_RBUTTONUP Right mouse button" << endl;
            break;
            //키보드누르기
        case WM_KEYDOWN:
             cout << "WM_KEYDOWN " << (int)wParam << endl;
            break;
            //창이닫힐대
        case WM_DESTROY:
            ::PostQuitMessage(0);
            return 0;
        }

        return ::DefWindowProc(hwnd, msg, wParam, lParam);
    }

    bool AppBase::Initialize()
    {

        if (!InitMainWindow())
            return false;


        return true;
    }
   
    int AppBase::Run() {

        // Main message loop
        MSG msg = { 0 };
        while (WM_QUIT != msg.message)
        {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

        }

        return 0;
    }



}