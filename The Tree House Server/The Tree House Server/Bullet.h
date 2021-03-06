#pragma once
#include "GameObject.h"

class cBullet :public cGameObject
{
public:
	cBullet() {};
	virtual ~cBullet() {};

	virtual unsigned int GetObjectType();
	virtual void  Update(float _delta_second);
public:
	iVector3   m_launch_position;
	sRotation3 m_launch_rotation;
};

class cMatch :public cBullet
{
public:
		cMatch() {};
	~cMatch() {};

	virtual unsigned int GetObjectType();
	virtual void  Update(float _delta_second);
private:
	
};


class cSap :public cBullet
{
public:
	cSap() {};
	~cSap() {};

	virtual unsigned int GetObjectType();
	virtual void  Update(float _delta_second);
private:

};