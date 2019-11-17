#pragma once


class Graphics
{
public:
    enum TextureSlot
    {
        TEXTURE_SLOT_DIFFUSE,
		TEXTURE_SLOT_NORMAL,
		TEXTURE_SLOT_TOTAL
    };

	enum ConstantBuffer
	{
		CONSTANT_BUFFER_CAMERA,
		CONSTANT_BUFFER_RENDEROBJ,
        CONSTANT_BUFFER_LIGHTING,
		CONSTANT_BUFFER_SKINNING,
	};

    class Color4
    {
    public:
        Color4()
            : r(0), g(0), b(0), a(0)
        {}
        Color4(float _r, float _g, float _b, float _a = 1.0f)
            : r(_r), g(_g), b(_b), a(_a)
        {}
        float r, g, b, a;
    };

	bool CreateDepthStencil(int inWidth, int inHeight, ID3D11Texture2D** pDepthTexture, ID3D11DepthStencilView** pDepthView);
	ID3D11DepthStencilState* CreateDepthStencilState(bool inDepthTestEnable, D3D11_COMPARISON_FUNC inDepthComparisonFunction);
    Graphics();
    ~Graphics();

    void InitD3D(HWND hWnd, float width, float height);
    void BeginFrame(const Color4 &clearColor);
    void EndFrame();
    void CleanD3D();
	float GetScreenWidth() const { return mScreenWidth; }
	float GetScreenHeight() const { return mScreenHeight; }
	ID3D11Device* GetDevice() { return mDev; }
	ID3D11DeviceContext* GetDeviceContext() { return mDevCon; }
	void UploadBuffer(ID3D11Buffer* buffer, const void* data, size_t dataSize);
	ID3D11Buffer* CreateGraphicsBuffer(const void* initialData, int inDataSize, D3D11_BIND_FLAG inBindFlags, D3D11_CPU_ACCESS_FLAG inCPUAccessFlags, D3D11_USAGE inUsage);
	void SetActiveTexture(int slot, ID3D11ShaderResourceView* pView);
	void SetActiveSampler(int slot, ID3D11SamplerState* pSampler);
	void SetRenderTarget(ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthView);
	ID3D11RenderTargetView* GetBackBuffer();
	ID3D11DepthStencilView* GetDepthBuffer();
	void SetViewport(float x, float y, float width, float height);

	ID3D11BlendState* CreateBlendState(bool enable, D3D11_BLEND srcBlend, D3D11_BLEND dstBlend);
	void SetBlendState(ID3D11BlendState* inBlendState);

private:
    float mScreenWidth;
    float mScreenHeight;

	ID3D11RenderTargetView* currentRenderTarget;
	ID3D11DepthStencilView* currentDepthView;

	IDXGISwapChain* mSwapChain;
	ID3D11Device* mDev;
	ID3D11DeviceContext* mDevCon;
	ID3D11RenderTargetView* mBackBuffer;
	ID3D11Texture2D* mDepthStencilTexture;
	ID3D11DepthStencilView* mDepthStencilView;
	ID3D11DepthStencilState* mDepthState;
	ID3D11SamplerState* mDefaultSampler;
};