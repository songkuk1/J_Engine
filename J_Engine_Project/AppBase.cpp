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

        // g_appBase�� �̿��ؼ� ���������� ��� �Լ� ȣ��
        return g_appBase->MsgProc(hWnd, msg, wParam, lParam);
    }

    // ������
	AppBase::AppBase() : m_screenHeight(720),
		m_screenWidth(1280),
		m_mainWindow(0), m_screenViewport(D3D11_VIEWPORT())
    {

        g_appBase = this;
    }

    AppBase::~AppBase() {
        g_appBase = nullptr;

        // Cleanup(GUI����)
        //ImGui_ImplDX11_Shutdown();
        //ImGui_ImplWin32_Shutdown();
        //ImGui::DestroyContext();

        DestroyWindow(m_mainWindow);
        // UnregisterClass(wc.lpszClassName, wc.hInstance);//����

        // COMPtr���� �˾Ƽ� release
        // ComPtr automatically maintains a reference count for the underlying
        // interface pointer and releases the interface when the reference count
        // goes to zero.
        // https:learn.microsoft.com/en-us/cpp/cppcx/wrl/comptr-class?view=msvc-170
        // ����: m_d3dDevice.Reset(); ����
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

        //�ػ�
        RECT wr = { 0, 0, m_screenWidth, m_screenHeight };

        // �ʿ��� ������ ũ��(�ػ�) ���
        // wr�� ���� �ٲ�
        AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, false);

        // �����츦 ���鶧 ������ ����� wr ���
        m_mainWindow = CreateWindow(wc.lpszClassName, L"JEngineWindow Example",
            WS_OVERLAPPEDWINDOW,
            100, // ������ ���� ����� x ��ǥ
            100, // ������ ���� ����� y ��ǥ
            wr.right - wr.left, // ������ ���� ���� �ػ�
            wr.bottom - wr.top, // ������ ���� ���� �ػ�
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
            //â�� ũ�Ⱑ ����Ǿ�����
        case WM_SIZE:
            // Reset and resize swapchain
            break;
            //altŰ�� �����ų� �ý��� �޴����� �ý��� ����� �߻�������
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
            break;
            //���콺�� ������ ���ο��� �̵� �� ������ �߻�
        case WM_MOUSEMOVE:
             cout << "Mouse " << LOWORD(lParam) << " " << HIWORD(lParam) << endl;
            break;
            //���콺���� ����
        case WM_LBUTTONUP:
             cout << "WM_LBUTTONUP Left mouse button" << endl;
            break;
            //���콺�����ʹ�ư ����
        case WM_RBUTTONUP:
             cout << "WM_RBUTTONUP Right mouse button" << endl;
            break;
            //Ű���崩����
        case WM_KEYDOWN:
             cout << "WM_KEYDOWN " << (int)wParam << endl;
            break;
            //â�̴�����
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