#pragma once

class CUser;

class CUserManager
{
public:
	CUserManager();
	~CUserManager();

	void Init();
	unsigned int GetNewID();
	void AddNewUser(const unsigned int& _user_id, SOCKET& _socket);
	void UserInit(const unsigned int& _user_id);

	void UserSend(const unsigned int& _user_id, int num_bytes, void* mess);
	void UserRecv(const unsigned int& _user_id);

	int GetPrevSize(const unsigned int& _user_id);

	void SetPrevSize(const unsigned int& _user_id, int& _prev_size);

private:
	CUser* m_users;
	unsigned int m_connected_user_num;
};