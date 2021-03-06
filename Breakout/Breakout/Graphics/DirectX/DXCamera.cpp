#include "DXCamera.h"

DXCamera::DXCamera(void)
{
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;

	m_fovy = (float)59 * PI * 1.0f / 180.0f;
	m_aspectRatio = 16.0f / 9.0f;
	m_aspectRatio = 1000.0f / 800.0f;
	m_nearZ = 1.0f;
	m_farZ = 420.0f;

	m_position = Vector3(0, 0, 0);
	m_forward = Vector3(0, 0, 1);
	m_right = Vector3(1, 0, 0);
	m_up = Vector3(0, 1, 0);

	UpdateView();
	UpdateProjection();
}

DXCamera::DXCamera(float _fovy, float _aspectRatio, float _nearZ, float _farZ)
{
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;


	m_fovy = 2 * atan(tan(0.5f * _fovy) * _aspectRatio);
	m_aspectRatio = _aspectRatio;
	m_nearZ = _nearZ;
	m_farZ = _farZ;

	m_position = Vector3(0, 0, 0);
	m_forward = Vector3(0, 0, 1);
	m_right = Vector3(1, 0, 0);
	m_up = Vector3(0, 1, 0);

	UpdateView();
	UpdateProjection();
}

DXCamera::~DXCamera(void)
{

}


void DXCamera::UpdateView()
{
	DirectX::XMFLOAT3 up, right, forward, position;
	
	memcpy(&up, &m_up, sizeof(DirectX::XMFLOAT3));
	memcpy(&right, &m_right, sizeof(DirectX::XMFLOAT3));
	memcpy(&forward, &m_forward, sizeof(DirectX::XMFLOAT3));
	memcpy(&position, &m_position, sizeof(DirectX::XMFLOAT3));

	DirectX::XMVECTOR R = DirectX::XMLoadFloat3(&right);
	DirectX::XMVECTOR U = DirectX::XMLoadFloat3(&up);
	DirectX::XMVECTOR L = DirectX::XMLoadFloat3(&forward);
	DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);

	// Keep camera's axes orthogonal to each other and of unit length.
	L = DirectX::XMVector3Normalize(L);
	U = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(L, R));

	// U, L already ortho-normal, so no need to normalize cross product.
	R = DirectX::XMVector3Cross(U, L);

	// Fill in the view matrix entries.
	float x = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, R));
	float y = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, U));
	float z = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, L));

	DirectX::XMStoreFloat3(&right, R);
	DirectX::XMStoreFloat3(&up, U);
	DirectX::XMStoreFloat3(&forward, L);

	m_view(0, 0) = right.x;
	m_view(1, 0) = right.y;
	m_view(2, 0) = right.z;
	m_view(3, 0) = x;

	m_view(0, 1) = up.x;
	m_view(1, 1) = up.y;
	m_view(2, 1) = up.z;
	m_view(3, 1) = y;

	m_view(0, 2) = forward.x;
	m_view(1, 2) = forward.y;
	m_view(2, 2) = forward.z;
	m_view(3, 2) = z;

	m_view(0, 3) = 0.0f;
	m_view(1, 3) = 0.0f;
	m_view(2, 3) = 0.0f;
	m_view(3, 3) = 1.0f;
}

void DXCamera::UpdateProjection()
{
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(m_fovy, m_aspectRatio, m_nearZ, m_farZ);
	DirectX::XMFLOAT4X4 proj4x4;
	DirectX::XMStoreFloat4x4(&proj4x4, proj);

	memcpy(&m_projection, &proj4x4, sizeof(Float4x4));
}

void DXCamera::SetForward(Vector3 _forward)
{

	DirectX::XMFLOAT3 up, right, forward2, position;

	memcpy(&up, &m_up, sizeof(DirectX::XMFLOAT3));
	memcpy(&right, &m_right, sizeof(DirectX::XMFLOAT3));
	memcpy(&forward2, &_forward, sizeof(DirectX::XMFLOAT3));
	memcpy(&position, &m_position, sizeof(DirectX::XMFLOAT3));

	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&position);
	DirectX::XMVECTOR direction = DirectX::XMLoadFloat3(&forward2);
	direction = DirectX::XMVector3Normalize(direction);
	DirectX::XMVECTOR up2 = DirectX::XMVectorSet(0, 1, 0, 0);

	if (DirectX::XMVector3Equal(direction, DirectX::XMVectorZero()))
		direction = DirectX::XMVectorSet(0, 0, 1, 0);

	else if (DirectX::XMVector3Equal(direction, DirectX::XMVectorSet(0, -1, 0, 0)))
		up2 = DirectX::XMVectorSet(0, 0, 1, 0);

	else if (DirectX::XMVector3Equal(direction, DirectX::XMVectorSet(0, 1, 0, 0)))
		up2 = DirectX::XMVectorSet(0, 0, -1, 0);

	DirectX::XMMATRIX view = DirectX::XMMatrixLookToLH(pos, direction, up2);

	DirectX::XMFLOAT4X4 view4x4;
	DirectX::XMStoreFloat4x4(&view4x4, view);
	memcpy(&m_view, &view4x4, sizeof(Float4x4));

	m_right = Vector3(m_view._11, m_view._21, m_view._31);
	m_up = Vector3(m_view._12, m_view._22, m_view._32);
	m_forward = Vector3(m_view._13, m_view._23, m_view._33);

	UpdateView();
}

void DXCamera::SetLookAt(Vector3 _target)
{
	Vector3 forward;
	forward.x = _target.x - m_position.x;
	forward.y = _target.y - m_position.y;
	forward.z = _target.z - m_position.z;

	SetForward(forward);
}