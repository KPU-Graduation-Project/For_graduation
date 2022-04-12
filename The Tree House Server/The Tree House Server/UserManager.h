#pragma once
#include <unordered_map>

class cUser;

class cUserManager
{
public:
	cUserManager();
	~cUserManager();

	void Init();
	void InitUsers();
	unsigned short GetNewID();

public: 
	//unordered_map <unsigned short, cUser> m_users;
	//cUser m_users[MAX_USER];
	cUser* m_users;

	unsigned int m_connected_user_num;
};