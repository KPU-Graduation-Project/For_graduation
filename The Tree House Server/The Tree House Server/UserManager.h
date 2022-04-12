#pragma once
#include <unordered_map>
#include <mutex>

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
	unordered_map <unsigned short, cUser*> m_users;
	//cUser m_users[MAX_USER];
	//cUser* m_users;


private:
	unsigned short m_last_id;
	mutex          m_last_id_lock;
};