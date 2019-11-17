#pragma once
#include "engineMath.h"
class Camera
{
public:
	Camera(class Graphics* pGraphics);
	virtual ~Camera();
	void SetActive();

	struct PerCameraConstants
	{
		Matrix4 viewProjData;
		Vector3 cameraPosData;
		float padding;
	};
	struct PerCameraConstants mCameraData;
	Matrix4 mView;
	Matrix4 mProj;
	Graphics* mGraphics;
	ID3D11Buffer* mCameraBuffer;
};

