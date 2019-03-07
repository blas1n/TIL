#pragma once

#include "../Utility/AlignedAllocator.h"
#include "../Utility//NonMovable.h"
#include <DirectXMath.h>

class CameraClass : public AlignedAllocator<16>, public NonMovable {
public:
	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	DirectX::XMVECTOR GetPosition();
	DirectX::XMVECTOR GetRotation();

	void Render();
	DirectX::XMMATRIX GetViewMatrix();

private:
	float m_posX, m_posY, m_posZ;
	float m_rotX, m_rotY, m_rotZ;
	DirectX::XMMATRIX m_viewMatrix;
};