#pragma once
#include <list>
#include "GameObject.h"

//------------------Character------------------//
class cCharacter :public cGameObject
{
public:
	cCharacter() {};
	virtual ~cCharacter() {};

	void SetCharacterTransform(
		const iVector3& _position, const sRotation3& _rotation,
		const iVector3& _head_position, const sRotation3& _head_rotation,
		const iVector3& _rh_position, const sRotation3& _rh_rotation,
		const iVector3& _lh_position, const sRotation3& _lh_rotation);
	
	virtual unsigned int GetObjectType() ;
	virtual void  Update();
	//list<unsigned short >m_view_list;
public:
	iVector3  m_head_position;
	sRotation3 m_head_rotation;

	iVector3  m_rh_position;
	sRotation3 m_rh_rotation;

	iVector3  m_lh_position;
	sRotation3 m_lh_rotation;
};

//----------------GirlCharacter----------------//
class cGirlCharacter :public cCharacter
{
public:
	cGirlCharacter() {};
	virtual ~cGirlCharacter() {};

	virtual unsigned int GetObjectType();
	virtual void  Update();
public:

};

//----------------BoyCharacter----------------//
class cBoyCharacter :public cCharacter
{
public:
	cBoyCharacter() {};
	virtual ~cBoyCharacter() {};

	virtual unsigned int GetObjectType() ;
	virtual void  Update();
public:

};