#pragma once
#include "Graphics.h"
#include "engineMath.h"
#include "BufferDataStructures.h"
#include "assetCache.h"
#include "Shader.h"
#include "texture.h"
#include "mesh.h"
#include "Skeleton.h"
#include "Animation.h"

class Game
{
public:

    Game();
    ~Game();

    void Init(HWND hWnd, float width, float height);
    void Shutdown();
	void Update(float deltaTime);
    void RenderFrame();

	void OnKeyDown(uint32_t key);
	void OnKeyUp(uint32_t key);
	bool IsKeyHeld(uint32_t key) const;

	Graphics* GetGraphics() { return mGraphics; };
	BufferData::PointLightData* AllocateLight();
	void FreeLight(BufferData::PointLightData* pLight);
	void SetAmbientLight(const Vector3& color);
	Vector3 GetAmbientLight() const;
	AssetCache<Shader> mShaderCache = AssetCache<Shader>(this);
	AssetCache<Texture> mTextureCache = AssetCache<Texture>(this);
	AssetCache<Mesh> mMeshCache = AssetCache<Mesh>(this);
	AssetCache<Skeleton> mSkeletonCache = AssetCache<Skeleton>(this);
	AssetCache<Animation> mAnimationCache = AssetCache<Animation>(this);
	Shader* GetShader(const std::wstring& shaderName);
	Texture* LoadTexture(const std::wstring& textureName);
	Mesh* LoadMesh(const std::wstring& fileName);
	Skeleton* LoadSkeleton(const std::wstring& fileName);
	Animation* LoadAnimation(const std::wstring& fileName);

	BufferData::LightingData mLightingData;
	ID3D11Buffer* mLightingBuffer;
	std::unordered_map<uint32_t, bool> m_keyIsHeld;
	Graphics* mGraphics = new Graphics();
	std::vector<class RenderObj*> mRenderObj = std::vector<RenderObj*>();
	class Camera* mCamera;
	bool LoadLevel(const WCHAR* fileName);
	float s_angle = 0;
	static class JobManager* jobManager;
	class Player* mPlayer;
	class FollowCam* mFollowCam;
	class Physics* mPhysics;

	Texture* offScreenTexture;
	ID3D11RenderTargetView* offScreenRenderTarget;
	Texture* halfOffScreenTexture;
	ID3D11RenderTargetView* halfOffScreenRenderTarget;
	Texture* quarterOffScreenTexture;
	ID3D11RenderTargetView* quarterOffScreenRenderTarget;
	Texture* quarterOffScreenTexture2;
	ID3D11RenderTargetView* quarterOffScreenRenderTarget2;
	VertexBuffer* quadVertexBuffer;
	ID3D11BlendState* opaqueBlendState;
	ID3D11BlendState* additiveBlendState;
};