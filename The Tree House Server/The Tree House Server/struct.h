#pragma once
#include <iostream>
#pragma pack (push, 1)

struct PacketInfo
{
	unsigned char size;
	unsigned char type;
};

struct iVector3
{
	int x;
	int y;
	int z;

	iVector3 operator=(const iVector3& rhs){
		//cout << "called vector operator=\n";
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}
	iVector3 operator+(const iVector3& rhs) {
		return iVector3{ x + rhs.x,y + rhs.y,z + rhs.z };
	}
	iVector3 operator+=(const iVector3& rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	iVector3 operator*(const float& rhs) {
		return iVector3{ (short)(x * rhs),(short)(y * rhs),(short)(z * rhs) };
	}
};

struct sVector3
{
	short x;
	short y;
	short z;

	sVector3 operator=(const iVector3& rhs) {
		//cout << "called vector operator=\n";
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}
	sVector3 operator+(const sVector3& rhs) {
		return sVector3{ x + rhs.x,y + rhs.y,z + rhs.z };
	}
	sVector3 operator+=(const sVector3& rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	sVector3 operator*(const float& rhs) {
		return sVector3{ (short)(x * rhs),(short)(y * rhs),(short)(z * rhs) };
	}
};

struct sRotation3
{
	short pitch;
	short yaw;
	short roll;

	sRotation3 operator=(const sRotation3& rhs) {
	//	cout << "called rotation operator=\n";
		pitch = rhs.pitch;
		yaw = rhs.yaw;
		roll = rhs.roll;
		return *this;
	}
	sRotation3 operator+(const sRotation3& rhs) {
		return sRotation3{ pitch + rhs.pitch,yaw + rhs.yaw,roll + rhs.roll };
	}
	sRotation3 operator+=(const sRotation3& rhs) {
		pitch += rhs.pitch;
		yaw += rhs.yaw;
		roll += rhs.roll;
		return *this;
	}
	sRotation3 operator*(const float& rhs) {
		return sRotation3{ (short)(pitch * rhs),(short)(yaw * rhs),(short)(roll * rhs) };
	}
};

struct Transform
{
	iVector3   position;
	sRotation3 rotation;
	sVector3   scale;

	Transform operator=(const Transform& rhs) {
		//cout << "called Transform operator=\n";
		position.x = rhs.position.x;
		position.y = rhs.position.y;
		position.z = rhs.position.z;
		rotation.pitch = rhs.rotation.pitch;
		rotation.yaw = rhs.rotation.yaw;
		rotation.roll = rhs.rotation.roll;
		scale.x = rhs.scale.x;
		scale.y = rhs.scale.y;
		scale.z = rhs.scale.z;
		return *this;
	}
	Transform operator*(const float& rhs) {
		return Transform{ {position * rhs }, {rotation * rhs}, {scale * rhs} };
	}
	Transform operator+=(const Transform& rhs) {
		  position+=rhs.position;
		  rotation += rhs.rotation;
		  scale += rhs.scale;
		  return *this;
	}
};


#pragma pack(pop)