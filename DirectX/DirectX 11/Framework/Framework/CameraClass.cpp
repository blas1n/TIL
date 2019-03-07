#include "CameraClass.h"

void CameraClass::SetPosition(float x, float y, float z) {
	m_posX = x;
	m_posY = y;
	m_posZ = z;
}
void CameraClass::SetRotation(float x, float y, float z) {
	m_rotX = x;
	m_rotY = y;
	m_rotZ = z;
}

DirectX::XMVECTOR CameraClass::GetPosition() {
	return DirectX::XMVectorSet(m_posX, m_posY, m_posZ, 0);
}

DirectX::XMVECTOR CameraClass::GetRotation() {
	return DirectX::XMVectorSet(m_rotX, m_rotY, m_rotZ, 0);
}

void CameraClass::Render() {
	const auto degToRad(0.0174532925f);

	auto up(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	auto position(DirectX::XMVectorSet(m_posX, m_posY, m_posZ, 0.0f));
	auto lookAt(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
	
	auto yaw(m_rotX * degToRad);
	auto pitch(m_rotY * degToRad);
	auto roll(m_rotZ * degToRad);

	DirectX::XMMATRIX rotationMatrix =
		DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	
	lookAt = DirectX::XMVector3TransformCoord(lookAt, rotationMatrix);
	up = DirectX::XMVector3TransformCoord(up, rotationMatrix);

	lookAt = DirectX::XMVectorAdd(position, lookAt);

	m_viewMatrix = DirectX::XMMatrixLookAtLH(position, lookAt, up);
}

DirectX::XMMATRIX CameraClass::GetViewMatrix() {
	return m_viewMatrix;
}