#pragma once
#include "stdafx.h"
#include "Room.h"
#include "GameObject.h"
#include "Character.h"
#include "UserManager.h"
#include "User.h"
#include "Bullet.h"
#include "Monster.h"


void cRoom::Init()
{
	InitializeCriticalSection(&m_state_cs);
}

void cRoom::InitObjects()
{


}

void cRoom::StartGame()
{

	//월드 초기 설정


	//캐릭터 설정
	{
		{
			cCharacter* character = cRoomManager::m_character_pool.PopObject();
			character->m_id = m_users[HOST]->GetID();
			character->m_object_type = OBJECT_TYPE::GIRL;
			character->m_mesh_id = 0;
			character->m_state = OBJECT_STATE::ALIVE;
			character->SetScale({ 100,100,100 });
			character->SetCharacterTransform({ -68000,19000, 9200 }, { 0,0,0 },
				{ 0,0,0 }, { 0,0,0 }, { 0, 0, 0 }, { 0,0,0 }, { 0,0,0 }, { 0,0,0 });
			character->m_sector = 1;
			character->m_parent_id;
			m_users[HOST]->m_character = character;

			sc_put_object_packet packet;
			packet.size = sizeof(sc_put_object_packet);
			packet.type = SC_PACKET::SC_PUT_OBJECT;
			packet.id = m_users[HOST]->GetID();
			packet.object_type = character->m_object_type;
			packet.mesh_id = 0;
			packet.parent_object_id = 0;

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
			character->m_id = m_users[GUEST]->GetID();
			character->m_object_type = OBJECT_TYPE::BOY;
			character->m_mesh_id = 0;
			character->m_state = OBJECT_STATE::ALIVE;
			character->SetScale({ 100,100,100 });
			character->SetCharacterTransform({ 171200,21700,9200 }, { 0,0,-9000 }, { 0,0,0 }, { 0,0,0 }, { 0, 0, 0 }, { 0,0,0 }, { 0,0,0 }, { 0,0,0 });

			character->m_sector = 1;
			character->m_parent_id;
			m_users[GUEST]->m_character = character;

			sc_put_object_packet packet;
			packet.size = sizeof(sc_put_object_packet);
			packet.type = SC_PACKET::SC_PUT_OBJECT;
			packet.id = m_users[GUEST]->GetID();
			packet.object_type = character->m_object_type;
			packet.mesh_id = 0;
			packet.parent_object_id = 0;

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
	}

	//월드 오브젝트
	{
		{//12
			cGameObject* game_object = cRoomManager::m_object_pool.PopObject();
			game_object->m_id = cRoomManager::m_last_object_id++;
			game_object->m_object_type = OBJECT_TYPE::TARGET;
			game_object->m_mesh_id = 0;
			game_object->m_state = OBJECT_STATE::ALIVE;
			game_object->SetTransform({ { -74563,-135681,7747 }, { 0,0,0 }, {278,278,278} });
			game_object->m_sector = 1;
			game_object->m_parent_id = 0;
			m_game_objects.insert(pair<unsigned int, cGameObject*>{game_object->m_id, game_object});
			SendPutObject(game_object->m_id);
		}
		{  //2
			cGameObject* game_object = cRoomManager::m_object_pool.PopObject();
			game_object->m_id = cRoomManager::m_last_object_id++;
			game_object->m_object_type = OBJECT_TYPE::DOOR;		
			game_object->m_mesh_id = 0;
			game_object->m_state = OBJECT_STATE::ALIVE;
			game_object->SetTransform({ { -68443,-170107,583 }, { 0,0,0 }, {159,159,159} });
			game_object->m_sector = 1;
			game_object->m_parent_id = game_object->m_id-1;
			m_game_objects.insert(pair<unsigned int, cGameObject*>{game_object->m_id, game_object});	
			SendPutObject(game_object->m_id);
		}		
		{//3
			cGameObject* game_object = cRoomManager::m_object_pool.PopObject();
			game_object->m_id = cRoomManager::m_last_object_id++;
			game_object->m_object_type = OBJECT_TYPE::BOX;
			game_object->m_mesh_id = 0;
			game_object->m_state = OBJECT_STATE::ALIVE;
			game_object->SetTransform({ { -233838,-133995,48376 }, { 0,0,0 }, {131,131,131} });
			game_object->m_sector = 1;
			game_object->m_parent_id = 0;
			m_game_objects.insert(pair<unsigned int, cGameObject*>{game_object->m_id, game_object});
			SendPutObject(game_object->m_id);
		}
		{//4
			cGameObject* game_object = cRoomManager::m_object_pool.PopObject();
			game_object->m_id = cRoomManager::m_last_object_id++;
			game_object->m_object_type = OBJECT_TYPE::DOOR;
			game_object->m_mesh_id = 1;
			game_object->m_state = OBJECT_STATE::ALIVE;
			game_object->SetTransform({ { -219798,-166375,-224 }, { 0,-1000,0 }, {65,65,65} });
			game_object->m_sector = 1;
			game_object->m_parent_id = (game_object->m_id-1);
			m_game_objects.insert(pair<unsigned int, cGameObject*>{game_object->m_id, game_object});
			SendPutObject(game_object->m_id);
		}		
		{//5
			cGameObject* game_object = cRoomManager::m_object_pool.PopObject();
			game_object->m_id = cRoomManager::m_last_object_id++;
			game_object->m_object_type = OBJECT_TYPE::DESTRUCTIBLE;
			game_object->m_mesh_id = 2;
			game_object->m_state = OBJECT_STATE::ALIVE;
			game_object->SetTransform({ { -229869,-432744,0 }, { 0,-14499,0 }, {246,246,246} });
			game_object->m_sector = 1;
			game_object->m_parent_id = 0;
			m_game_objects.insert(pair<unsigned int, cGameObject*>{game_object->m_id, game_object});
			SendPutObject(game_object->m_id);
		}
		{//6
			cGameObject* game_object = cRoomManager::m_object_pool.PopObject();
			game_object->m_id = cRoomManager::m_last_object_id++;
			game_object->m_object_type = OBJECT_TYPE::DESTRUCTIBLE;
			game_object->m_mesh_id = 1;
			game_object->m_state = OBJECT_STATE::ALIVE;
			game_object->SetTransform({ { -93807,-441730,0 }, { 0,999,0 }, {284,284,284} });
			game_object->m_sector = 1;
			game_object->m_parent_id = 0;
			m_game_objects.insert(pair<unsigned int, cGameObject*>{game_object->m_id, game_object});
			SendPutObject(game_object->m_id);
					}
		{
			cGameObject* game_object = cRoomManager::m_object_pool.PopObject();
			game_object->m_id = cRoomManager::m_last_object_id++;
			game_object->m_object_type = OBJECT_TYPE::DESTRUCTIBLE;
			game_object->m_mesh_id = 1;
			game_object->m_state = OBJECT_STATE::ALIVE;
			game_object->SetTransform({ { -238294,-510670,0 }, { 0,0,0 }, {321,321,321} });
			game_object->m_sector = 1;
			game_object->m_parent_id = 0;
			m_game_objects.insert(pair<unsigned int, cGameObject*>{game_object->m_id, game_object});
			SendPutObject(game_object->m_id);
					}
		{//8
			cGameObject* game_object = cRoomManager::m_object_pool.PopObject();
			game_object->m_id = cRoomManager::m_last_object_id++;
			game_object->m_object_type = OBJECT_TYPE::DESTRUCTIBLE;
			game_object->m_mesh_id = 1;
			game_object->m_state = OBJECT_STATE::ALIVE;
			game_object->SetTransform({ { -156704,-568106,-1172 }, { 0,1000,0 }, {246,246,246} });
			game_object->m_sector = 1;
			game_object->m_parent_id = 0;
			m_game_objects.insert(pair<unsigned int, cGameObject*>{game_object->m_id, game_object});
			SendPutObject(game_object->m_id);
					}
		{
			cMonster* game_object = cRoomManager::m_monster_pool.PopObject();
			game_object->m_id = cRoomManager::m_last_object_id++;
			game_object->m_object_type = OBJECT_TYPE::TARGET_DOLL;
			game_object->m_mesh_id = 0;
			game_object->m_state = OBJECT_STATE::ALIVE;
			game_object->SetTransform({ { -564302,-342228,-104675 }, { 0,0,0 }, {246,246,246} });
			game_object->m_sector = 1;
			game_object->m_parent_id = 0;
			game_object->m_behavior = cPlayZoneTarget::BEHAVIOR::BH1_NONE;
			m_game_objects.insert(pair<unsigned int, cMonster*>{game_object->m_id, game_object});
			SendPutObject(game_object->m_id);
		}
		{//10
			cPlayZoneTarget* game_object = reinterpret_cast<cPlayZoneTarget*>(cRoomManager::m_monster_pool.PopObject());
			game_object->m_id = cRoomManager::m_last_object_id++;
			game_object->m_object_type = OBJECT_TYPE::PLAY_ZONE_TARGET;
			game_object->m_mesh_id = 1;
			game_object->m_state = OBJECT_STATE::ALIVE;
			//game_object->SetTransform({ { -593700,-384900,55500 }, { 9000,1403,10403 }, {100,100,100} });
			game_object->SetTransform({ {	 -71000, -18000, 9200  }, { 9000,-1403,7596 }, {100,100,100} });
			game_object->m_sector = 1;
			game_object->m_parent_id = 0;
			game_object->m_behavior = cPlayZoneTarget::BEHAVIOR::BH2_NONE;
			m_game_objects.insert(pair<unsigned int, cMonster*>{game_object->m_id, game_object});
			SendPutObject(game_object->m_id);

			game_object->ProgressNextBehavior();
		}
		{
			cPlayZoneTarget* game_object = reinterpret_cast<cPlayZoneTarget*>(cRoomManager::m_monster_pool.PopObject());
			game_object->m_id = cRoomManager::m_last_object_id++;
			game_object->m_object_type = OBJECT_TYPE::PLAY_ZONE_TARGET;
			game_object->m_mesh_id = 1;
			game_object->m_state = OBJECT_STATE::ALIVE;
			//game_object->SetTransform({ { -593700,-357600,40200 }, { 9000,-1403,7596 }, {100,100,100} });
			game_object->SetTransform({ {	 -73000, -18000, 9200  }, { 9000,-1403,7596 }, {100,100,100} });

			game_object->m_sector = 1;
			game_object->m_parent_id = 0;
			game_object->m_behavior = cPlayZoneTarget::BEHAVIOR::BH1_NONE;
			m_game_objects.insert(pair<unsigned int, cMonster*>{game_object->m_id, game_object});
			SendPutObject(game_object->m_id);

			game_object->ProgressNextBehavior();
		}
	}

	
	m_state = ROOM_STATE::INGAME;
	std::cout << "Room [ " << m_id << " ] Started Game\n";
}

void cRoom::UserLoadingComplete(const unsigned int _user_id)
{





}

void cRoom::SendPutObject(const unsigned int _object_id)
{
	auto object = m_game_objects[_object_id];
	sc_put_object_packet packet;
	packet.size = sizeof(sc_put_object_packet);
	packet.type = SC_PACKET::SC_PUT_OBJECT;
	packet.id = object->m_id;
	packet.object_type = object->m_object_type;
	packet.mesh_id = object->m_mesh_id;
	packet.parent_object_id = object->m_parent_id;

	Transform transform = object->GetTransform();

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




void cRoom::SendPlayerData()
{
	static char cnt[2] = {0,0};
	if (m_state == ROOM_STATE::INGAME)
	{
		for (int i = 0; i < 2; ++i)
		{
			/*cout << "send - user" << i << " position " << g_temp_position[i].x << "," << g_temp_position[i].y << ", " << g_temp_position[i].z
				<< " // rotation " << g_temp_rotation[i].x << "," << g_temp_rotation[i].y << ", " << g_temp_rotation[i].z << endl;*/
		
			
			sc_player_data_packet packet;

			packet.size = sizeof(sc_player_data_packet);
			packet.type = SC_PACKET::SC_PLAYER_DATA;
			packet.id = m_users[1 - i]->GetID();

			cCharacter* character = m_users[1 - i]->m_character;

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

		
			/*	cout << "SC_PLAYER_DATA to User [ " << m_users[i]->GetID() << " ] In Room [ " << (int)m_id << " ] // User [ " << m_users[1 - i]->GetID()
					<< " ] position " << packet.x << "," << packet.y << ", " << packet.z
					<< " // rotation " << packet.pitch << ", " << packet.yaw << "," << packet.roll << endl;
		*/

			//cout << "size: " << sizeof(packet) << endl;
			m_users[i]->Send(sizeof(packet), &packet);
		}
	}
}

void cRoom::SendAllObjectData()
{
	// 뷰리스트 등을 통한 최적화 필요
	for (const auto& p : m_game_objects)
	{
		if (p.second->m_object_type / 100000 != 2)
			continue;

		sc_object_data_packet packet;

		packet.size = sizeof(sc_object_data_packet);
		packet.type = SC_PACKET::SC_OBJECT_DATA;
		packet.id = p.second->m_id;

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

		//cout << "Send SC_OBJECT_DATA: Object [ " << packet.id << " ] position: " << packet.x << ", " << packet.y << ", " << packet.z << "\n";
	}

}


void cRoom::ShootBullet(UserRef _user, iVector3 _source_position, sRotation3 _rotation)
{
	cBullet* new_bullet = cRoomManager::m_bullet_pool.PopObject();

	new_bullet->m_id = cRoomManager::m_last_object_id++;
	new_bullet->m_mesh_id = 0;

	// 수정 필요
	if (m_users[HOST] == _user)
		new_bullet->m_object_type = OBJECT_TYPE::MATCH;
	else
		new_bullet->m_object_type = OBJECT_TYPE::SAP;
	new_bullet->SetTransform({ _source_position,_rotation,{100,100,100} });

	new_bullet->m_launch_position = _source_position;
	new_bullet->m_launch_rotation = _rotation;
	new_bullet->m_state = OBJECT_STATE::ALIVE;
	m_game_objects.insert(pair<unsigned int, cBullet*>{new_bullet->m_id, new_bullet});

	sc_shoot_bullet_packet packet;
	packet.type = SC_PACKET::SC_SHOOT_BULLET;
	packet.size = sizeof(sc_shoot_bullet_packet);

	// 로비 제작 후 캐릭터 선택 가능하게 되면 수정 필요
	packet.bullet_type = new_bullet->m_object_type - OBJECT_TYPE::BULLET;

	packet.id = new_bullet->m_id;
	packet.x = _source_position.x;
	packet.y = _source_position.y;
	packet.z = _source_position.z;
	packet.pitch = _rotation.pitch;
	packet.yaw = _rotation.yaw;
	packet.roll = _rotation.roll;

	Broadcast(sizeof(sc_shoot_bullet_packet), &packet);
}

void cRoom::BulletHit(unsigned int _bullet_id, unsigned int _hit_object_id, iVector3 _position, sRotation3 _rotation)
{
	//if bullet is match
	if (m_game_objects[_bullet_id]->m_object_type == OBJECT_TYPE::MATCH)
	{
		sc_destroy_object_packet packet;
		packet.size = sizeof(sc_destroy_object_packet);
		packet.type = SC_PACKET::SC_DESTROY_OBJECT;
		packet.id = _bullet_id;

		Broadcast(sizeof(sc_destroy_object_packet), &packet);

		// 수정필요
		m_game_objects.unsafe_erase(_bullet_id);

		// match - sap hit check on match's side
		if ((_hit_object_id != 0) && (m_game_objects[_hit_object_id]->m_object_type == OBJECT_TYPE::SAP))
		{
			sc_destroy_object_packet packet;
			packet.size = sizeof(sc_destroy_object_packet);
			packet.type = SC_PACKET::SC_DESTROY_OBJECT;
			packet.id = _hit_object_id;

			Broadcast(sizeof(sc_destroy_object_packet), &packet);
			// 수정필요
			m_game_objects.unsafe_erase(_hit_object_id);
		}

	}
	//if bullet is sap
	else if (m_game_objects[_bullet_id]->m_object_type == OBJECT_TYPE::SAP)
	{
		// Hit terreain
		if (_hit_object_id == 0)
		{
			m_game_objects[_bullet_id]->m_transform.position = _position;
			m_game_objects[_bullet_id]->m_transform.rotation = _rotation;
			m_game_objects[_bullet_id]->m_state = OBJECT_STATE::STOP;
		}
		else if (m_game_objects[_hit_object_id]->m_object_type == OBJECT_TYPE::MATCH);
		//pass: match - sap hit check on match's side
		else
		{
			// 추후 붙은 오브젝트의 로컬좌표로 수정 필요
			m_game_objects[_bullet_id]->m_transform.position = _position;
			m_game_objects[_bullet_id]->m_transform.rotation = _rotation;
			m_game_objects[_bullet_id]->m_state = OBJECT_STATE::STOP;
		}

	}
	else  std::cout << "bullet hit error\n";

};

void cRoom::Update(float _delta_second)
{
	for (auto object : m_game_objects)
	{
		if (object.second->m_object_type / 100000 == 2)
		{

			switch (object.second->m_object_type)
			{
			case OBJECT_TYPE::PLAY_ZONE_TARGET:
			{
				reinterpret_cast<cPlayZoneTarget*>(object.second)->Update(_delta_second);
				break;
			}
			case OBJECT_TYPE::TARGET_DOLL:
			{
				//reinterpret_cast<cTargetDoll*>(object->second)->ProgressNextBehavior();
				break;
			}
			}
		}
		else
		object.second->Update(_delta_second);
	}
}

void cRoom::Broadcast(int _size, void* _mess)
{
	m_users[0]->Send(_size, _mess);
	m_users[1]->Send(_size, _mess);
}

void cRoom::Disconnect(UserRef _user)
{
	StateLock();
	m_state = ROOM_STATE::DISCONNECTING;
	// 방 상태 전환

	// 유저 캐릭터 삭제
	   // 유저 캐릭터 삭제 패킷 전송
	   // 유저 캐릭터 메모리 해제

	// 유저 삭제

}