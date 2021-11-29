#pragma once

//모든 패킷에 공통으로 들어감.
//무조건 패킷 맨 앞에 위치
struct packet_info
{
	unsigned char size;
	char	      type;
};

struct fvector3
{
	float x;
	float y;
	float z;
};

// 기본 플레이어 데이터. 
struct player_data {
	// Head
	fvector3 HeadLookDir;
	fvector3 HeadPosition;

	// Body
	fvector3 BodyLookDir;
	fvector3 BodyPosition;

	// LeftHand
	fvector3 LHandAngle;
	fvector3 LHandPosition;
	float    LHandFingerAngle[5];

	// RightHand
	fvector3 RHandAngle;
	fvector3 RHandPosition;
	float    RHandFingerAngle[5];
};



