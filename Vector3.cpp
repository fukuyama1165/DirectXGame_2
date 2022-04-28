#include "Vector3.h"


Vector3::Vector3()
{
	x = 0;
	y = 0;
	z = 0;
}
Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

// 単項演算子オーバーロード
Vector3 Vector3::operator+() const
{
	return *this;
}
Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

// 代入演算子オーバーロード
Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}
Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}
Vector3& Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}
Vector3& Vector3::operator/=(float s)
{
	if (s != 0)
	{
		x /= s;
		y /= s;
		z /= s;
	}
	return *this;
}