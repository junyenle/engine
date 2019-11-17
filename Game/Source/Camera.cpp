#include "stdafx.h"
#include "Camera.h"
#include "Graphics.h"

Camera::Camera(Graphics* pGraphics) :
	mGraphics(pGraphics)
{
	mView = Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, -100.0f));
	mProj = Matrix4::CreateRotationY(-Math::PiOver2) * Matrix4::CreateRotationZ(-Math::PiOver2) * Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f), mGraphics->GetScreenWidth(), mGraphics->GetScreenHeight(), 25.0f, 10000.0f);
	//mProj = Matrix4::CreateOrtho(mGraphics->GetScreenWidth(), mGraphics->GetScreenHeight(), 1000.0f, -1000.0f);
	mCameraBuffer = mGraphics->CreateGraphicsBuffer(&mCameraData, sizeof(mCameraData), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
}


Camera::~Camera()
{
	mCameraBuffer->Release();
}

void Camera::SetActive()
{
	mCameraData.viewProjData = mView * mProj;

	Matrix4 inverseView = mView;
	inverseView.Invert();
	mCameraData.cameraPosData = Vector3(inverseView.mat[3][0], inverseView.mat[3][1], inverseView.mat[3][2]);

	mGraphics->UploadBuffer(mCameraBuffer, &(mCameraData), sizeof(mCameraData));
	mGraphics->GetDeviceContext()->VSSetConstantBuffers(Graphics::CONSTANT_BUFFER_CAMERA, 1, &mCameraBuffer);
	mGraphics->GetDeviceContext()->PSSetConstantBuffers(Graphics::CONSTANT_BUFFER_CAMERA, 1, &mCameraBuffer);
}
