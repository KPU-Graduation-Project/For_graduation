#pragma once
#include "header_files.h"

class CGameObject
{
public:

protected:
	virtual void Init();

public:
	void SetPosition(fvector3 pos);
	void SetLook(fvector3 pos);

	fvector3 GetPosition();
	fvector3 GetLook();

	explicit CGameObject();
	~CGameObject();

private:

protected:
	fvector3 m_pos;
	fvector3 m_look;
	// 바운딩박스
};