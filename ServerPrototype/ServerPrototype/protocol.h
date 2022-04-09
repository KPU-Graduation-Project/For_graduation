#pragma once
#include "struct.h"

const short SERVER_PORT = 5000;
const int   BUFSIZE = 512;

const int  WORLD_HEIGHT = 400;
const int  WORLD_WIDTH = 400;
const int  MAX_NAME_SIZE = 20;


//===================================================
//             CLIENT TO SERVER PACKET NUM
//===================================================
//--------------------InTitle------------------------
const char CS_PACKET_LOGIN = 1;

//--------------------InLobby------------------------
const char CS_PACKET_PLAY_RANDOM_MATCHING = 2;
const char CS_PACKET_CREATE_MATCHING_ROOM = 3;
const char CS_PACKET_JOIN_MATCHING_ROOM = 4;
const char CS_PACKET_STOP_MATHCING = 5;

//----------------InMatchingRoom---------------------
const char CS_PACKET_CHANGE_CHARACTER = 6;
const char CS_PACKET_CHANGE_READY = 7;

//--------------------InGame------------------------


      

//===================================================
//             SERVER TO CLIENT PACKET NUM
//===================================================
//--------------------InTitle------------------------
const char SC_PACKET_LOGIN_OK = 1;

//--------------------InLobby------------------------
const char SC_PACKET_START_RANDOM_MATHCING = 2;
const char SC_PACKET_MATHCING_ROOM_CREATED = 3;
const char SC_PACKET_JOINING_MATCHING_ROOM = 4;
const char SC_PACKET_STOP_MATHCING = 5;
const char SC_PACKET_PLAYER_MATCHED = 6;

//----------------InMatchingRoom---------------------
const char SC_PACKET_CHANGE_CHARACTER = 7;
const char SC_PACKET_CHANGE_READY = 8;
const char SC_PACKET_GAME_START = 9;

//--------------------InGame------------------------
const char SC_PACKET_CHANGE_STAGE = 10;

#pragma pack (push, 1)
//===================================================
//             CLIENT TO SERVER PACKET
//===================================================
//--------------------InTitle------------------------
struct cs_packet_login 
{
	packet_info           info;
	char	              name[MAX_NAME_SIZE]; //사용? 미사용?
};

//--------------------InLobby------------------------
struct cs_packet_play_random_matching
{
	packet_info           info;
};

struct cs_packet_create_matching_room
{
	packet_info           info;
};


struct cs_packet_join_matching_room
{
	packet_info           info;
	short                 room_name; // 방 생성자의 id로 방 매칭
};

struct cs_packet_stop_matching
{
	packet_info           info;
	// 0 = stop random_mathcing / 1 = exit created mathcing room / 2 = stop joining matching room 
	char                  matching_type; 
};

//------------------InMatchingRoom--------------------
struct cs_packet_change_character
{
	packet_info           info;
	char                  choose_char; // 0 = char_1 / 1 = char_2 / 2 = 가운데(최초 입장시에만)
};

struct cs_packet_change_ready  
{
	packet_info           info;
	char                  try_ready; // 0 = try to unready / 1 = try to ready
};

//---------------------InGame------------------------
struct cs_packet_player_move
{
	packet_info           info;

};


//===================================================
//             SERVER TO CLIENT
//===================================================
//--------------------InTitle------------------------
struct sc_packet_login_ok 
{
	packet_info           info;
	short		          id;          // 플레이어 식별키
};

//--------------------InLobby------------------------
// cs_packet_play_random_matching 수신시 매칭 큐에 들어갔음을 알림
struct sc_packet_start_random_matching 
{
	packet_info           info;
};

// cs_packet_create_matching_room 수신시 방을 생성하고 생성되었음을 알림
struct sc_packet_matching_room_created  
{
	packet_info           info;
	short                 room_name; // 방 생성자의 id로 방 매칭
};

// cs_packet_join_matching_room 수신시 해당 방 이름으로 매칭 대기중임을 알림
struct sc_packet_joining_matching_room
{
	packet_info           info;
	short                 room_name;  // 방 생성자의 id로 방 매칭
};

struct sc_packet_stop_matching
{
	packet_info           info;
};

// 매칭 대기중 혹은 방 생성 대기중인 플레이어에게 매칭이 잡혔음을 알림
// 이후 매칭 상대 등에 대한 정보를 뒤이어 송신
struct sc_packet_player_matched
{
	packet_info           info;
	short                 other_id;
};

//------------------InMatchingRoom--------------------
struct sc_packet_change_character
{
	packet_info           info;
	short                 id;
	char                  choose_char; // 0 = char_1 / 1 = char_2 / 2 = 가운데(최초 입장시에만)
};

struct sc_packet_change_ready
{
	packet_info           info;
	short                 id;
	char                  ready; // 0 = diable reay button / 1 = unready / 2 = ready
};

// 두 플레이어가 다 준비했을 때 게임 시작
struct sc_packet_game_start
{
	packet_info           info;
	char                  stage; // 처음엔 sc_packet_game_start로 stage 전송, 이후엔 sc_packet_change_stage로
};

//---------------------InGame------------------------



//struct sc_packet_move {
//	packet_info   info;
//	short		id;
//	short  x, y;
//};
//
//struct sc_packet_put_object {
//	packet_info   info;
//	short id;
//	short x, y;
//	char object_type;
//	char	name[MAX_NAME_SIZE];
//};
//
//struct sc_packet_remove_object {
//	packet_info   info;
//	short id;
//};

// 게임을 진행하면서 스테이지가 바뀔때 전송
struct sc_packet_change_stage
{
	packet_info           info;
	char                  stage;
};













struct sc_packet_put_player
{
	packet_info           info;

	short                 id;
	fvector3              pos;
	fvector3              look;
	fvector3              up;

	//헤드트래킹 정보?

	//핸드트래킹 정보?
};

//달리기 등 다른 스테이트가 없다면, 단순 move와 stop만으로 가능
	struct cs_packet_move_player
	{
		packet_info           info;

		fvector3              dir;
	};

	struct cs_packet_stop_player
	{
		packet_info           info;
	};

	struct sc_packet_move_player
	{
		packet_info           info;
		short                 id;

		fvector3              dir;
	};

	struct sc_packet_stop_player
	{
		packet_info           info;
		short                 id;
	};


	//사격 - 키다운시 딜레이를 가지고 연사가 가능한 상황으로 상정 / 무기 고정(변경불가)으로 상정
	struct cs_packet_shoot
	{
		packet_info           info;

	};

	struct cs_packet_stop_shooting
	{
		packet_info           info;
	};

	struct sc_packet_shoot
	{
		packet_info           info;

		//총알 종류
		fvector3              pos;
		fvector3              look;
	};


struct sc_packet_put_player
{
	packet_info           info;

	short                 id;
	fvector3              pos;
	fvector3              look;
	fvector3              up;

	//헤드트래킹 정보?

	//핸드트래킹 정보?
};

//핸드트래킹 정보
struct cs_packet_put_player
{
	packet_info           info;

	fvector3              look;
	fvector3              up;

	//헤드트래킹 정보?

	//핸드트래킹 정보?
};


struct sc_packet_put_object
{
	packet_info           info;

	//오브젝트 id
	//오브젝트 타입
	fvector3              pos;
	fvector3              look;
	fvector3              up;


};



////////
struct sc_packet_put_object
{
	packet_info           info;
};

#pragma pack(pop)