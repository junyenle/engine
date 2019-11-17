#include "stdafx.h"
#include "Components/CollisionComponent.h"
#include "Game.h"
#include "Camera.h"
#include "engineMath.h"
#include "Graphics.h"
#include "VertexBuffer.h"
#include "RenderObj.h"
#include "SkinnedObj.h"
#include "RenderCube.h"
#include "stringUtil.h"
#include "rapidjson/include/rapidjson/rapidjson.h"
#include "rapidjson/include/rapidjson/document.h"
#include "jsonUtil.h"
#include "Components/PointLight.h"
#include <fstream>
#include <sstream>
#include "Profiler.h"
#include "Skeleton.h"
#include "JobManager.h"
#include "Components/Character.h"
#include "Components/followCam.h"
#include "Components/player.h"
#include "Components/SimpleRotate.h"
#include "Physics.h"

float EXPERIMENTAL_TRIANGLE_ROTATION = 0;

JobManager* Game::jobManager;

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(HWND hWnd, float width, float height)
{
	mPhysics = new Physics();

	jobManager = new JobManager(4);
	jobManager->Begin();
	//Job* job = new Job();
	//jobManager->AddJob(job);
	mGraphics->InitD3D(hWnd, width, height);
	mCamera = new Camera(mGraphics);

	// shaders
	// simple
	Shader* simpleShader = new Shader(GetGraphics());
	D3D11_INPUT_ELEMENT_DESC inputElemS[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(BufferData::VertexPosNormalColorTex, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(BufferData::VertexPosNormalColorTex, color), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	simpleShader->Load(L"Shaders/Mesh.hlsl", inputElemS, 2);
	mShaderCache.Cache(L"Mesh", simpleShader);

	// basic
	Shader* basicMeshShader = new Shader(GetGraphics());
	D3D11_INPUT_ELEMENT_DESC inputElemBM[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(BufferData::VertexPosNormalColorTex, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(BufferData::VertexPosNormalColorTex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(BufferData::VertexPosNormalColorTex, color), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(BufferData::VertexPosNormalColorTex, uv), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	basicMeshShader->Load(L"Shaders/BasicMesh.hlsl", inputElemBM, 4);
	mShaderCache.Cache(L"BasicMesh", basicMeshShader);

	// phong
	Shader* phongShader = new Shader(GetGraphics());
	D3D11_INPUT_ELEMENT_DESC inputElemPHONG[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(BufferData::VertexPosNormalTex, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(BufferData::VertexPosNormalTex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(BufferData::VertexPosNormalTex, uv), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	phongShader->Load(L"Shaders/Phong.hlsl", inputElemPHONG, 3);
	mShaderCache.Cache(L"Phong", phongShader);

	// unlit
	Shader* unlitShader = new Shader(GetGraphics());
	D3D11_INPUT_ELEMENT_DESC inputElemUNLIT[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(BufferData::VertexPosNormalTex, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(BufferData::VertexPosNormalTex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(BufferData::VertexPosNormalTex, uv), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	unlitShader->Load(L"Shaders/Unlit.hlsl", inputElemUNLIT, 3);
	mShaderCache.Cache(L"Unlit", unlitShader);

	// skinned
	Shader* skinnedShader = new Shader(GetGraphics());
	D3D11_INPUT_ELEMENT_DESC inputElemSKINNED[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(BufferData::VertexPosNormBoneWeightTex, position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(BufferData::VertexPosNormBoneWeightTex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONES", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, offsetof(BufferData::VertexPosNormBoneWeightTex, boneIndices), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHTS", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, offsetof(BufferData::VertexPosNormBoneWeightTex, boneWeights), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(BufferData::VertexPosNormBoneWeightTex, text), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	skinnedShader->Load(L"Shaders/Skinned.hlsl", inputElemSKINNED, 5);
	mShaderCache.Cache(L"Skinned", skinnedShader);

	// normal
	Shader* normalShader = new Shader(GetGraphics());
	D3D11_INPUT_ELEMENT_DESC inputElemNORMAL[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(BufferData::VertexPosNormalTexTan, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(BufferData::VertexPosNormalTexTan, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(BufferData::VertexPosNormalTexTan, tangent), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(BufferData::VertexPosNormalTexTan, uv), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	normalShader->Load(L"Shaders/Normal.hlsl", inputElemNORMAL, 4);
	mShaderCache.Cache(L"Normal", normalShader);

	// copy
	Shader* copyShader = new Shader(GetGraphics());
	D3D11_INPUT_ELEMENT_DESC inputElemCOPY[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(BufferData::QuadVertexData, position), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(BufferData::QuadVertexData, uv), D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	copyShader->Load(L"Shaders/Copy.hlsl", inputElemCOPY, 2);
	mShaderCache.Cache(L"Copy", copyShader);

	// bloom mask
	Shader* bloomMaskShader = new Shader(GetGraphics());
	D3D11_INPUT_ELEMENT_DESC inputElemBloomMask[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(BufferData::QuadVertexData, position), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(BufferData::QuadVertexData, uv), D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	bloomMaskShader->Load(L"Shaders/BloomMask.hlsl", inputElemBloomMask, 2);
	mShaderCache.Cache(L"BloomMask", bloomMaskShader);

	// blur
	Shader* blurHoriShader = new Shader(GetGraphics());
	// just use bloom mask input
	blurHoriShader->Load(L"Shaders/BlurHori.hlsl", inputElemBloomMask, 2);
	mShaderCache.Cache(L"BlurHori", blurHoriShader);

	Shader* blurVertShader = new Shader(GetGraphics());
	// just use bloom mask input
	blurVertShader->Load(L"Shaders/BlurVert.hlsl", inputElemBloomMask, 2);
	mShaderCache.Cache(L"BlurVert", blurVertShader);

	// create lighting buffer
	mLightingBuffer = mGraphics->CreateGraphicsBuffer(&mLightingData, sizeof(BufferData::LightingData), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);

	LoadLevel(L"Assets/Levels/Level10.itplevel");

	offScreenTexture = new Texture(mGraphics);
	offScreenRenderTarget = offScreenTexture->CreateRenderTarget(mGraphics->GetScreenWidth(), mGraphics->GetScreenHeight(), DXGI_FORMAT_R32G32B32A32_FLOAT);
	halfOffScreenTexture = new Texture(mGraphics);
	halfOffScreenRenderTarget = halfOffScreenTexture->CreateRenderTarget(mGraphics->GetScreenWidth()/2, mGraphics->GetScreenHeight()/2, DXGI_FORMAT_R32G32B32A32_FLOAT);
	quarterOffScreenTexture = new Texture(mGraphics);
	quarterOffScreenRenderTarget = quarterOffScreenTexture->CreateRenderTarget(mGraphics->GetScreenWidth() / 4, mGraphics->GetScreenHeight() / 4, DXGI_FORMAT_R32G32B32A32_FLOAT);
	quarterOffScreenTexture2 = new Texture(mGraphics);
	quarterOffScreenRenderTarget2 = quarterOffScreenTexture2->CreateRenderTarget(mGraphics->GetScreenWidth()/4, mGraphics->GetScreenHeight()/4, DXGI_FORMAT_R32G32B32A32_FLOAT);

	BufferData::QuadVertexData screenQuad[] =
	{
		{ Vector3(-1.0f, -1.0f, 0.0f), Vector2(0.0f, 1.0f) },
		{ Vector3(1.0f, -1.0f, 0.0f), Vector2(1.0f, 1.0f) },
		{ Vector3(1.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f) },
		{ Vector3(-1.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f) }
	};
	uint16_t screenQuadIndices[] =
	{
		0, 1, 2, 2, 3, 0
	};

	quadVertexBuffer = new VertexBuffer(mGraphics, screenQuad, 4, sizeof(BufferData::QuadVertexData), screenQuadIndices, 6, sizeof(uint16_t));

	opaqueBlendState = mGraphics->CreateBlendState(false, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ZERO);
	additiveBlendState = mGraphics->CreateBlendState(true, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE);

}

BufferData::PointLightData* Game::AllocateLight()
{
	for (unsigned i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		if (!mLightingData.c_pointLight[i].isEnabled)
		{
			mLightingData.c_pointLight[i].isEnabled = 1;
			return &(mLightingData.c_pointLight[i]);
		}
	}
	return NULL;
}

void Game::FreeLight(BufferData::PointLightData* pLight)
{
	pLight->isEnabled = 0;
}

void Game::SetAmbientLight(const Vector3& color)
{
	mLightingData.c_ambient = color;
}

Vector3 Game::GetAmbientLight() const
{
	return mLightingData.c_ambient;
}

void Game::Shutdown()
{
	mGraphics->CleanD3D();
	mLightingBuffer->Release();
	for (RenderObj* renderObj : mRenderObj)
	{
		delete renderObj;
	}
	// bloom stuff
	offScreenRenderTarget->Release();
	delete offScreenTexture;
	halfOffScreenRenderTarget->Release();
	delete halfOffScreenTexture;
	quarterOffScreenRenderTarget->Release();
	delete quarterOffScreenTexture;
	quarterOffScreenRenderTarget2->Release();
	delete quarterOffScreenTexture2;
	delete quadVertexBuffer;
	opaqueBlendState->Release();
	additiveBlendState->Release();

	delete mGraphics;
	delete mCamera;
	delete mPhysics;
	jobManager->End();
	delete jobManager;
}

void Game::Update(float deltaTime)
{
	for (RenderObj* renderObj : mRenderObj)
	{
		renderObj->Update(deltaTime);
	}
	jobManager->WaitForJobs();
}

void Game::RenderFrame()
{
	mGraphics->SetBlendState(opaqueBlendState);

	mGraphics->SetActiveTexture(0, nullptr);
	mGraphics->SetRenderTarget(offScreenRenderTarget, mGraphics->GetDepthBuffer());
	{
		PROFILE_SCOPE(RENDERFRAME_begin_frame);
		mGraphics->BeginFrame(Graphics::Color4(0.0f, 0.2f, 0.4f, 1.0f));
	}
	{
		PROFILE_SCOPE(RENDERFRAME_set_active_camera);
		mCamera->SetActive();
	}
	{
		PROFILE_SCOPE(RENDERFRAME_upload_lighting_buffer);
		mGraphics->UploadBuffer(mLightingBuffer, &mLightingData, sizeof(BufferData::LightingData));
	}
	{
		PROFILE_SCOPE(RENDERFRAME_set_ps_constant_buffers);
		mGraphics->GetDeviceContext()->PSSetConstantBuffers(Graphics::CONSTANT_BUFFER_LIGHTING, 1, &mLightingBuffer);
	}
	{
		PROFILE_SCOPE(RENDERFRAME_draw_objects);
		for (RenderObj* renderObj : mRenderObj)
		{
			renderObj->Draw();
		}
	}

	// orig
	//mGraphics->SetRenderTarget(mGraphics->GetBackBuffer(), 0);
	//mGraphics->SetViewport(0.0f, 0.0f, mGraphics->GetScreenWidth(), mGraphics->GetScreenHeight());
	//GetShader(L"Copy")->SetActive();
	//offScreenTexture->SetActive(0);
	//quadVertexBuffer->SetActive();
	//quadVertexBuffer->Draw();
	//mGraphics->EndFrame();
	//return;

	mGraphics->SetActiveTexture(0, nullptr);
	// game to half
	mGraphics->SetRenderTarget(halfOffScreenRenderTarget, nullptr);
	mGraphics->SetViewport(0.0f, 0.0f, mGraphics->GetScreenWidth() / 2.0f, mGraphics->GetScreenHeight() / 2.0f);
	GetShader(L"BloomMask")->SetActive();
	offScreenTexture->SetActive(0);
	quadVertexBuffer->SetActive();
	quadVertexBuffer->Draw();

	mGraphics->SetActiveTexture(0, nullptr);;
	// half to quarter
	mGraphics->SetRenderTarget(quarterOffScreenRenderTarget, nullptr);
	mGraphics->SetViewport(0.0f, 0.0f, mGraphics->GetScreenWidth() / 4.0f, mGraphics->GetScreenHeight() / 4.0f);
	GetShader(L"Copy")->SetActive();
	halfOffScreenTexture->SetActive(0);
	quadVertexBuffer->SetActive();
	quadVertexBuffer->Draw();

	mGraphics->SetActiveTexture(0, nullptr);
	// quarter to quarter2
	mGraphics->SetRenderTarget(quarterOffScreenRenderTarget2, nullptr);
	mGraphics->SetViewport(0.0f, 0.0f, mGraphics->GetScreenWidth() / 4.0f, mGraphics->GetScreenHeight() / 4.0f);
	GetShader(L"BlurHori")->SetActive();
	quarterOffScreenTexture->SetActive(0);
	quadVertexBuffer->SetActive();
	quadVertexBuffer->Draw();


	mGraphics->SetActiveTexture(0, nullptr);
	// quarter2 to quarter
	mGraphics->SetRenderTarget(quarterOffScreenRenderTarget, nullptr);
	mGraphics->SetViewport(0.0f, 0.0f, mGraphics->GetScreenWidth() / 4.0f, mGraphics->GetScreenHeight() / 4.0f);
	GetShader(L"BlurVert")->SetActive();
	quarterOffScreenTexture2->SetActive(0);
	quadVertexBuffer->SetActive();
	quadVertexBuffer->Draw();


	mGraphics->SetActiveTexture(0, nullptr);
	// quarter to back buffer
	mGraphics->SetRenderTarget(mGraphics->GetBackBuffer(), nullptr);
	mGraphics->SetViewport(0.0f, 0.0f, mGraphics->GetScreenWidth(), mGraphics->GetScreenHeight());
	GetShader(L"Copy")->SetActive();
	quarterOffScreenTexture->SetActive(0);
	quadVertexBuffer->SetActive();
	quadVertexBuffer->Draw();

	mGraphics->SetActiveTexture(0, nullptr);
	// blend
	mGraphics->SetBlendState(additiveBlendState);
	mGraphics->SetRenderTarget(mGraphics->GetBackBuffer(), nullptr);
	mGraphics->SetViewport(0.0f, 0.0f, mGraphics->GetScreenWidth(), mGraphics->GetScreenHeight());
	GetShader(L"Copy")->SetActive();
	offScreenTexture->SetActive(0);
	quadVertexBuffer->SetActive();
	quadVertexBuffer->Draw();


	{
		PROFILE_SCOPE(RENDERFRAME_end_frame_swapchain_present);
		mGraphics->EndFrame();
	}
}

void Game::OnKeyDown(uint32_t key)
{
	m_keyIsHeld[key] = true;
}

void Game::OnKeyUp(uint32_t key)
{
	m_keyIsHeld[key] = false;
}

bool Game::IsKeyHeld(uint32_t key) const
{
	const auto find = m_keyIsHeld.find(key);
	if (find != m_keyIsHeld.end())
		return find->second;
	return false;
}

bool Game::LoadLevel(const WCHAR* fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		return false;
	}

	std::string str = doc["metadata"]["type"].GetString();
	int ver = doc["metadata"]["version"].GetInt();

	// Check the metadata
	if (!doc["metadata"].IsObject() ||
		str != "itplevel" ||
		ver != 2)
	{
		return false;
	}

	// load camera
	Vector3 cameraPos;
	GetVectorFromJSON(doc["camera"], "position", cameraPos);
	Quaternion cameraRot;
	GetQuaternionFromJSON(doc["camera"], "rotation", cameraRot);
	Matrix4 translationInverse = Matrix4::CreateTranslation(-1 * cameraPos);
	Matrix4 rotationInverse = Matrix4::CreateFromQuaternion(cameraRot);
	rotationInverse.Transpose();
	mCamera->mView = translationInverse * rotationInverse;

	// load lightingdata
	Vector3 ambientLight;
	GetVectorFromJSON(doc["lightingData"], "ambient", ambientLight);
	SetAmbientLight(ambientLight);

	// render objects
	const rapidjson::Value& renderObjs = doc["renderObjects"];
	for (rapidjson::SizeType i = 0; i < renderObjs.Size(); i++)
	{
		// for each object renderObjs[i]
		std::wstring meshName;
		GetWStringFromJSON(renderObjs[i], "mesh", meshName);
		Mesh* mesh = LoadMesh(meshName);

		RenderObj* renderObj;
		if (mesh->IsSkinned())
		{
			renderObj = new SkinnedObj(this, mesh);
		}
		else
		{
			renderObj = new RenderObj(this, mesh);
		}
		float scale;
		GetFloatFromJSON(renderObjs[i], "scale", scale);
		Vector3 translation;
		GetVectorFromJSON(renderObjs[i], "position", translation);
		Quaternion rotation;
		GetQuaternionFromJSON(renderObjs[i], "rotation", rotation);
		renderObj->mObjectData.data = Matrix4::CreateScale(scale) * Matrix4::CreateFromQuaternion(rotation) * Matrix4::CreateTranslation(translation);
		// load components, if they exist
		const rapidjson::Value& components = renderObjs[i]["components"];
		// would put a null check here, but it seems the level format just uses empty arrays
		for (rapidjson::SizeType i = 0; i < components.Size(); i++)
		{
			std::string componentType;
			GetStringFromJSON(components[i], "type", componentType);
			if (componentType == "PointLight")
			{
				// create point light
				PointLight* pointLight = new PointLight(renderObj);
				pointLight->LoadProperties(components[i]);
				renderObj->AddComponent(pointLight);
			}
			else if (componentType == "Character")
			{
				Character* character = new Character((SkinnedObj*)renderObj);
				character->LoadProperties(components[i]);
				renderObj->AddComponent(character);
			}
			else if (componentType == "FollowCam")
			{
				mFollowCam = new FollowCam(renderObj);
				mFollowCam->LoadProperties(components[i]);
				renderObj->AddComponent(mFollowCam);
			}
			else if (componentType == "Player")
			{
				mPlayer = new Player((SkinnedObj*)renderObj, this);
				mPlayer->LoadProperties(components[i]);
				renderObj->AddComponent(mPlayer);
			}
			else if (componentType == "Collision")
			{
				CollisionComponent* component = new CollisionComponent(renderObj);
				component->LoadProperties(components[i]);
				renderObj->AddComponent(component);
			}
			else if (componentType == "SimpleRotate")
			{
				SimpleRotate* rotate = new SimpleRotate(renderObj);
				rotate->LoadProperties(components[i]);
				renderObj->AddComponent(rotate);
			}
		}
		mRenderObj.push_back(renderObj);
	}

	return true;
}

Shader* Game::GetShader(const std::wstring& shaderName)
{
	return mShaderCache.Get(shaderName);
}

Texture* Game::LoadTexture(const std::wstring& textureName)
{
	return mTextureCache.Load(textureName);
}


Mesh* Game::LoadMesh(const std::wstring& fileName)
{
	return mMeshCache.Load(fileName);
}

Skeleton* Game::LoadSkeleton(const std::wstring& fileName)
{
	return mSkeletonCache.Load(fileName);
}

Animation* Game::LoadAnimation(const std::wstring& fileName)
{
	return mAnimationCache.Load(fileName);
}