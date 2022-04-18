#pragma once
#include "stdafx.h"
#include "Room.h"
#include "Character.h"
#include "UserManager.h"
#include "User.h"

cRoom::cRoom() {};
cRoom::~cRoom() {};


void cRoom::StartGame()
{
	////월드 초기값 설정

	////
	//if (m_state == room_state::INGAME)
	//{
	//	for (int i = 0; i < 2; ++i)
	//	{
	//		sc_put_object_packet packet;

	//		packet.size = sizeof(sc_put_object_packet);
	//		packet.type = SC_PACKET::SC_PUT_OBJECT;
	//		packet.object_type = 0;     // 임시값. 오브젝트별 ID 분류 나온 후 수정 필요
	//		packet.id = m_user_id[i];

	//		m_characters[i]->SetPosition(0, 0, 0);
	//		m_characters[i]->SetRotation(0, 0, 0);

	//		iVector3 character_position = m_characters[i]->GetPosition();
	//		sRotation3 character_rotation = m_characters[i]->GetRotation();

	//		packet.x = character_position.x;
	//		packet.y = character_position.y;
	//		packet.z = character_position.z;
	//		packet.pitch = character_rotation.pitch;
	//		packet.yaw = character_rotation.yaw;
	//		packet.roll = character_rotation.roll;


	//		m_users[user_type::HOST]->Send(sizeof(packet), &packet);
	//		m_users[user_type::GUEST]->Send(sizeof(packet), &packet);
	//	}
	//}

}

void cRoom::UserLoadingComplete(const unsigned int _user_id)
{

}

void cRoom::SendOtherPlayerTransform()
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
			packet.id = m_user_id[i];

			cCharacter* character = cUserManager::m_users[m_user_id[i]]->m_character;

			packet.x = character->m_position.x;
			packet.y = character->m_position.y;
			packet.z = character->m_position.z;
			packet.pitch = character->m_rotation.pitch;
			packet.yaw = character->m_rotation.yaw;
			packet.roll = character->m_rotation.roll;

			packet.head_x = character->m_head_position.x;
			packet.head_y = character->m_head_position.y;
			packet.head_z = character->m_head_position.z;
			packet.head_pitch = character->m_head_rotation.pitch;
			packet.head_yaw = character->m_head_rotation.yaw;
			packet.head_roll = character->m_head_rotation.roll;

			packet.rh_x = character->m_rh_position.x;
			packet.rh_y = character->m_rh_position.y;
			packet.rh_z = character->m_rh_position.z;
			packet.rh_pitch = character->m_rh_rotation.pitch;
			packet.rh_yaw = character->m_rh_rotation.yaw;
			packet.rh_roll = character->m_rh_rotation.roll;

			packet.lh_x = character->m_lh_position.x;
			packet.lh_y = character->m_lh_position.y;
			packet.lh_z = character->m_lh_position.z;
			packet.lh_pitch = character->m_lh_rotation.pitch;
			packet.lh_yaw = character->m_lh_rotation.yaw;
			packet.lh_roll = character->m_lh_rotation.roll;

			cout << "SC_PLAYER_DATA to User [ " << 1-i << " ] In Room [ "<<(int)m_id<<" ] // User [ "<<i 
				<<" ] position " << packet.x << "," << packet.y << ", " << packet.z
				<< " // rotation " << packet.pitch << ", " << packet.yaw << "," << packet.roll  << endl;
			
			cUserManager::m_users[m_user_id[1-i]]->Send(sizeof(packet), &packet);
		}
	}
}

void cRoom::Broadcast(int _size, void* _mess)
{
	cUserManager::m_users[m_user_id[0]]->Send(_size, _mess);
	cUserManager::m_users[m_user_id[1]]->Send(_size, _mess);
}

