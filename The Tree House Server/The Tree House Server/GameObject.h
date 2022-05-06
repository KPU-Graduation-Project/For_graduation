#pragma once
#include "struct.h"

class cGameObject
{
public:
	cGameObject();
	virtual ~cGameObject();
		
	void SetID(const unsigned int& _id) { m_id = _id; };
	void SetPosition(const iVector3& _position) { m_transform.position = _position; };
	void SetRotation(const sRotation3& _rotation) { m_transform.rotation = _rotation; };
	void SetScale(const sVector3& _scale) { m_transform.scale = _scale; }
	void SetTransform(const Transform& _transform) { m_transform = _transform; };

	unsigned int GetID() { return m_id; };
	iVector3 GetPosition() { return m_transform.position; };
	sRotation3 GetRotation() { return m_transform.rotation; };
	sVector3 GetScale() { return m_transform.scale; };
	Transform GetTransform() { return m_transform; };

	virtual unsigned int GetObjectType() { return 0; };
	virtual void  Update() {};
public:
	// have to be static
	unsigned int  m_object_type;

	unsigned int m_id;
	unsigned char m_mesh_id;

	unsigned char m_sector;

	Transform m_transform;
};