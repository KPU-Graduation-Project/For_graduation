#pragma once
#include "struct.h"

class CGameObject
{
public:



public:
	virtual void Init() = 0;

	void SetPosition(fvector3 pos);
	void SetLook(fvector3 pos);

	fvector3 GetPosition();
	fvector3 GetLook();

	CGameObject();
	~CGameObject();

private:

protected:
	fvector3 m_pos;
	fvector3 m_look;
	// 바운딩박스
};