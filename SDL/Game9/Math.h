#pragma once

#include <cmath>
#include <memory.h>
#include <limits>

namespace Math {
	constexpr float Pi = 3.1415926535f;
	constexpr float Inf = std::numeric_limits<float>::infinity();

	inline float ToRadians(const float degrees) {
		return degrees * Pi / 180.0f;
	}

	inline float ToDegrees(const float radians) {
		return radians * 180.0f / Pi;
	}

	inline bool NearZero(const float val, const float epsilon = 0.001f) {
		return (fabs(val) <= epsilon);
	}

	template <typename T>
	T Max(const T& a, const T& b) {
		return (a < b ? b : a);
	}

	template <typename T>
	T Min(const T& a, const T& b) {
		return (a < b ? a : b);
	}

	template <typename T>
	T Clamp(const T& value, const T& lower, const T& upper) {
		return Min(upper, Max(lower, value));
	}

	inline int Abs(const int value) {
		return value > 0 ? value : -value;
	}

	inline float Abs(const float value) {
		return fabs(value);
	}

	inline float Cos(const float angle) {
		return cosf(angle);
	}

	inline float Sin(const float angle) {
		return sinf(angle);
	}

	inline float Tan(const float angle) {
		return tanf(angle);
	}

	inline float Acos(const float value) {
		return acosf(value);
	}

	inline float ASin(const float value) {
		return asinf(value);
	}

	inline float ATan(const float value) {
		return atanf(value);
	}

	inline float Atan2(const float y, const float x) {
		return atan2f(y, x);
	}

	inline float Sec(const float angle) {
		return 1.0f / Cos(angle);
	}

	inline float Cosec(const float angle) {
		return 1.0f / Sin(angle);
	}

	inline float Cot(const float angle) {
		return 1.0f / Tan(angle);
	}

	inline float Lerp(const float a, const float b, const float f) {
		return a + f * (b - a);
	}

	inline float Sqrt(const float value) {
		return sqrtf(value);
	}

	inline float Fmod(const float numer, const float denom) {
		return fmod(numer, denom);
	}
}

class Vector2 {
public:
	float x;
	float y;

	Vector2()
		:x(0.0f), y(0.0f) {}

	explicit Vector2(const float inX, const float inY)
		:x(inX), y(inY) {}

	void Set(const float inX, const float inY) {
		x = inX;
		y = inY;
	}

	Vector2 operator-() const {
		return Vector2(-x, -y);
	}

	friend Vector2 operator+(const Vector2& lhs, const Vector2& rhs) {
		return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
	}

	friend Vector2 operator-(const Vector2& lhs, const Vector2& rhs) {
		return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
	}

	friend Vector2 operator*(const Vector2& lhs, const Vector2& rhs) {
		return Vector2(lhs.x * rhs.x, lhs.y * rhs.y);
	}

	friend Vector2 operator*(const Vector2& vec, const float scalar) {
		return Vector2(vec.x * scalar, vec.y * scalar);
	}

	friend Vector2 operator*(const float scalar, const Vector2& vec) {
		return Vector2(vec.x * scalar, vec.y * scalar);
	}

	friend Vector2 operator/(const Vector2& vec, const float scalar) {
		return Vector2(vec.x / scalar, vec.y / scalar);
	}

	Vector2& operator+=(const Vector2& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	Vector2& operator-=(const Vector2& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Vector2 operator*=(const Vector2& other) {
		x *= other.x;
		y *= other.y;
		return *this;
	}

	Vector2& operator*=(const float scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}

	Vector2& operator/=(const float scalar) {
		x /= scalar;
		y /= scalar;
		return *this;
	}

	float LengthSquared() const {
		return x * x + y * y;
	}

	float Length() const {
		return (Math::Sqrt(LengthSquared()));
	}

	void Normalized() {
		const auto length = Length();
		x /= length;
		y /= length;
	}

	static Vector2 Normalize(const Vector2& vec) {
		Vector2 temp = vec;
		temp.Normalized();
		return temp;
	}

	static float Dot(const Vector2& lhs, const Vector2& rhs) {
		return (lhs.x * rhs.x + lhs.y * rhs.y);
	}

	static Vector2 Lerp(const Vector2& a, const Vector2& b, const float f) {
		return Vector2(a + f * (b - a));
	}

	static Vector2 Reflect(const Vector2& v, const Vector2& n) {
		return v - 2.0f * Vector2::Dot(v, n) * n;
	}

	static Vector2 Transform(const Vector2& vec, const class Matrix3& mat, float w = 1.0f);

	static const Vector2 Zero;
	static const Vector2 One;
	static const Vector2 UnitX;
	static const Vector2 UnitY;
};

class Vector3 {
public:
	float x;
	float y;
	float z;

	Vector3()
		:x(0.0f), y(0.0f), z(0.0f) {}

	explicit Vector3(const float inX, const float inY, const float inZ)
		:x(inX), y(inY), z(inZ) {}

	void Set(const float inX, const float inY, const float inZ) {
		x = inX;
		y = inY;
		z = inZ;
	}

	Vector3 operator-() const {
		return Vector3(-x, -y, -z);
	}

	friend Vector3 operator+(const Vector3& lhs, const Vector3& rhs) {
		return Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}

	friend Vector3 operator-(const Vector3& lhs, const Vector3& rhs) {
		return Vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}

	friend Vector3 operator*(const Vector3& lhs, const Vector3& rhs) {
		return Vector3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
	}

	friend Vector3 operator*(const Vector3& vec, const float scalar) {
		return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
	}

	friend Vector3 operator*(const float scalar, const Vector3& vec) {
		return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
	}

	friend Vector3 operator/(const Vector3& vec, const float scalar) {
		return Vector3(vec.x / scalar, vec.y / scalar, vec.z / scalar);
	}

	Vector3& operator+=(const Vector3& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vector3& operator-=(const Vector3& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	Vector3& operator*=(const Vector3& other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	Vector3& operator*=(const float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	Vector3& operator/=(const float scalar) {
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	float LengthSquared() const {
		return (x * x + y * y + z * z);
	}

	float Length() const {
		return (Math::Sqrt(LengthSquared()));
	}

	void Normalized() {
		const auto length = Length();
		x /= length;
		y /= length;
		z /= length;
	}

	static Vector3 Normalize(const Vector3& vec) {
		auto temp = vec;
		temp.Normalized();
		return temp;
	}

	static float Dot(const Vector3& lhs, const Vector3& rhs) {
		return (lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z);
	}

	static Vector3 Cross(const Vector3& lhs, const Vector3& rhs) {
		Vector3 ret;
		ret.x = lhs.y * rhs.z - lhs.z * rhs.y;
		ret.y = lhs.z * rhs.x - lhs.x * rhs.z;
		ret.z = lhs.x * rhs.y - lhs.y * rhs.x;
		return ret;
	}

	static Vector3 Lerp(const Vector3& a, const Vector3& b, const float f) {
		return Vector3(a + f * (b - a));
	}

	static Vector3 Reflect(const Vector3& v, const Vector3& n) {
		return v - 2.0f * Vector3::Dot(v, n) * n;
	}

	static Vector3 Transform(const Vector3& vec, const class Matrix4& mat, float w = 1.0f);

	static Vector3 TransformWithPerspDiv(const Vector3& vec, const class Matrix4& mat, float w = 1.0f);

	static Vector3 Transform(const Vector3& v, const class Quaternion& q);

	static const Vector3 Zero;
	static const Vector3 One;
	static const Vector3 UnitX;
	static const Vector3 UnitY;
	static const Vector3 UnitZ;
	static const Vector3 Infinity;
};

class Matrix3 {
public:
	float mat[3][3];

	Matrix3() {
		*this = Matrix3::Identity;
	}

	explicit Matrix3(const float inMat[3][3]) {
		memcpy(mat, inMat, 9 * sizeof(float));
	}

	friend Matrix3 operator*(const Matrix3& lhs, const Matrix3& rhs) {
		Matrix3 ret;

		ret.mat[0][0] =
			lhs.mat[0][0] * rhs.mat[0][0] +
			lhs.mat[0][1] * rhs.mat[1][0] +
			lhs.mat[0][2] * rhs.mat[2][0];

		ret.mat[0][1] =
			lhs.mat[0][0] * rhs.mat[0][1] +
			lhs.mat[0][1] * rhs.mat[1][1] +
			lhs.mat[0][2] * rhs.mat[2][1];

		ret.mat[0][2] =
			lhs.mat[0][0] * rhs.mat[0][2] +
			lhs.mat[0][1] * rhs.mat[1][2] +
			lhs.mat[0][2] * rhs.mat[2][2];

		ret.mat[1][0] =
			lhs.mat[1][0] * rhs.mat[0][0] +
			lhs.mat[1][1] * rhs.mat[1][0] +
			lhs.mat[1][2] * rhs.mat[2][0];

		ret.mat[1][1] =
			lhs.mat[1][0] * rhs.mat[0][1] +
			lhs.mat[1][1] * rhs.mat[1][1] +
			lhs.mat[1][2] * rhs.mat[2][1];

		ret.mat[1][2] =
			lhs.mat[1][0] * rhs.mat[0][2] +
			lhs.mat[1][1] * rhs.mat[1][2] +
			lhs.mat[1][2] * rhs.mat[2][2];

		ret.mat[2][0] =
			lhs.mat[2][0] * rhs.mat[0][0] +
			lhs.mat[2][1] * rhs.mat[1][0] +
			lhs.mat[2][2] * rhs.mat[2][0];

		ret.mat[2][1] =
			lhs.mat[2][0] * rhs.mat[0][1] +
			lhs.mat[2][1] * rhs.mat[1][1] +
			lhs.mat[2][2] * rhs.mat[2][1];

		ret.mat[2][2] =
			lhs.mat[2][0] * rhs.mat[0][2] +
			lhs.mat[2][1] * rhs.mat[1][2] +
			lhs.mat[2][2] * rhs.mat[2][2];

		return ret;
	}

	Matrix3& operator*=(const Matrix3& other) {
		*this = *this * other;
		return *this;
	}

	static Matrix3 CreateScale(const float xScale, const float yScale) {
		const float temp[3][3] {
			{ xScale, 0.0f, 0.0f },
			{ 0.0f, yScale, 0.0f },
			{ 0.0f, 0.0f, 1.0f },
		};

		return Matrix3(temp);
	}

	static Matrix3 CreateScale(const Vector2& scaleVector) {
		return CreateScale(scaleVector.x, scaleVector.y);
	}

	static Matrix3 CreateScale(const float scale) {
		return CreateScale(scale, scale);
	}

	static Matrix3 CreateRotation(const float theta) {
		const float temp[3][3] {
			{ Math::Cos(theta), Math::Sin(theta), 0.0f },
			{ -Math::Sin(theta), Math::Cos(theta), 0.0f },
			{ 0.0f, 0.0f, 1.0f },
		};

		return Matrix3(temp);
	}

	static Matrix3 CreateTranslation(const Vector2& trans) {
		const float temp[3][3] {
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			{ trans.x, trans.y, 1.0f },
		};

		return Matrix3(temp);
	}

	static const Matrix3 Identity;
};

class Matrix4 {
public:
	float mat[4][4];

	Matrix4() {
		*this = Matrix4::Identity;
	}

	explicit Matrix4(const float inMat[4][4]) {
		memcpy(mat, inMat, 16 * sizeof(float));
	}

	friend Matrix4 operator*(const Matrix4& lhs, const Matrix4& rhs) {
		Matrix4 ret;

		ret.mat[0][0] =
			lhs.mat[0][0] * rhs.mat[0][0] +
			lhs.mat[0][1] * rhs.mat[1][0] +
			lhs.mat[0][2] * rhs.mat[2][0] +
			lhs.mat[0][3] * rhs.mat[3][0];

		ret.mat[0][1] =
			lhs.mat[0][0] * rhs.mat[0][1] +
			lhs.mat[0][1] * rhs.mat[1][1] +
			lhs.mat[0][2] * rhs.mat[2][1] +
			lhs.mat[0][3] * rhs.mat[3][1];

		ret.mat[0][2] =
			lhs.mat[0][0] * rhs.mat[0][2] +
			lhs.mat[0][1] * rhs.mat[1][2] +
			lhs.mat[0][2] * rhs.mat[2][2] +
			lhs.mat[0][3] * rhs.mat[3][2];

		ret.mat[0][3] =
			lhs.mat[0][0] * rhs.mat[0][3] +
			lhs.mat[0][1] * rhs.mat[1][3] +
			lhs.mat[0][2] * rhs.mat[2][3] +
			lhs.mat[0][3] * rhs.mat[3][3];

		ret.mat[1][0] =
			lhs.mat[1][0] * rhs.mat[0][0] +
			lhs.mat[1][1] * rhs.mat[1][0] +
			lhs.mat[1][2] * rhs.mat[2][0] +
			lhs.mat[1][3] * rhs.mat[3][0];

		ret.mat[1][1] =
			lhs.mat[1][0] * rhs.mat[0][1] +
			lhs.mat[1][1] * rhs.mat[1][1] +
			lhs.mat[1][2] * rhs.mat[2][1] +
			lhs.mat[1][3] * rhs.mat[3][1];

		ret.mat[1][2] =
			lhs.mat[1][0] * rhs.mat[0][2] +
			lhs.mat[1][1] * rhs.mat[1][2] +
			lhs.mat[1][2] * rhs.mat[2][2] +
			lhs.mat[1][3] * rhs.mat[3][2];

		ret.mat[1][3] =
			lhs.mat[1][0] * rhs.mat[0][3] +
			lhs.mat[1][1] * rhs.mat[1][3] +
			lhs.mat[1][2] * rhs.mat[2][3] +
			lhs.mat[1][3] * rhs.mat[3][3];

		ret.mat[2][0] =
			lhs.mat[2][0] * rhs.mat[0][0] +
			lhs.mat[2][1] * rhs.mat[1][0] +
			lhs.mat[2][2] * rhs.mat[2][0] +
			lhs.mat[2][3] * rhs.mat[3][0];

		ret.mat[2][1] =
			lhs.mat[2][0] * rhs.mat[0][1] +
			lhs.mat[2][1] * rhs.mat[1][1] +
			lhs.mat[2][2] * rhs.mat[2][1] +
			lhs.mat[2][3] * rhs.mat[3][1];

		ret.mat[2][2] =
			lhs.mat[2][0] * rhs.mat[0][2] +
			lhs.mat[2][1] * rhs.mat[1][2] +
			lhs.mat[2][2] * rhs.mat[2][2] +
			lhs.mat[2][3] * rhs.mat[3][2];

		ret.mat[2][3] =
			lhs.mat[2][0] * rhs.mat[0][3] +
			lhs.mat[2][1] * rhs.mat[1][3] +
			lhs.mat[2][2] * rhs.mat[2][3] +
			lhs.mat[2][3] * rhs.mat[3][3];

		ret.mat[3][0] =
			lhs.mat[3][0] * rhs.mat[0][0] +
			lhs.mat[3][1] * rhs.mat[1][0] +
			lhs.mat[3][2] * rhs.mat[2][0] +
			lhs.mat[3][3] * rhs.mat[3][0];

		ret.mat[3][1] =
			lhs.mat[3][0] * rhs.mat[0][1] +
			lhs.mat[3][1] * rhs.mat[1][1] +
			lhs.mat[3][2] * rhs.mat[2][1] +
			lhs.mat[3][3] * rhs.mat[3][1];

		ret.mat[3][2] =
			lhs.mat[3][0] * rhs.mat[0][2] +
			lhs.mat[3][1] * rhs.mat[1][2] +
			lhs.mat[3][2] * rhs.mat[2][2] +
			lhs.mat[3][3] * rhs.mat[3][2];

		ret.mat[3][3] =
			lhs.mat[3][0] * rhs.mat[0][3] +
			lhs.mat[3][1] * rhs.mat[1][3] +
			lhs.mat[3][2] * rhs.mat[2][3] +
			lhs.mat[3][3] * rhs.mat[3][3];

		return ret;
	}

	Matrix4& operator*=(const Matrix4& other) {
		*this = *this * other;
		return *this;
	}

	void Invert();

	Vector3 GetTranslation() const {
		return Vector3(mat[3][0], mat[3][1], mat[3][2]);
	}

	Vector3 GetXAxis() const {
		return Vector3::Normalize(Vector3(mat[0][0], mat[0][1], mat[0][2]));
	}

	Vector3 GetYAxis() const {
		return Vector3::Normalize(Vector3(mat[1][0], mat[1][1], mat[1][2]));
	}

	Vector3 GetZAxis() const {
		return Vector3::Normalize(Vector3(mat[2][0], mat[2][1], mat[2][2]));
	}

	Vector3 GetScale() const {
		Vector3 ret;
		ret.x = Vector3(mat[0][0], mat[0][1], mat[0][2]).Length();
		ret.y = Vector3(mat[1][0], mat[1][1], mat[1][2]).Length();
		ret.z = Vector3(mat[2][0], mat[2][1], mat[2][2]).Length();
		return ret;
	}

	static Matrix4 CreateScale(const float xScale, const float yScale, const float zScale) {
		const float temp[4][4] {
			{ xScale, 0.0f, 0.0f, 0.0f },
			{ 0.0f, yScale, 0.0f, 0.0f },
			{ 0.0f, 0.0f, zScale, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		};

		return Matrix4(temp);
	}

	static Matrix4 CreateScale(const Vector3& scaleVector) {
		return CreateScale(scaleVector.x, scaleVector.y, scaleVector.z);
	}

	static Matrix4 CreateScale(const float scale) {
		return CreateScale(scale, scale, scale);
	}

	static Matrix4 CreateRotationX(const float theta) {
		const float temp[4][4] {
			{ 1.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, Math::Cos(theta), Math::Sin(theta), 0.0f },
			{ 0.0f, -Math::Sin(theta), Math::Cos(theta), 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		};

		return Matrix4(temp);
	}

	static Matrix4 CreateRotationY(const float theta) {
		const float temp[4][4] {
			{ Math::Cos(theta), 0.0f, -Math::Sin(theta), 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ Math::Sin(theta), 0.0f, Math::Cos(theta), 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		};

		return Matrix4(temp);
	}

	static Matrix4 CreateRotationZ(const float theta)
	{
		const float temp[4][4] {
			{ Math::Cos(theta), Math::Sin(theta), 0.0f, 0.0f },
			{ -Math::Sin(theta), Math::Cos(theta), 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		};

		return Matrix4(temp);
	}

	static Matrix4 CreateFromQuaternion(const class Quaternion& q);

	static Matrix4 CreateTranslation(const Vector3& trans) {
		const float temp[4][4] {
			{ 1.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ trans.x, trans.y, trans.z, 1.0f }
		};

		return Matrix4(temp);
	}

	static Matrix4 CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
		const auto zaxis = Vector3::Normalize(target - eye);
		const auto xaxis = Vector3::Normalize(Vector3::Cross(up, zaxis));
		const auto yaxis = Vector3::Normalize(Vector3::Cross(zaxis, xaxis));
		const Vector3 trans {
			-Vector3::Dot(xaxis, eye),
			-Vector3::Dot(yaxis, eye),
			-Vector3::Dot(zaxis, eye)
		};

		const float temp[4][4] {
			{ xaxis.x, yaxis.x, zaxis.x, 0.0f },
			{ xaxis.y, yaxis.y, zaxis.y, 0.0f },
			{ xaxis.z, yaxis.z, zaxis.z, 0.0f },
			{ trans.x, trans.y, trans.z, 1.0f }
		};

		return Matrix4(temp);
	}

	static Matrix4 CreateOrtho(const float width, const float height, const float near, const float far) {
		const float temp[4][4] {
			{ 2.0f / width, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 2.0f / height, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f / (far - near), 0.0f },
			{ 0.0f, 0.0f, near / (near - far), 1.0f }
		};

		return Matrix4(temp);
	}

	static Matrix4 CreatePerspectiveFOV(const float fovY, const float width, const float height, const float near, const float far) {
		const auto yScale = Math::Cot(fovY / 2.0f);
		const auto xScale = yScale * height / width;

		const float temp[4][4] {
			{ xScale, 0.0f, 0.0f, 0.0f },
			{ 0.0f, yScale, 0.0f, 0.0f },
			{ 0.0f, 0.0f, far / (far - near), 1.0f },
			{ 0.0f, 0.0f, -near * far / (far - near), 0.0f }
		};

		return Matrix4(temp);
	}

	static Matrix4 CreateSimpleViewProjection(const float width, const float height) {
		const float temp[4][4] = {
			{ 2.0f / width, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 2.0f / height, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 1.0f }
		};

		return Matrix4(temp);
	}

	static const Matrix4 Identity;
};

class Quaternion {
public:
	float x;
	float y;
	float z;
	float w;

	Quaternion() {
		*this = Quaternion::Identity;
	}

	explicit Quaternion(const float inX, const float inY, const float inZ, const float inW) {
		Set(inX, inY, inZ, inW);
	}

	explicit Quaternion(const Vector3& axis, const float angle) {
		const auto scalar = Math::Sin(angle / 2.0f);
		x = axis.x * scalar;
		y = axis.y * scalar;
		z = axis.z * scalar;
		w = Math::Cos(angle / 2.0f);
	}

	void Set(const float inX, const float inY, const float inZ, const float inW) {
		x = inX;
		y = inY;
		z = inZ;
		w = inW;
	}

	void Conjugate() {
		x *= -1.0f;
		y *= -1.0f;
		z *= -1.0f;
	}

	float LengthSquared() const {
		return (x * x + y * y + z * z + w * w);
	}

	float Length() const {
		return Math::Sqrt(LengthSquared());
	}

	void Normalized() {
		const auto length = Length();
		x /= length;
		y /= length;
		z /= length;
		w /= length;
	}

	static Quaternion Normalize(const Quaternion& q) {
		auto ret = q;
		ret.Normalized();
		return ret;
	}

	static Quaternion Lerp(const Quaternion& a, const Quaternion& b, const float f) {
		Quaternion retVal;
		retVal.x = Math::Lerp(a.x, b.x, f);
		retVal.y = Math::Lerp(a.y, b.y, f);
		retVal.z = Math::Lerp(a.z, b.z, f);
		retVal.w = Math::Lerp(a.w, b.w, f);
		retVal.Normalized();
		return retVal;
	}

	static float Dot(const Quaternion& lhs, const Quaternion& rhs) {
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
	}

	static Quaternion Slerp(const Quaternion& a, const Quaternion& b, const float f) {
		float rawCosm = Quaternion::Dot(a, b);

		float cosom = -rawCosm;
		if (rawCosm >= 0.0f)
			cosom = rawCosm;

		float scale0, scale1;

		if (cosom < 0.9999f) {
			const float omega = Math::Acos(cosom);
			const float invSin = 1.f / Math::Sin(omega);
			scale0 = Math::Sin((1.f - f) * omega) * invSin;
			scale1 = Math::Sin(f * omega) * invSin;
		}
		else {
			// Use linear interpolation if the quaternions
			// are collinear
			scale0 = 1.0f - f;
			scale1 = f;
		}

		if (rawCosm < 0.0f) {
			scale1 = -scale1;
		}

		Quaternion ret;
		ret.x = scale0 * a.x + scale1 * b.x;
		ret.y = scale0 * a.y + scale1 * b.y;
		ret.z = scale0 * a.z + scale1 * b.z;
		ret.w = scale0 * a.w + scale1 * b.w;
		ret.Normalized();
		return ret;
	}

	static Quaternion Concatenate(const Quaternion& q, const Quaternion& p) {
		Quaternion ret;

		const Vector3 qv(q.x, q.y, q.z);
		const Vector3 pv(p.x, p.y, p.z);
		const Vector3 newVec = p.w * qv + q.w * pv + Vector3::Cross(pv, qv);
		ret.x = newVec.x;
		ret.y = newVec.y;
		ret.z = newVec.z;
		ret.w = p.w * q.w - Vector3::Dot(pv, qv);

		return ret;
	}

	static const Quaternion Identity;
};

namespace Color {
	static const Vector3 Black(0.0f, 0.0f, 0.0f);
	static const Vector3 White(1.0f, 1.0f, 1.0f);
	static const Vector3 Red(1.0f, 0.0f, 0.0f);
	static const Vector3 Green(0.0f, 1.0f, 0.0f);
	static const Vector3 Blue(0.0f, 0.0f, 1.0f);
	static const Vector3 Yellow(1.0f, 1.0f, 0.0f);
	static const Vector3 LightYellow(1.0f, 1.0f, 0.88f);
	static const Vector3 LightBlue(0.68f, 0.85f, 0.9f);
	static const Vector3 LightPink(1.0f, 0.71f, 0.76f);
	static const Vector3 LightGreen(0.56f, 0.93f, 0.56f);
}
