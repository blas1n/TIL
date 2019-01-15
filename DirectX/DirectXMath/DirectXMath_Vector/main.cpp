#include <iostream>
#include <Windows.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

ostream& XM_CALLCONV operator<<(ostream& os, FXMVECTOR v) {
	XMFLOAT3 dest;
	XMStoreFloat3(&dest, v);

	os << "(" << dest.x << ", " << dest.y << ", " << dest.z << ")";
	return os;
}

bool Equals(float lhs, float rhs) {
	const float epsilon = 0.001f;

	return fabs(lhs - rhs) < epsilon ? true : false;
}

int main() {
	// bool을 1(0)이 아닌 true(false)로 출력
	cout.setf(ios_base::boolalpha);

	// DirectXMath 지원 (SSE2 지원) 여부 확인
	// Windows.h 필요
	if (!XMVerifyCPUSupport()) {
		cout << "DirectXMath 미지원" << endl;
		return 0;
	}

	XMVECTOR p = XMVectorZero();
	XMVECTOR q = XMVectorSplatOne();
	XMVECTOR r = XMVectorSet(1.0f, 2.0f, 3.0f, 0.0f);
	XMVECTOR s = XMVectorReplicate(-2.0f);
	XMVECTOR t = XMVectorSplatZ(r);

	cout << "p = " << p << endl;
	cout << "q = " << q << endl;
	cout << "r = " << r << endl;
	cout << "s = " << s << endl;
	cout << "t = " << t << endl << endl;

	XMVECTOR n = XMVectorZero();
	n = XMVectorSetX(n, 1.0f);

	XMVECTOR u = XMVectorSet(1.0f, 2.0f, 3.0f, 0.0f);
	XMVECTOR v = XMVectorSet(-2.0f, 1.0f, -3.0f, 0.0f);
	XMVECTOR w = XMVectorSet(0.707f, 0.707f, 0.0f, 0.0f);

	XMVECTOR a = u + v;
	XMVECTOR b = u - v;
	XMVECTOR c = 10.0f * u;

	XMVECTOR length = XMVector3Length(u);
	//XMVECTOR length = XMVector3LengthEst(u);
	float lengthResult = XMVectorGetX(length);

	XMVECTOR normalize = XMVector3Normalize(u);
	//XMVECTOR normalize = XMVector3NormalizeEst(u);

	XMVECTOR dot = XMVector3Dot(u, v);
	float dotResult = XMVectorGetX(dot);

	XMVECTOR cross = XMVector3Cross(u, v);

	XMVECTOR projW, perpW;
	XMVector3ComponentsFromNormal(&projW, &perpW, w, n);

	bool equal = XMVector3Equal(projW + perpW, w);
	//bool equal = XMVector3NearEqual(projW + perpW, w);
	bool notEqual = XMVector3NotEqual(projW + perpW, w);

	XMVECTOR angleVec = XMVector3AngleBetweenVectors(projW, perpW);
	float angleRadians = XMVectorGetX(angleVec);
	float angleDegrees = XMConvertToDegrees(angleRadians);

	cout << "u = " << u << endl;
	cout << "v = " << v << endl;
	cout << "w = " << w << endl;
	cout << "n = " << n << endl << endl;

	cout << "u + v = " << a << endl;
	cout << "u - v = " << b << endl;
	cout << "10 * u = " << c << endl << endl;

	cout << "u.normalize = " << normalize << endl;
	cout << "u.length = " << lengthResult << endl << endl;

	cout << "u dot v = " << dotResult << endl;
	cout << "u cross v = " << cross << endl << endl;

	cout << "proj (w) = " << projW << endl;
	cout << "perp (w) = " << perpW << endl << endl;

	cout << "proj (w) + perp (w) == w = " << equal << endl;
	cout << "proj (w) + perp (w) != w = " << notEqual << endl;
	cout << "angle = " << angleDegrees << endl << endl;

	return 0;
}