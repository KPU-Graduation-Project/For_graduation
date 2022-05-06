#include "stdafx.h"
#include "Character.h"




void cCharacter::SetCharacterTransform(
	const iVector3& _position, const sRotation3& _rotation,
	const iVector3& _head_position, const sRotation3& _head_rotation,
	const iVector3& _rh_position, const sRotation3& _rh_rotation,
	const iVector3& _lh_position, const sRotation3& _lh_rotation)
{
	m_transform.position = _position;
	m_transform.rotation = _rotation;
	m_head_position = _head_position;
	m_head_rotation = _head_rotation;
	m_rh_position = _rh_position;
	m_rh_rotation = _rh_rotation;
	m_lh_position = _lh_position;
	m_lh_rotation = _lh_rotation;
}

unsigned int cCharacter::GetObjectType() 
{
	static unsigned int object_type = 10000;
	return object_type;
};
void  cCharacter::Update() {};

//----------------GirlCharacter----------------//
unsigned int cGirlCharacter::GetObjectType()
{
	static unsigned int object_type = 10001;
	return object_type;
};

void  cGirlCharacter::Update()
{

};



//----------------BoyCharacter----------------//

unsigned int cBoyCharacter::GetObjectType() 
{
	static unsigned int object_type = 10002;
	return object_type;
};

void  cBoyCharacter::Update()
{

};