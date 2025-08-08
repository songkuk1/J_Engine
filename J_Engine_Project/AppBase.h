#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <iostream>
#include <vector>
#include <windows.h>
#include <wrl.h> // ComPtr

namespace JEngine {

    using Microsoft::WRL::ComPtr;
    using std::vector;
    using std::wstring;

    // ��� �������� ���������� ����� ��ɵ��� ������ �ִ�
    // �θ� Ŭ����
    class AppBase {
    public:
        AppBase();
        virtual ~AppBase();

        float GetAspectRatio() const;

        int Run();

        virtual bool Initialize();


        virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

        // Convenience overrides for handling mouse input.
        virtual void OnMouseDown(WPARAM btnState, int x, int y) {};
        virtual void OnMouseUp(WPARAM btnState, int x, int y) {};
        virtual void OnMouseMove(WPARAM btnState, int x, int y) {};

    protected: // ��� ���� Ŭ���������� ���� ����
        bool InitMainWindow();
        bool InitDirect3D();
        bool InitGUI();
        void CreateVertexShaderAndInputLayout(
            const wstring& filename,
            const vector<D3D11_INPUT_ELEMENT_DESC>& inputElements,
            ComPtr<ID3D11VertexShader>& vertexShader,
            ComPtr<ID3D11InputLayout>& inputLayout);
        void CreatePixelShader(const wstring& filename,
            ComPtr<ID3D11PixelShader>& pixelShader);
        void CreateIndexBuffer(const vector<uint16_t>& indices,
            ComPtr<ID3D11Buffer>& m_indexBuffer);

        template <typename T_VERTEX>
        void CreateVertexBuffer(const vector<T_VERTEX>& vertices,
            ComPtr<ID3D11Buffer>& vertexBuffer) {

            // D3D11_USAGE enumeration (d3d11.h)
            // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ne-d3d11-d3d11_usage

            D3D11_BUFFER_DESC bufferDesc;
            ZeroMemory(&bufferDesc, sizeof(bufferDesc));
            bufferDesc.Usage = D3D11_USAGE_IMMUTABLE; // �ʱ�ȭ �� ����X
            bufferDesc.ByteWidth = UINT(sizeof(T_VERTEX) * vertices.size());
            bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            bufferDesc.CPUAccessFlags = 0; // 0 if no CPU access is necessary.
            bufferDesc.StructureByteStride = sizeof(T_VERTEX);

            D3D11_SUBRESOURCE_DATA vertexBufferData = {
                0 }; // MS �������� �ʱ�ȭ�ϴ� ���
            vertexBufferData.pSysMem = vertices.data();
            vertexBufferData.SysMemPitch = 0;
            vertexBufferData.SysMemSlicePitch = 0;

            const HRESULT hr = m_device->CreateBuffer(
                &bufferDesc, &vertexBufferData, vertexBuffer.GetAddressOf());
            if (FAILED(hr)) {
                std::cout << "CreateBuffer() failed. " << std::hex << hr
                    << std::endl;
            };
        }

        template <typename T_CONSTANT>
        void CreateConstantBuffer(const T_CONSTANT& constantBufferData,
            ComPtr<ID3D11Buffer>& constantBuffer) {
            // ����:
            // For a constant buffer (BindFlags of D3D11_BUFFER_DESC set to
            // D3D11_BIND_CONSTANT_BUFFER), you must set the ByteWidth value of
            // D3D11_BUFFER_DESC in multiples of 16, and less than or equal to
            // D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT.

            D3D11_BUFFER_DESC cbDesc;
            cbDesc.ByteWidth = sizeof(constantBufferData);
            cbDesc.Usage = D3D11_USAGE_DYNAMIC;
            cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            cbDesc.MiscFlags = 0;
            cbDesc.StructureByteStride = 0;

            // Fill in the subresource data.
            D3D11_SUBRESOURCE_DATA initData;
            initData.pSysMem = &constantBufferData;
            initData.SysMemPitch = 0;
            initData.SysMemSlicePitch = 0;

            auto hr = m_device->CreateBuffer(&cbDesc, &initData,
                constantBuffer.GetAddressOf());
            if (FAILED(hr)) {
                std::cout << "CreateConstantBuffer() CreateBuffer failed()."
                    << std::endl;
            }
        }

        template <typename T_DATA>
        void UpdateBuffer(const T_DATA& bufferData, ComPtr<ID3D11Buffer>& buffer) {

            if (!buffer) {
                std::cout << "UpdateBuffer() buffer was not initialized."
                    << std::endl;
            }

            D3D11_MAPPED_SUBRESOURCE ms;
            m_context->Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
            memcpy(ms.pData, &bufferData, sizeof(bufferData));
            m_context->Unmap(buffer.Get(), NULL);
        }

        void CreateTexture(const std::string filename, ComPtr<ID3D11Texture2D>& texture,
            ComPtr<ID3D11ShaderResourceView>& textureResourceView);

    public:
        // ���� �̸� ���̴� ��Ģ�� VS DX11/12 �⺻ ���ø��� �����ϴ�.
        // �ٸ� ���� �̸��� ���̱� ���� d3d�� �����߽��ϴ�.
        // ��: m_d3dDevice -> m_device
        int m_screenWidth; // �������� ���� ȭ���� �ػ�
        int m_screenHeight;
        HWND m_mainWindow;

        ComPtr<ID3D11Device> m_device;
        ComPtr<ID3D11DeviceContext> m_context;
        ComPtr<ID3D11RenderTargetView> m_renderTargetView;
        ComPtr<IDXGISwapChain> m_swapChain;
        ComPtr<ID3D11RasterizerState> m_rasterizerSate;

        // Depth buffer ����
        ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
        ComPtr<ID3D11DepthStencilView> m_depthStencilView;
        ComPtr<ID3D11DepthStencilState> m_depthStencilState;

        D3D11_VIEWPORT m_screenViewport;
    };
} // namespace hlab