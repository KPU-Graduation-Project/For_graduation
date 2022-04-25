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

	void operator=(const iVector3& rhs){
		cout << "called vector operator=\n";
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
	}
};

struct sVector3
{
	short x;
	short y;
	short z;

	void operator=(const iVector3& rhs) {
		cout << "called vector operator=\n";
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
	}
};

struct sRotation3
{
	short pitch;
	short yaw;
	short roll;

	void operator=(const sRotation3& rhs) {
		cout << "called rotation operator=\n";
		pitch = rhs.pitch;
		yaw = rhs.yaw;
		roll = rhs.roll;
	}
};

struct Transform
{
	iVector3   position;
	sRotation3 rotation;
	sVector3   scale;

	void operator=(const Transform& rhs) {
		cout << "called Transform operator=\n";
		position.x = rhs.position.x;
		position.y = rhs.position.y;
		position.z = rhs.position.z;
		rotation.pitch = rhs.rotation.pitch;
		rotation.yaw = rhs.rotation.yaw;
		rotation.roll = rhs.rotation.roll;
		scale.x = rhs.scale.x;
		scale.y = rhs.scale.y;
		scale.z = rhs.scale.z;
	}
};


#pragma pack(pop)