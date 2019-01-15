#include <iostream>
#include <Windows.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

ostream& XM_CALLCONV operator<< (ostream& os, FXMVECTOR v) {
	XMFLOAT4 dest;
	XMStoreFloat4(&dest, v);

	os << "(" << dest.x << ", " << dest.y << ", " << dest.z << ", " << dest.w << ")";
	return os;
}

ostream& XM_CALLCONV operator<< (ostream& os, FXMMATRIX m) {
	for (int i = 0; i < 4; i++)
		os << m.r[i] << endl;

	return os;
}

int main() {
	if (!XMVerifyCPUSupport()) {
		cout << "DirectXMath ¹ÌÁö¿ø" << endl;
		return 0;
	}

	XMMATRIX a(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f, 0.f, 0.0f,
		0.0f, 0.0f, 4.0f, 0.0f,
		1.0f, 2.0f, 3.0f, 1.0f);

	XMMATRIX b = XMMatrixIdentity();
	XMMATRIX c = a * b; // c == a
	XMMATRIX d = XMMatrixTranspose(a);

	XMVECTOR det = XMMatrixDeterminant(a);
	XMMATRIX e = XMMatrixInverse(&det, a);

	XMMATRIX f = a * e;

	cout << "a =\n" << a << endl;
	cout << "b =\n" << b << endl;
	cout << "c =\n" << c << endl;
	cout << "d =\n" << d << endl;
	cout << "det(a) = " << det << endl;
	cout << "e =\n" << e << endl;
	cout << "f =\n" << f << endl;

	return 0;
}