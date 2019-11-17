#include "stdafx.h"
#include "VertexBuffer.h"
#include "Graphics.h"


VertexBuffer::VertexBuffer(Graphics* graphics, const void* vertData, uint32_t vertCount, uint32_t vertStride, const void* indexData, uint32_t indexCount, uint32_t indexStride) :
	mGraphics(graphics), 
	mIndexCount(indexCount), 
	mIndexStride(indexStride),
	mVertStride(vertStride)
{
	mIndexFormat = DXGI_FORMAT_R16_UINT;
	mVertBuffer = mGraphics->CreateGraphicsBuffer(vertData, vertCount * vertStride, D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC); // no instructions as to what to set flags to	
	mIndexBuffer = mGraphics->CreateGraphicsBuffer(indexData, indexCount * indexStride, D3D11_BIND_INDEX_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC); // no instructions as to what to set flags to	
}


VertexBuffer::~VertexBuffer()
{
	mVertBuffer->Release();
	mIndexBuffer->Release();
}


void VertexBuffer::SetActive() const
{
	unsigned zero = 0;
	mGraphics->GetDeviceContext()->IASetVertexBuffers(0, 1, &mVertBuffer, &mVertStride, &zero);
	mGraphics->GetDeviceContext()->IASetIndexBuffer(mIndexBuffer, mIndexFormat, 0);
}

void VertexBuffer::Draw() const
{
	//mGraphics->GetDeviceContext()->Draw(mIndexCount, 0);
	mGraphics->GetDeviceContext()->DrawIndexed(mIndexCount, 0, 0);
}