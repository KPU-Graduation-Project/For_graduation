#pragma once
#include "stdafx.h"
#include "User.h"



unsigned short CUser::GetID() { return m_id; }
E_USER_STATE CUser::GetState() { return m_state; };
unsigned short CUser::GetRoomNum() { return m_room_num; };

void CUser::SetID(const unsigned short _id){	m_id = _id;}
void CUser::SetState(const E_USER_STATE _state) { m_state = _state; };
void CUser::SetRoomNum(const unsigned short _room_num) { m_room_num = _room_num; };

CUser::CUser() {};
CUser::~CUser() {};
