#pragma once
#include "GameObject.h"

//-----------------------------//
//          cMonster                                   
//-----------------------------//
class cMonster :public cGameObject
{
public:
	cMonster() { Init(); };
	virtual ~cMonster() {};

	void Init() { ClearDeltaTransform(); };

	void SetDeltaTransform(const Transform& _delta_transform) { m_delta_transform = _delta_transform; }
	void SetDeltaPosition(const iVector3& _delta_position) { m_delta_transform.position = _delta_position; }
	void SetDeltaRotation(const sRotation3& _delta_rotation) { m_delta_transform.rotation = _delta_rotation; }
	void SetDeltaScale(const sVector3& _delta_scale) { m_delta_transform.scale = _delta_scale; }
	void ClearDeltaTransform() { m_delta_transform = { {0,0,0},{0,0,0},{0,0,0} }; }

	// stop behaving: next_state= 0
	virtual void ChangeBehavior(unsigned char next_state) {};
	//virtual void ProgressNextBehavior();
	virtual void  Update(float _delta_second) {};

	virtual unsigned int GetObjectType() { return OBJECT_TYPE::MONSTER; };
public:
	// for now, linear function only
	Transform         m_delta_transform;
	unsigned char     m_behavior=0;

};

//-----------------------------//
//                                            
//-----------------------------//
class cPlayZoneTarget :public cMonster
{
public:
	enum BEHAVIOR
	{
		NONE = 0, 
		BH1_NONE, BH1_MOVE_DOWN1, BH1_HOLD1, BH1_MOVE_UP1, BH1_HOLD2,
		BH2_NONE, BH2_ROTATE_PITCH_DOWN1, /*BH2_HOLD1*/ BH2_ROTATE_PITCH_UP1, /*BH2_HOLD2*/
		MAX
	};
public:
	cPlayZoneTarget() {};
	virtual ~cPlayZoneTarget() {};

	virtual void ChangeBehavior(unsigned char next_state);
	void ProgressNextBehavior();
	virtual void  Update(float _delta_second);

	virtual unsigned int GetObjectType() { return OBJECT_TYPE::PLAY_ZONE_TARGET; };
};