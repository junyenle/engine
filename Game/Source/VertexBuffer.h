#pragma once
class VertexBuffer
{
public:
	VertexBuffer(class Graphics* graphics, const void* vertData, uint32_t vertCount, uint32_t vertStride, const void* indexData, uint32_t indexCount, uint32_t indexStride);
	~VertexBuffer();
	void SetActive() const;
	void Draw() const;
private:
	struct ID3D11Buffer* mVertBuffer;
	uint32_t mVertStride;
	uint32_t mIndexStride;
	uint32_t mIndexCount;
	Graphics* mGraphics;
	ID3D11Buffer* mIndexBuffer;
	DXGI_FORMAT mIndexFormat;
};

