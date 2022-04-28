#pragma once
#include "stdafx.h"
#include "Room.h"
#include "GameObject.h"
#include "Character.h"
#include "UserManager.h"
#include "User.h"

int cRoom::m_last_object_id = 0;



void cRoom::InitObjects()
{


}

void cRoom::StartGame()
{
	
	//월드 초기 설정


	//캐릭터 설정
	{
		cCharacter* character = cRoomManager::m_character_pool.PopObject();
		character->SetScale({ 1,1,1 });
		character->SetCharacterTransform({ -68000,19000,9200 }, { 0,0,-9000 },
			{ 0,0,0 }, { 0,0,0 }, { 0, 0, 0 }, { 0,0,0 }, { 0,0,0 }, { 0,0,0 });
		character->m_object_type = 1;
		character->m_id = m_user_id[0];
		cUserManager::m_users[m_user_id[0]]->m_character = character;

		sc_put_object_packet packet;

		packet.size = sizeof(sc_put_object_packet);
		packet.type = SC_PACKET::SC_PUT_OBJECT;
		packet.object_type = 100001;
		packet.id = m_user_id[0];

		Transform transform = character->GetTransform();
		packet.x = transform.position.x;
		packet.y = transform.position.y;
		packet.z = transform.position.z;
		packet.pitch = transform.rotation.pitch;
		packet.yaw = transform.rotation.yaw;
		packet.roll = transform.rotation.roll;
		packet.scale_x = transform.scale.x;
		packet.scale_y = transform.scale.y;
		packet.scale_z = transform.scale.z;

		Broadcast(sizeof(sc_put_object_packet), &packet);
	}

	{
		cCharacter* character = cRoomManager::m_character_pool.PopObject();
		character->SetScale({ 1,1,1 });
		character->SetCharacterTransform({ 1712,21700,9200 }, { 0,0,-9000 },
			{ 0,0,0 }, { 0,0,0 }, { 0, 0, 0 }, { 0,0,0 }, { 0,0,0 }, { 0,0,0 });
		character->m_object_type = 100002;
		character->m_id = m_user_id[1];
		cUserManager::m_users[m_user_id[1]]->m_character = character;

		sc_put_object_packet packet;

		packet.size = sizeof(sc_put_object_packet);
		packet.type = SC_PACKET::SC_PUT_OBJECT;
		packet.object_type = 2;
		packet.id = m_user_id[1];

		Transform transform = character->GetTransform();
		packet.x = transform.position.x;
		packet.y = transform.position.y;
		packet.z = transform.position.z;
		packet.pitch = transform.rotation.pitch;
		packet.yaw = transform.rotation.yaw;
		packet.roll = transform.rotation.roll;
		packet.scale_x = transform.scale.x;
		packet.scale_y = transform.scale.y;
		packet.scale_z = transform.scale.z;

		Broadcast(sizeof(sc_put_object_packet), &packet);
	}


	//월드 오브젝트
	{
		cGameObject* game_object = cRoomManager::m_object_pool.PopObject();
		game_object->SetTransform({ { -233838,-133995,48376 }, { 0,0,0 }, {131,131,131} });
		game_object->m_object_type = 400002;
		game_object->m_id = MAX_USER + m_last_object_id++;
		game_object->m_mesh_id = 0;
		m_game_objects.insert(pair<unsigned int, cGameObject*>{game_object->m_id, game_object});
		sc_put_object_packet packet;

		packet.size = sizeof(sc_put_object_packet);
		packet.type = SC_PACKET::SC_PUT_OBJECT;
		packet.object_type = game_object->m_object_type;
		packet.id = game_object->m_id;

		Transform transform = game_object->GetTransform();
		packet.x = transform.position.x;
		packet.y = transform.position.y;
		packet.z = transform.position.z;
		packet.pitch = transform.rotation.pitch;
		packet.yaw = transform.rotation.yaw;
		packet.roll = transform.rotation.roll;
		packet.scale_x = transform.scale.x;
		packet.scale_y = transform.scale.y;
		packet.scale_z = transform.scale.z;

		Broadcast(sizeof(sc_put_object_packet), &packet);
	}

	cout << "Room [ " << m_id << " ] Started Game\n";
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

			packet.x = character->m_transform.position.x;
			packet.y = character->m_transform.position.y;
			packet.z = character->m_transform.position.z;
			packet.pitch = character->m_transform.rotation.pitch;
			packet.yaw = character->m_transform.rotation.yaw;
			packet.roll = character->m_transform.rotation.roll;

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

void cRoom::SendAllObjectData()
{
	// 뷰리스트 등을 통한 최적화 필요
	for (const auto& p : m_game_objects)
	{
		sc_object_data_packet packet;

		packet.size = sizeof(sc_object_data_packet);
		packet.type = SC_PACKET::SC_OBJECT_DATA;
		packet.id = p.first;

		cGameObject* object = p.second;
		packet.x = object->m_transform.position.x;
		packet.y = object->m_transform.position.y;
		packet.z = object->m_transform.position.z;
		packet.pitch = object->m_transform.rotation.pitch;
		packet.yaw = object->m_transform.rotation.yaw;
		packet.roll = object->m_transform.rotation.roll;
		packet.scale_x = object->m_transform.position.x;
		packet.scale_y = object->m_transform.position.y;
		packet.scale_z = object->m_transform.position.z;

		Broadcast(sizeof(sc_object_data_packet), &packet);
	}

}

void cRoom::ShootBullet(const unsigned int& _user_id, iVector3 _source_position, sRotation3 _rotation)
{

}
void cRoom::Broadcast(int _size, void* _mess)
{
	cUserManager::m_users[m_user_id[0]]->Send(_size, _mess);
	cUserManager::m_users[m_user_id[1]]->Send(_size, _mess);
}

