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
	sRotation3 ratation;

	void operator=(const Transform& rhs) {
		cout << "called Transform operator=\n";
		position.x = rhs.position.x;
		position.y = rhs.position.y;
		position.z = rhs.position.z;
		ratation.pitch = rhs.ratation.pitch;
		ratation.yaw = rhs.ratation.yaw;
		ratation.roll = rhs.ratation.roll;
	}
};


#pragma pack(pop)