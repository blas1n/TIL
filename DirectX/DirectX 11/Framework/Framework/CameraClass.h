#pragma once

#ifndef __CAMERACLASS_H__
#define __CAMERACLASS_H__

#include <DirectXMath.h>
#include "AlignedAllocator.h"

class CameraClass : public AlignedAllocator<16> {
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

#endif