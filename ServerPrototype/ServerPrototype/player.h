#pragma once
#include "game_object.h"
#include "header_files.h"
#include "protocol.h"

class CPlayer:CGameObject
{
public:
	void Init();

	//void SetPosition(fvector3 pos);
	//void SetLook(fvector3 pos);
	void SetHeadPosition(fvector3 pos);
	void SetHeadLook(fvector3 pos);
	void SetLHandPosition(fvector3 pos);
	void SetLHandAngle(fvector3 pos);
	void SetRHandPosition(fvector3 pos);
	void SetRHandAngle(fvector3 pos);

	//fvector3 GetPosition();
	//fvector3 GetLook();
	fvector3 GetHeadPosition();
	fvector3 GetHeadLook();
	fvector3 GetLHandPosition();
	fvector3 GetLHandAngle();
	fvector3 GetRHandPosition();
	fvector3 GetRHandAngle();



	explicit CPlayer();
	~CPlayer();

private:
	// Head
	fvector3 m_head_pos;
	fvector3 m_head_look;

	//// Body
	//fvector3 m_body_pos;
	//fvector3 m_body_look;

	// LeftHand
	fvector3 m_lhand_pos;
	fvector3 m_lhand_angle;

	float m_lhand_finger_angle0;
	float m_lhand_finger_angle1;
	float m_lhand_finger_angle2;
	float m_lhand_finger_angle3;
	float m_lhand_finger_angle4;

	// RightHand
	fvector3 m_rhand_pos;
	fvector3 m_rhand_angle;	

	float m_rhand_finger_angle0;
	float m_rhand_finger_angle1;
	float m_rhand_finger_angle2;
	float m_rhand_finger_angle3;
	float m_rhand_finger_angle4;
};