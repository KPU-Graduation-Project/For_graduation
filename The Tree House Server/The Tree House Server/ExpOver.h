#pragma once
#include <WS2tcpip.h>
#include <MSWSock.h>
#include "protocol.h"
#include <map>

enum OP_TYPE { OP_RECV, OP_SEND, OP_ACCEPT,OP_SEND_WORLD_DATA };

class CEXP_OVER {
public:
	WSAOVERLAPPED	m_wsa_over;
	OP_TYPE			m_comp_op;
	WSABUF			m_wsa_buf;
	unsigned char	m_net_buf[BUFSIZE];
public:
	CEXP_OVER(OP_TYPE comp_op, char num_bytes, void* mess);

	CEXP_OVER(OP_TYPE comp_op);

	CEXP_OVER();

	~CEXP_OVER();
};