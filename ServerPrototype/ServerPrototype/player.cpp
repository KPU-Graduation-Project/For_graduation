#include "player.h"



void CPlayer::Init() {}

//void SetPosition(fvector3 pos);
// 
//void SetLook(fvector3 pos);

void CPlayer::SetHeadPosition(fvector3 pos) {	m_head_pos = pos;}

void CPlayer::SetHeadLook(fvector3 look) { m_head_look = look; }

void CPlayer::SetLHandPosition(fvector3 pos) { m_lhand_pos = pos; }

void CPlayer::SetLHandAngle(fvector3 angle) { m_lhand_angle = angle; }

void CPlayer::SetRHandPosition(fvector3 pos) { m_rhand_pos = pos; }

void CPlayer::SetRHandAngle(fvector3 angle) { m_rhand_angle = angle; }

//fvector3 GetPosition();
// 
//fvector3 GetLook();

fvector3 CPlayer::GetHeadPosition() { return m_head_pos; }

fvector3 CPlayer::GetHeadLook() { return m_head_look; }

fvector3 CPlayer::GetLHandPosition() {	return m_lhand_pos;}

fvector3 CPlayer::GetLHandAngle() { return m_lhand_angle; }

fvector3 CPlayer::GetRHandPosition() { return m_rhand_pos; }

fvector3 CPlayer::GetRHandAngle() { return m_rhand_angle; }

explicit CPlayer::CPlayer() {};

CPlayer::~CPlayer() {};