#pragma once
#include "GameObject.h"

class cBullet :public cGameObject
{
public:
	cBullet() {};
	~cBullet() {};
private:

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