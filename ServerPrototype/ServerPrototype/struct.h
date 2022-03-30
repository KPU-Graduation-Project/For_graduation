#pragma once

//��� ��Ŷ�� �������� ��.
//������ ��Ŷ �� �տ� ��ġ
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

// �⺻ �÷��̾� ������. 
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



