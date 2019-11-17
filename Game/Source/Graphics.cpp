#include "stdafx.h"
#include "Graphics.h"
#include "Shader.h"
#include "engineMath.h"

#pragma comment (lib, "d3d11.lib") 

Graphics::Graphics()
    : mScreenWidth(0)
    , mScreenHeight(0)
{
}

Graphics::~Graphics()
{
}

void Graphics::SetRenderTarget(ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthView)
{
	currentRenderTarget = renderTarget;
	currentDepthView = depthView;
	mDevCon->OMSetRenderTargets(1, &renderTarget, depthView);
}

bool Graphics::CreateDepthStencil(int inWidth, int inHeight, ID3D11Texture2D** pDepthTexture, ID3D11DepthStencilView** pDepthView)
{
	D3D11_TEXTURE2D_DESC tdesc;
	ZeroMemory(&tdesc, sizeof(tdesc));
	tdesc.Width = inWidth;
	tdesc.Height = inHeight;
	tdesc.MipLevels = 1;
	tdesc.ArraySize = 1;
	tdesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tdesc.SampleDesc.Count = 1;
	tdesc.Usage = D3D11_USAGE_DEFAULT;
	tdesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	HRESULT result = mDev->CreateTexture2D(&tdesc, NULL, pDepthTexture);
	if (FAILED(result))
	{
		return false;
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC ddesc;
	ZeroMemory(&ddesc, sizeof(ddesc));
	ddesc.Format = tdesc.Format;
	ddesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	ddesc.Texture2D.MipSlice = 0;
	result = mDev->CreateDepthStencilView(*pDepthTexture, &ddesc, pDepthView);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}


ID3D11DepthStencilState* Graphics::CreateDepthStencilState(bool inDepthTestEnable, D3D11_COMPARISON_FUNC inDepthComparisonFunction)
{
	D3D11_DEPTH_STENCIL_DESC dsdesc;
	dsdesc.DepthEnable = inDepthTestEnable;
	dsdesc.DepthEnable = true;
	dsdesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsdesc.DepthFunc = (D3D11_COMPARISON_FUNC) inDepthComparisonFunction;
	dsdesc.StencilEnable = false;
	dsdesc.StencilReadMask = 0xFF;
	dsdesc.StencilWriteMask = 0xFF;
	dsdesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsdesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsdesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsdesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsdesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsdesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsdesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsdesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	ID3D11DepthStencilState* dsState = NULL;
	HRESULT result = mDev->CreateDepthStencilState(&dsdesc, &dsState);
	if (FAILED(result))
	{
		return NULL;
	}
	return dsState;
}

void Graphics::SetBlendState(ID3D11BlendState* inBlendState)
{
	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	mDevCon->OMSetBlendState(inBlendState, blendFactor, 0xffffffff);
}

void Graphics::InitD3D(HWND hWnd, float width, float height)
{

    mScreenWidth = width;
    mScreenHeight = height;

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1;
	scd.BufferDesc.Width = (UINT)mScreenWidth;
	scd.BufferDesc.Height = (UINT)mScreenHeight;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 1;
	scd.Windowed = TRUE;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&mSwapChain,
		&mDev,
		NULL,
		&mDevCon
	);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = mScreenWidth;
	viewport.Height = mScreenHeight;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;
	mDevCon->RSSetViewports((UINT)1, &viewport);

	ID3D11Texture2D* pBackBuffer;
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	DbgAssert(hr == S_OK, "Something wrong with your back buffer");
	mDev->CreateRenderTargetView(pBackBuffer, NULL, &mBackBuffer);
	pBackBuffer->Release();
	currentRenderTarget = mBackBuffer;

	CreateDepthStencil((int)width, (int)height, &mDepthStencilTexture, &mDepthStencilView);
	mDepthState = CreateDepthStencilState(true, D3D11_COMPARISON_LESS_EQUAL);
	currentDepthView = mDepthStencilView;

	mDevCon->OMSetRenderTargets(1, &mBackBuffer, mDepthStencilView);
	mDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDevCon->OMSetDepthStencilState(mDepthState, 1);

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = mDev->CreateSamplerState(&sampDesc, &mDefaultSampler);
	DbgAssert(hr == S_OK, "Something wrong with your smapler state creation");
	SetActiveSampler(0, mDefaultSampler);

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = true;
	ID3D11RasterizerState* rasterizerState;
	mDev->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
	mDevCon->RSSetState(rasterizerState);
	rasterizerState->Release();

}


void Graphics::SetActiveSampler(int slot, ID3D11SamplerState* pSampler)
{
	mDevCon->PSSetSamplers(slot, 1, &pSampler);
}

void Graphics::SetActiveTexture(int slot, ID3D11ShaderResourceView* pView)
{
	mDevCon->PSSetShaderResources(slot, 1, &pView);
}

void Graphics::UploadBuffer(ID3D11Buffer* buffer, const void* data, size_t dataSize)
{
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	mDevCon->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	memcpy(mappedSubresource.pData, data, dataSize);
	mDevCon->Unmap(buffer, 0);
}


ID3D11Buffer* Graphics::CreateGraphicsBuffer(const void* initialData, int inDataSize, D3D11_BIND_FLAG inBindFlags, D3D11_CPU_ACCESS_FLAG inCPUAccessFlags, D3D11_USAGE inUsage)
{
	ID3D11Buffer* buffer;

	D3D11_BUFFER_DESC bufferDesc = { 0 };
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = inUsage;
	bufferDesc.ByteWidth = inDataSize;
	bufferDesc.BindFlags = inBindFlags;
	bufferDesc.CPUAccessFlags = inCPUAccessFlags;	

	D3D11_SUBRESOURCE_DATA subresourceData;
	subresourceData.pSysMem = initialData;
	subresourceData.SysMemPitch = 0;
	subresourceData.SysMemSlicePitch = 0;

	mDev->CreateBuffer(&bufferDesc, &subresourceData, &buffer);

	if (initialData != NULL)
	{
		UploadBuffer(buffer, initialData, inDataSize);
	}

	return buffer;
}

void Graphics::BeginFrame(const Color4 &clearColor)
{
	mDevCon->ClearRenderTargetView(currentRenderTarget, (const float*)&clearColor);
	mDevCon->ClearDepthStencilView(currentDepthView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

ID3D11RenderTargetView* Graphics::GetBackBuffer()
{
	return mBackBuffer;
}

void Graphics::SetViewport(float x, float y, float width, float height)
{
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(vp));
	vp.TopLeftX = x;
	vp.TopLeftY = y;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	GetDeviceContext()->RSSetViewports((UINT)1, &vp);
}

ID3D11BlendState* Graphics::CreateBlendState(bool enable, D3D11_BLEND srcBlend, D3D11_BLEND dstBlend)
{
	D3D11_BLEND_DESC blender;
	ZeroMemory(&blender, sizeof(D3D11_BLEND_DESC));
	blender.AlphaToCoverageEnable = false;
	blender.IndependentBlendEnable = false;
	blender.RenderTarget[0].BlendEnable = enable;
	blender.RenderTarget[0].SrcBlend = srcBlend;
	blender.RenderTarget[0].DestBlend = dstBlend;
	blender.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blender.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blender.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blender.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blender.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	ID3D11BlendState* state;
	mDev->CreateBlendState(&blender, &state);
	return state;
}

ID3D11DepthStencilView* Graphics::GetDepthBuffer()
{
	return mDepthStencilView;
}

void Graphics::EndFrame()
{
	mSwapChain->Present(1, 0);
}

void Graphics::CleanD3D()
{
	mSwapChain->Release();
	mDev->Release();
	mDevCon->Release();
	mBackBuffer->Release();
	mDepthStencilTexture->Release(); 
	mDepthStencilView->Release();
	mDepthState->Release();
	mDefaultSampler->Release();
}
