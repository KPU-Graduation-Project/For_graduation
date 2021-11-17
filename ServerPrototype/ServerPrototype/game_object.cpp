#include "game_object.h"

explicit CGameObject::CGameObject() {};

CGameObject::~CGameObject() {};

void CGameObject::SetPosition(fvector3 pos) { m_pos = pos; };

void CGameObject::SetLook(fvector3 look) { m_look = look; }

fvector3 CGameObject::GetPosition() { return m_pos; }

fvector3 CGameObject::GetLook() { return m_look; }