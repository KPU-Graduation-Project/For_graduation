#pragma once
#include "struct.h"

class CGameObject
{
public:
	CGameObject();
	~CGameObject();

	void SetID(const unsigned short _id);
	void SetPosition(const int _x, const int _y, const int _z);
	void SetRotation (const short _pitch, const short _yaw, const short _roll);
	void SetTransform(const int _x, const int _y, const int _z,
		const short _pitch, const short _yaw, const short _roll);

	unsigned short GetID();
	iVector3 GetPosition();
	sRotation3 GetRotation();
	Transform GetTransform();

public:
	unsigned short m_id;

	iVector3   m_position;
	sRotation3 m_rotation;
};