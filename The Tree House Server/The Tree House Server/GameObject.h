#pragma once
#include "struct.h"

enum class OBJECT_STATE
{
	FREE = 0, ALIVE = 1, STOP = 2, DESTROYING = 3
};

enum OBJECT_TYPE
{
	CHARACTER = 100000, GIRL, BOY,
	MONSTER = 200000, PLAY_ZONE_TARGET, TARGET_DOLL,
	BULLET = 300000, MATCH, SAP,
	DYNAMIC_OBJECT = 400000, TARGET, BOX,DESTRUCTIBLE,
	PASSIVE_OBJECT=500000,DOOR
};


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
	virtual void  Update(float _delta_second) {};
public:
	unsigned int  m_id;
	unsigned int  m_object_type;	
	unsigned char m_mesh_id;

	OBJECT_STATE m_state;
	
	Transform m_transform;
	unsigned char m_sector;

	unsigned int m_parent_object;
};