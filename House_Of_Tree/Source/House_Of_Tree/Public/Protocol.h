#pragma once


constexpr short SERVER_PORT = 6000;
constexpr int   BUFSIZE = 512;


constexpr unsigned int  MAX_USER = 10;
constexpr unsigned int  MAX_ROOM = 5;

namespace CS_PACKET
{
    enum ePACKET_TYPE
    {
        NONE = 0,
        CS_CREATE_ROOM, CS_JOIN_ROOM, CS_JOIN_RANDOM_ROOM, CS_READY_GAME, CS_CHANGE_SELECTED_CHARACTER,
        CS_START_GAME, CS_LOADING_COMPLETE,
        CS_SHOOT_BULLET, CS_BULLET_HIT, CS_PLAYER_DATA
    };
};

namespace SC_PACKET
{

    enum ePACKET_TYPE
    {
        NONE = 0,
        SC_LOGINOK,
        SC_CREATE_ROOM, SC_JOIN_ROOM, SC_USER_JOIN_ROOM, USER_EXIT_ROOM, SC_USER_READY_GAME, SC_USER_CHANGE_SELECTED_CHARACTER,
        SC_START_GAME, SC_ALL_USERS_LOADING_COMPLETE,
        SC_PUT_OBJECT, SC_REMOVE_OBJECT, SC_OBJECT_DATA, SC_PLAYER_DATA
    };

};

#pragma pack (push, 1)
//--------------------CS PACKET----------------------//
//---------------------------------------------------//
struct cs_create_room_packet
{
    unsigned char size;
    unsigned char type;
};

struct cs_join_room_packet
{
    unsigned char size;
    unsigned char type;

    unsigned int room_id;
};

struct cs_join_random_room_packet
{
    unsigned char size;
    unsigned char type;
};

struct cs_ready_game_packet
{
    unsigned char size;
    unsigned char type;

    bool is_ready;
};

struct cs_change_selected_character
{
    unsigned char size;
    unsigned char type;

    char          selected_character;
};

struct cs_start_game_packet
{
    unsigned char size;
    unsigned char type;
};  //보류

struct cs_loading_complete_packet
{
    unsigned char size;
    unsigned char type;
};

struct cs_player_data_packet
{

    unsigned char size;
    unsigned char type;

    unsigned int id;

    // Character Transform
    int           x;
    int           y;
    int           z;
    short         pitch;
    short         yaw;
    short         roll;

    // Head(VR) Transform
    int           head_x;
    int           head_y;
    int           head_z;
    short         head_pitch;
    short         head_yaw;
    short         head_roll;

    // RightHand Transform
    int           rh_x;
    int           rh_y;
    int           rh_z;
    short         rh_pitch;
    short         rh_yaw;
    short         rh_roll;

    // LeftHand Transform
    int           lh_x;
    int           lh_y;
    int           lh_z;
    short         lh_pitch;
    short         lh_yaw;
    short         lh_roll;
};

struct cs_shoot_bullet_packet
{
    unsigned char size;
    unsigned char type;

    unsigned int id;

    int x;
    int y;
    int z;
    short pitch;
    short yaw;
    short roll;
};

struct cs_bullet_hit_packet
{
    unsigned char size;
    unsigned char type;

    unsigned int id;

    int x;
    int y;
    int z;
    short pitch;
    short yaw;
    short roll;
};


//--------------------SC PACKET----------------------//
//---------------------------------------------------//
struct sc_loginok_packet
{
    unsigned char size;
    unsigned char type;

    unsigned int id;
};

struct sc_create_room_packet
{
    unsigned char size;
    unsigned char type;

    unsigned int  room_id;
    bool          is_ready;
    char          selected_character;

};

struct sc_join_room_packet
{
    unsigned char size;
    unsigned char type;

    unsigned int  room_id;
    bool          is_ready;
    char          selected_character;
};

struct sc_user_join_room_packet
{
    unsigned char size;
    unsigned char type;

    unsigned int  id;
    bool          is_ready;
    char          selected_character;
};

struct sc_user_exit_room_packet
{
    unsigned char size;
    unsigned char type;

    unsigned int  id;
};

struct sc_user_ready_game_packet
{
    unsigned char size;
    unsigned char type;

    unsigned int  id;
    bool          is_ready;
};

struct sc_user_change_selected_character
{
    unsigned char size;
    unsigned char type;

    unsigned int  id;
    char          selected_character;
};

struct sc_start_game_packet
{
    unsigned char size;
    unsigned char type;
};

struct sc_all_users_loading_complete_packet
{
    unsigned char size;
    unsigned char type;
};

struct sc_put_object_packet
{
    unsigned char size;
    unsigned char type;

    unsigned int  id;
    unsigned int object_type;
    unsigned char mesh_id;

    int           x;
    int           y;
    int           z;
    short         pitch;
    short         yaw;
    short         roll;
    short         scale_x;
    short         scale_y;
    short         scale_z;
};

struct sc_remove_object_packet
{
    unsigned char size;
    unsigned char type;

    unsigned int  id;
};

struct sc_object_data_packet
{
    unsigned char size;
    unsigned char type;

    unsigned int  id;

    int           x;
    int           y;
    int           z;
    short         pitch;
    short         yaw;
    short         roll;
    short         scale_x;
    short         scale_y;
    short         scale_z;
};

struct sc_player_data_packet
{
    unsigned char size;
    unsigned char type;

    unsigned int  id;

    // Character Transform
    int           x;
    int           y;
    int           z;
    short         pitch;
    short         yaw;
    short         roll;

    // Head(VR) Transform
    int           head_x;
    int           head_y;
    int           head_z;
    short         head_pitch;
    short         head_yaw;
    short         head_roll;

    // RightHand Transform
    int           rh_x;
    int           rh_y;
    int           rh_z;
    short         rh_pitch;
    short         rh_yaw;
    short         rh_roll;

    // LeftHand Transform
    int           lh_x;
    int           lh_y;
    int           lh_z;
    short         lh_pitch;
    short         lh_yaw;
    short         lh_roll;

};
#pragma pack(pop)