#include "stdafx.h"
#include "Character.h"

cCharacter::cCharacter() {};
cCharacter::~cCharacter() {};


void cCharacter::SetCharacterTransform(
	const iVector3& _position, const sRotation3& _rotation,
	const iVector3& _head_position, const sRotation3& _head_rotation,
	const iVector3& _rh_position, const sRotation3& _rh_rotation,
	const iVector3& _lh_position, const sRotation3& _lh_rotation)
{
	m_position = _position;
	m_rotation = _rotation;
	m_head_position = _head_position;
	m_head_rotation = _head_rotation;
	m_rh_position = _rh_position;
	m_rh_rotation = _rh_rotation;
	m_lh_position = _lh_position;
	m_lh_rotation = _lh_rotation;
}