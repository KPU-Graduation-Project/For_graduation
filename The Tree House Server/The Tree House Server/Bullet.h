#pragma once
#include "GameObject.h"

class cBullet :public cGameObject
{
public:
	cBullet() {};
	~cBullet() {};
private:
	iVector3   m_launch_point;
	sRotation3 m_launch_rotation;
};

class cMatch :public cBullet
{
public:
	cMatch() {};
	~cMatch() {};

private:
	
};


class cSap :public cBullet
{
public:
	cSap() {};
	~cSap() {};

private:

};