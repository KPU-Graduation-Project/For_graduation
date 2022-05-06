#include "stdafx.h"
#include "Bullet.h"

//---------------------cbullet-----------------------//
unsigned int cBullet::GetObjectType()
{
	static unsigned int object_type = 30000;

	return object_type;
};

void cBullet::Update()
{

}

//---------------------cMatch------------------------//
unsigned int cMatch::GetObjectType()
{
	static unsigned int object_type = 30001;

	return object_type;
};

void cMatch::Update()
{

}

//---------------------cSap------------------------//
unsigned int cSap::GetObjectType()
{
	static unsigned int object_type = 30002;

	return object_type;
};

void cSap::Update()
{

};