#include "stdafx.h"
#include "Gameobject.h"


CGameObject::CGameObject() {};
CGameObject::~CGameObject() {};

void CGameObject::SetID(const unsigned short _id) { m_id = _id; }
void CGameObject::SetPosition(const int _x, const int _y, const int _z)
{
	m_position.x = _x; m_position.y = _y; m_position.z = _z;
};
void CGameObject::SetRotation(const short _pitch, const short _yaw, const short _roll)
{
	 m_rotation.pitch = _pitch; m_rotation.yaw = _yaw; m_rotation.roll = _roll;
}
void CGameObject::SetTransform(const int _x, const int _y, const int _z,
	 const short _pitch, const short _yaw, const short _roll )
{
	m_position.x = _x; m_position.y = _y; m_position.z = _z;
	m_rotation.pitch = _pitch; m_rotation.yaw = _yaw; m_rotation.roll = _roll;
}

unsigned short CGameObject::GetID() { return m_id; }
iVector3 CGameObject::GetPosition() { return m_position; }
sRotation3 CGameObject::GetRotation() { return m_rotation; }
Transform CGameObject::GetTransform() { return Transform{ m_position,m_rotation }; };
