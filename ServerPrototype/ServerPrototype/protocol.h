#pragma once

const short SERVER_PORT = 4000;
const int BUFSIZE = 256;

const int  WORLD_HEIGHT = 400;
const int  WORLD_WIDTH = 400;
const int  MAX_NAME_SIZE = 20;

const char CS_PACKET_LOGIN = 1;
const char CS_PACKET_MOVE = 2;

const char SC_PACKET_LOGIN_OK = 1;
const char SC_PACKET_MOVE = 2;
const char SC_PACKET_PUT_OBJECT = 3;
const char SC_PACKET_REMOVE_OBJECT = 4;

#pragma pack (push, 1)

//모든 패킷에 공통으로 들어감.
//무조건 패킷 맨 앞에 위치
struct packet_info             
{
	unsigned char size;
	char	      type;
};

// 기본 플레이어 데이터. 
struct player_data{  
	// Head
	float HeadLookDir_x;
	float HeadLookDir_y;
	float HeadLookDir_z;

	float HeadPosition_x;
	float HeadPosition_y;
	float HeadPosition_z;

	// Body
	float BodyLookDir_x;
	float BodyLookDir_y;
	float BodyLookDir_z;

	float BodyPosition_x;
	float BodyPosition_y;

	// LeftHand
	float HandLAngle_x;
	float HandLAngle_y;
	float HandLAngle_z;

	float HandLPosition_x;
	float HandLPosition_y;
	float HandLPosition_z;

	float HandLFingerAngle0;
	float HandLFingerAngle1;
	float HandLFingerAngle2;
	float HandLFingerAngle3;
	float HandLFingerAngle4;

	// RightHand
	float HandRAngle_x;
	float HandRAngle_y;
	float HandRAngle_z;

		float HandRPosition_x;
	float HandRPosition_y;
	float HandRPosition_z;

	float HandRFingerAngle0;
	float HandRFingerAngle1;
	float HandRFingerAngle2;
	float HandRFingerAngle3;
	float HandRFingerAngle4;

};

// 통신용 플레이어 데이터. 기본 플레이어 데이터에서 서버에서 필요로 하는 데이터만 포함.
// 자주 변경될 것 같아서 일단 하드코딩, 이후 float3 등으로 묶겠음
struct player_data_to_send
{
	// Head
	float head_pos_x;
	float head_pos_y;
	float head_pos_z;

	float head_look_x;
	float head_look_y;
	float head_look_z;
	
	// Body
	float body_pos_x;
	float body_pos_y;

	float body_look_x;
	float body_look_y;
	float body_look_z;

	// LeftHand
	float lhand_pos_x;
	float lhand_pos_y;
	float lhand_pos_z;

	float lhand_angle_x;
	float lhand_angle_y;
	float lhand_angle_z;

	float lhand_finger_angle_0;
	float lhand_finger_angle_1;
	float lhand_finger_angle_2;
	float lhand_finger_angle_3;
	float lhand_finger_angle_4;

	// RightHand
	float rhand_pos_x;
	float rhand_pos_y;
	float rhand_pos_z;

	float rhand_angle_x;
	float rhand_angle_y;
	float rhand_angle_z;

	float rhand_finger_angle_0;
	float rhand_finger_angle_1;
	float rhand_finger_angle_2;
	float rhand_finger_angle_3;
	float rhand_finger_angle_4;
};

//---------------------------------------------------
//             client to server packet
//---------------------------------------------------
struct cs_packet_login {
	packet_info           info;
	char	              name[MAX_NAME_SIZE];
};

struct cs_packet_move {
	packet_info           info;
	player_data_to_send   player_data;
};


//---------------------------------------------------
//             server to client packet
//---------------------------------------------------
struct sc_packet_login_ok {
	packet_info           info;
	short		          id;          // 플레이어 식별키
	//short                 room_num;    // 다중 매칭시 방 번호. 다중 매칭 구현 전까지 기본 0;
	bool                  is_char1;    // true: 1P  false: 2P
	player_data_to_send   player_data; // 초기 플레이어 데이터
};

struct sc_packet_move {
	packet_info   info;
	short		id;
	short  x, y;
};

struct sc_packet_put_object {
	packet_info   info;
	short id;
	short x, y;
	char object_type;
	char	name[MAX_NAME_SIZE];
};

struct sc_packet_remove_object {
	packet_info   info;
	short id;
};
#pragma pack(pop)
