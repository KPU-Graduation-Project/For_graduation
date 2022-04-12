#pragma once
#include "stdafx.h"
#include "Room.h"

void cRoom::StartGame()
{
	//월드 초기값 설정

	//
	if (m_state == room_state::INGAME)
	{
		for (int i = 0; i < 2; ++i)
		{
			sc_put_player_packet packet;

			packet.size = sizeof(sc_put_player_packet);
			packet.type = SC_PACKET::SC_PUT_PLAYER;
			packet.character_type = 0; // 
			packet.id = m_users[i]->GetID();

			m_characters[i]->SetPosition(0, 0, 0);
			m_characters[i]->SetRotation(0, 0, 0);

			iVector3 character_position = m_characters[i]->GetPosition();
			sRotation3 character_rotation = m_characters[i]->GetRotation();

			packet.x = character_position.x;
			packet.y = character_position.y;
			packet.z = character_position.z;
			packet.pitch = character_rotation.pitch;
			packet.yaw = character_rotation.yaw;
			packet.roll = character_rotation.roll;


			m_users[user_type::HOST]->Send(sizeof(packet), &packet);
			m_users[user_type::GUEST]->Send(sizeof(packet), &packet);
		}
	}

}

void cRoom::SendPlayerTransform()
{
	if (m_state == room_state::INGAME)
	{
		for (int i = 0; i < 2; ++i)
		{
			/*cout << "send - user" << i << " position " << g_temp_position[i].x << "," << g_temp_position[i].y << ", " << g_temp_position[i].z
				<< " // rotation " << g_temp_rotation[i].x << "," << g_temp_rotation[i].y << ", " << g_temp_rotation[i].z << endl;*/

			sc_player_data_packet packet;

			packet.size = sizeof(sc_player_data_packet);
			packet.type = SC_PACKET::SC_PLAYER_DATA;
			packet.id = m_users[i]->GetID();


			iVector3 character_position = m_characters[i]->GetPosition();
			sRotation3 character_rotation = m_characters[i]->GetRotation();

			packet.x = character_position.x;
			packet.y = character_position.y;
			packet.z = character_position.z;
			packet.pitch = character_rotation.pitch;
			packet.yaw = character_rotation.yaw;
			packet.roll = character_rotation.roll;

			cout << "SC_PLAYER_DATA to User [ " << 1-i << " ] In Room [ "<<(int)m_room_id<<" ] // User [ "<<i 
				<<" ] position " << packet.x << "," << packet.y << ", " << packet.z
				<< " // rotation " << packet.pitch << ", " << packet.yaw << "," << packet.roll  << endl;
			
			m_users[1-i]->Send(sizeof(packet), &packet);
		}
	}
}

void cRoom::SetCharacterPosition(const unsigned short& _user_id, const short& _x, const short& _y, const short& _z)
{

};
void cRoom::SetCharacterRotation(const unsigned short& _user_id, const short& _pitch, const short& _yaw, const short& _roll)
{

};
void cRoom::SetCharacterTransform(const unsigned short& _user_id, const short& _x, const short& _y, const short& _z,
	const short& _pitch, const short& _yaw, const short& _roll)
{

};

cRoom::cRoom() {
	m_users[0] = nullptr;
	m_users[1] = nullptr;
	m_characters[0] = nullptr;
	m_characters[1] = nullptr;
};
cRoom::~cRoom() {};