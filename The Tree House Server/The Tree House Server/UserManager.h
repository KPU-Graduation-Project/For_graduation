#pragma once
#include <unordered_map>

class cUser;

class cUserManager
{
public:
	cUserManager() {};
	~cUserManager() {};

	void Init();
	void InitUsers();
	unsigned int GetNewID();

public: 
	static unordered_map <unsigned int, UserRef> m_users;

private:
	
};